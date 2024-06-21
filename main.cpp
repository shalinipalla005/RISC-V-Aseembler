#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iomanip>
#include <bitset>
#include <regex>
using namespace std;

unordered_map<string, string> register_map = {
    {"x0", "00000"}, {"x1", "00001"}, {"x2", "00010"}, {"x3", "00011"}, {"x4", "00100"}, {"x5", "00101"}, {"x6", "00110"}, {"x7", "00111"},
    {"x8", "01000"}, {"x9", "01001"}, {"x10", "01010"}, {"x11", "01011"}, {"x12", "01100"}, {"x13", "01101"}, {"x14", "01110"}, {"x15", "01111"},
    {"x16", "10000"}, {"x17", "10001"}, {"x18", "10010"}, {"x19", "10011"}, {"x20", "10100"}, {"x21", "10101"}, {"x22", "10110"}, {"x23", "10111"},
    {"x24", "11000"}, {"x25", "11001"}, {"x26", "11010"}, {"x27", "11011"}, {"x28", "11100"}, {"x29", "11101"}, {"x30", "11110"}, {"x31", "11111"}
};

unordered_map<string, string> register_valmap = {
    {"x0", "00000"}, {"x1", "00000"}, {"x2", "7FFFFFDC"}, {"x3", "10008000"}, {"x4", "00000"}, {"x5", "00000"}, {"x6", "00000"}, {"x7", "00000"},
    {"x8", "00000"}, {"x9", "00000"}, {"x10", "00000"}, {"x11", "00000"}, {"x12", "00000"}, {"x13", "00000"}, {"x14", "00000"}, {"x15", "00000"},
    {"x16", "00000"}, {"x17", "00000"}, {"x18", "00000"}, {"x19", "00000"}, {"x20", "00000"}, {"x21", "00000"}, {"x22", "00000"}, {"x23", "00000"},
    {"x24", "00000"}, {"x25", "00000"}, {"x26", "00000"}, {"x27", "00000"}, {"x28", "00000"}, {"x29", "00000"}, {"x30", "00000"}, {"x31", "00000"}
};

unordered_map<string, string> opcode_map = {
    {"add", "0110011"}, {"and", "0110011"}, {"or", "0110011"}, {"sll", "0110011"}, {"slt", "0110011"},
    {"sra", "0110011"}, {"srl", "0110011"}, {"sub", "0110011"}, {"xor", "0110011"}, {"mul", "0110011"},
    {"div", "0110011"}, {"rem", "0110011"}, {"addi", "0010011"}, {"andi", "0010011"}, {"ori", "0010011"},
    {"lb", "0000011"}, {"ld", "0000011"}, {"lh", "0000011"}, {"lw", "0000011"}, {"jalr", "1100111"},
    {"sb", "0100011"}, {"sw", "0100011"}, {"sd", "0100011"}, {"sh", "0100011"}, {"beq", "1100011"},
    {"bne", "1100011"}, {"bge", "1100011"}, {"blt", "1100011"}, {"auipc", "0010111"}, {"lui", "0110111"},
    {"jal", "1101111"}
};

unordered_map<string, string> formats = {
    {"0110011", "R"}, {"0010011", "I"}, {"0000011", "I"}, {"1100111", "I"}, {"0100011", "S"}, 
    {"1100011", "SB"}, {"0010111", "U"}, {"0110111", "U"}, {"1101111", "UJ"} 
};

unordered_map<string, string> func3 = {
    {"add", "000"}, {"and", "111"}, {"or", "110"}, {"sll", "001"}, {"slt", "010"},{"sra", "101"},
    {"srl", "101"}, {"sub", "000"}, {"xor", "100"}, {"mul", "000"}, {"div", "100"}, {"rem", "110"},
    {"addi", "000"}, {"andi", "111"}, {"ori", "110"},{"lb", "000"}, {"ld", "011"}, {"lh", "001"},
    {"lw", "010"}, {"jalr", "000"}, {"sb", "000"}, {"sw", "010"}, {"sd", "011"}, {"sh", "001"}, 
    {"beq", "000"}, {"bne", "001"}, {"bge", "101"}, {"blt", "100"}
};

unordered_map<string, string> func7 = {
    {"add", "0000000"}, {"and", "0000000"}, {"or", "0000000"}, {"sll", "0000000"}, {"slt", "0000000"},
    {"sra", "0100000"}, {"srl", "0000000"}, {"sub", "0100000"}, {"xor", "0000000"}, {"mul", "0000001"},
    {"div", "0000001"}, {"rem", "0000001"}
};

bool isEmptyLine(const string& line) {
    // Check if the line is empty or contains only whitespace characters
    return line.empty() || (line.find_first_not_of(" \t") == string::npos);
}

bool skipLabelAndGetInstruction(string& line){
    size_t labelPos = line.find(":");
    if (labelPos == string::npos) {
        // If no label is found, set instruction and return true
        //instruction = line;
        return false;
    }
    // Return true if label is found end of file is reached without finding an instruction
    return true;
}

int processFileUntilLabel(const string& filePath, const string& label) {
    ifstream file(filePath);
    if (!file.is_open()) {
        return -1; // File not opened
    }

    string line;
    int lineNum = 0;
    bool getLine=true;
    while (getline(file, line)) {
        stringstream sk(line);
        string check;
        sk >> check;
        if (check == ".data"){
            getLine = false;
            continue;
        }
        if (check == ".text"){
            getLine = true;
            continue;
        }
        if(!getLine || isEmptyLine(line) || check.substr(0,1) == "#"){
            continue;
        }
        
        // Use stringstream to extract the first word
        stringstream ss(line);
        string firstWord;
        ss >> firstWord;

        // Check if the first word matches the label
        if (firstWord == label | firstWord == label+":") {
            file.close();
            return lineNum; // Return the line number
        }
        bool isLabel=skipLabelAndGetInstruction(line);
        if (isLabel==true){
            size_t colonPos = line.find(":");
            if (colonPos != string::npos) {
            // Found ":" in the line, extract the substring from that position
            line = line.substr(colonPos+1);
            }
            isLabel=false;
        }
        size_t hashPos = line.find("#");
        if (hashPos != string::npos){
            line=line.substr(0,hashPos);
        }
        if(isEmptyLine(line)){
            continue;
        }
        ++lineNum;
    }

    // Close the file
    file.close();
    return -1; // Label not found
}

vector<string> split(const string& s) {
    vector<string> tokens;
    string token;
    stringstream tokenStream(s);
    tokenStream >> token;
    tokens.push_back(token);
    while(tokenStream>>token){
        stringstream ss(token);
        string registerToken;
        while (getline(ss, registerToken, ',')) {
            tokens.push_back(registerToken);
        }
    }
    return tokens;
}

string binaryToHex(const string& binary) {
    // Convert binary string to integer
    bitset<32> bits(binary);
    unsigned long num = bits.to_ulong();
    
    // Convert integer to hexadecimal string
    stringstream stream;
    stream << uppercase << hex << num;
    string hex = stream.str();
    
    // Ensure the hex string has 8 characters (32 bits)
    hex = string(8 - hex.length(), '0') + hex;
    
    return hex;
}

string intTo20BitBinaryString(int num) {
    // Mask out the 20 least significant bits
    num &= 0x1FFFFF;
    // If the number is negative, set the MSB to 1
    if (num < 0)
        num |= 0x100000;
    // Convert the integer to a bitset and then to a string
    return bitset<21>(num).to_string();
}

string intToHex(int value) {
    stringstream stream;
    stream << uppercase << hex << value; // Convert integer to hexadecimal string
    return stream.str(); // Return the hexadecimal string
}

vector<string> parseInputString(const string& input) {
    vector<string> result;
    stringstream ss(input);
    string value;
    while (getline(ss, value, ',')) {
        result.push_back(value);
    }
    return result;
}

string intToHex(int num, int bytes) {
    stringstream ss;
    if(num<0){
    ss << uppercase << setfill('F') << setw(bytes * 2) << hex << (unsigned int)num;}
    else{
       ss << uppercase << setfill('0') << setw(bytes * 2) << hex << (unsigned int)num; 
    }
    return ss.str();
}

bool hasQuotes(const stringstream& ss) {
    // Read the entire stringstream into a string
    string str = ss.str();

    // Check if opening and closing quotes are present
    size_t firstQuote = str.find('"');
    size_t lastQuote = str.rfind('"');
    
    return firstQuote != string::npos && lastQuote != string::npos && firstQuote != lastQuote;
}

vector<string> dataSeg(const string& filePath) {
    vector<string> data;
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return data;
    }

    string line;
    bool inDataSection = false;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        ss >> token;

        if (token == ".data") {
            inDataSection = true;
            continue;
        }
         
        if (token == ".text"){
            inDataSection = false;
            break;
        }

        if (inDataSection && !line.empty()) {
            stringstream ss(line);
            string label;
            ss >> label;
            vector<string> dataTokens = split(line);
            if (label.back() == ':' ) {
                ss >> label; // Skip the label
            }
            else if(label.find(":")==string::npos){
                ss>>label;
                if(label==":"){
                    ss>>label;
                }
            }
            if (label == ".asciiz" || label.find(".asciiz")!=string::npos ) {
                string asciiString;
                if(!hasQuotes(ss)){
                    cout<<"ERROR in data segment"<<endl;
                    continue;
                }
                getline(ss, asciiString, '"');
                getline(ss, asciiString, '"');
                string hexString;
                for (size_t i = 0; i < asciiString.length(); i += 4) {
                    string chunk = asciiString.substr(i, 4);
                    string hexChunk;
                    for (char c : chunk) {
                        stringstream ss;
                        ss << hex << static_cast<int>(c);
                        string hexValue = ss.str();
                        if (hexValue.length() == 1)
                            hexValue = "0" + hexValue;
                        hexChunk = hexValue+hexChunk;
                    }
                    data.push_back(hexChunk);
                }
                data.push_back("00");
            }
            else if (label == ".byte" || label.find(".byte")!=string::npos){
                string dataValue;
                while (ss >> dataValue) {
                    vector<string> parsedValues = parseInputString(dataValue);
                    for (const string& val : parsedValues) {
                        if (!val.empty() && (isdigit(val[0]) || val[0] == '+' || val[0] == '-')) {
                            int intValue = stoi(val);
                            string hexStream;
                            hexStream=intToHex(intValue,1);
                            if(hexStream.length()>2){
                                hexStream=hexStream.substr(hexStream.length()-2,2);
                            }
                            data.push_back(hexStream);
                        }
                    }
                }
            }
            else if (label == ".half" || label.find(".half")!=string::npos){
                string dataValue;
                while (ss >> dataValue) {
                    vector<string> parsedValues = parseInputString(dataValue);
                    for (const string& val : parsedValues) {
                        if (!val.empty() && (isdigit(val[0]) || val[0] == '+' || val[0] == '-')) {
                            int intValue = stoi(val);
                            string hexStream;
                            hexStream=intToHex(intValue,2);
                            if(hexStream.length()>4){
                                hexStream=hexStream.substr(hexStream.length()-4,4);
                            }
                            data.push_back(hexStream);
                        }
                    }
                }
            }
            else if (label == ".word" || label.find(".word")!=string::npos){
                string dataValue;
                while (ss >> dataValue) {
                    vector<string> parsedValues = parseInputString(dataValue);
                    for (const string& val : parsedValues) {
                        if (!val.empty() && (isdigit(val[0]) || val[0] == '+' || val[0] == '-')) {
                            int intValue = stoi(val);
                            string hexStream;
                            hexStream=intToHex(intValue,4);
                            data.push_back(hexStream);
                        }
                    }
                }
            }
            else if (label == ".dword" || label.find(".dword")!=string::npos){
                string dataValue;
                while (ss >> dataValue) {
                    vector<string> parsedValues = parseInputString(dataValue);
                    for (const string& val : parsedValues) {
                        if (!val.empty() && (isdigit(val[0]) || val[0] == '+' || val[0] == '-')) {
                            int intValue = stoi(val);
                            string hexStream;
                            hexStream=intToHex(intValue,8);
                            data.push_back(hexStream);
                        }
                    }
                }
            }
            else{
                cout<<"ERROR in data segment"<<endl;
            }
    
        }
    }
    file.close();
    return data;
}

string reverseString(const string& str) {
    string reversedStr = "";
    for (size_t i = 0; i < str.size(); i += 2) {
        string k= str.substr(i,2);
        reversedStr=k+reversedStr;
    }
    return reversedStr;
}

bool check_immediate_range(int immediate) {
        // These instructions typically use a signed immediate, such as -2048 to 2047.
        return (immediate >= -2048 && immediate <= 2047);
}

bool findRepeatedLabels(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return false;
    }

    unordered_set<std::string> labels;
    string line;
    bool repeatedLabels = false;

    while (getline(file, line)) {
        // Skip empty lines and comments
        if (isEmptyLine(line) || line.find("#") == 0) {
            continue;
        }

        // Check if the line contains a label
        size_t colonPos = line.find(":");
        if (colonPos != string::npos) {
            string label = line.substr(0, colonPos);
            if (labels.count(label) > 0) {
                // Found a repeated label
                repeatedLabels = true;
                break;
            } else {
                // Add the label to the set
                labels.insert(label);
            }
        }
    }

    file.close();

    return repeatedLabels;
}

int main() {
    string input_file = "input.asm";
    string output_file = "output.mc";

    ifstream infile(input_file);
    ofstream outfile(output_file);

    if (!infile.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return 1;
    }

    if (!outfile.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return 1;
    }
    string line;
    int address = 0;
    bool gettext = true;
    //handleDataErrors(input_file);
    if (findRepeatedLabels(input_file)){
        cout<<"ERROR!! Label is repeated"<<endl;
    }
    while (getline(infile, line)){
        int lineNumb=address/4 +1;
        stringstream ss(line);
        string check;
        ss >> check;
        if (check == ".data"){
            gettext = false;
            continue;
        }
        if (check == ".text"){
            gettext = true;
            continue;
        }
        if (!gettext){
            continue;
        }
        bool isLabel=skipLabelAndGetInstruction(line);
        if (isLabel==true){
            size_t colonPos = line.find(":");
            if (colonPos != string::npos) {
            // Found ":" in the line, extract the substring from that position
            line = line.substr(colonPos+1);
            }
            isLabel=false;
        }
        size_t hashPos = line.find("#");
        if (hashPos != string::npos){
            line=line.substr(0,hashPos);
        }
        if(isEmptyLine(line)){
            continue;
        }
        // Split the line into tokens
        vector<string> tokens;
        vector<string> token = split(line);
        if (token[0]=="ld" | token[0]=="lb"|token[0]=="lh"|token[0]=="lw"|token[0]=="sd"|token[0]=="sb"|token[0]=="sh"|token[0]=="sw" ){
            string temp=token[2];
            size_t openParen = temp.find("(");
            size_t closeParen = temp.find(")");
            token.erase(token.begin()+2);
            token.push_back(temp.substr(openParen+1,closeParen-openParen-1));
            token.push_back(temp.substr(0,openParen));
        }
        tokens.insert(tokens.end(), token.begin(), token.end());
        // Ignore empty lines
        if (tokens.empty()) {
            continue;
        }

        // Get opcode
        string opcode = tokens[0];
        string opcode_value = opcode_map[opcode];

        // Get funct3
        string funct7,funct3,rd,rs1,rs2,imm,machine_code;
        int offset;
        if (formats[opcode_value] == "R" || formats[opcode_value] == "I" || formats[opcode_value] == "S" || formats[opcode_value] == "SB") {
            funct3 = func3[opcode];
            rs1 = register_map[tokens[2]];
            if (formats[opcode_value] == "R"){
                funct7=func7[opcode];
                rs2=register_map[token[3]];
                rd = register_map[tokens[1]];
                if(rs1=="" || rs2=="" || rd==""){
                    cout<<"ERROR in instruction "<<lineNumb<<endl;
                }
                else{
                machine_code = funct7+rs2+rs1+funct3+rd+opcode_value;}
            }
            else if (formats[opcode_value] == "S"){
                rs2 = register_map[tokens[1]];
                imm=intTo20BitBinaryString(stoi(tokens[3])).substr(9,12);
                if(!check_immediate_range(stoi(tokens[3]))){
                    cout<<line;
                    cout<<" immediate value is out of range"<<endl;
                }
                else if(rs1=="" || rs2==""){
                    cout<<"ERROR in instruction "<<lineNumb<<endl;
                }
                else{
                machine_code = imm.substr(0,7)+rs2+rs1+funct3+imm.substr(7,5)+opcode_value;}
            }
            else if (formats[opcode_value] == "I"){
                rd = register_map[tokens[1]];
                imm=intTo20BitBinaryString(stoi(tokens[3])).substr(9,12);
                if(!check_immediate_range(stoi(tokens[3]))){
                    cout<<line;
                    cout<<" immediate value is out of range"<<endl;
                }
                else if(rs1=="" || rd==""){
                    cout<<"ERROR in instruction "<<lineNumb<<endl;
                }
                else{
                machine_code = imm+rs1+funct3+rd+opcode_value;}
            }
            else if (formats[opcode_value] == "SB"){
                rs2 = register_map[tokens[1]];
                offset=address+4;
                if(register_valmap[tokens[1]]==register_valmap[tokens[2]]){
                    offset=4*processFileUntilLabel("input.asm",tokens[3])-address;
                }
                if(rs1=="" || rs2==""){
                    cout<<"ERROR in instruction "<<lineNumb<<endl;
                }
                else{
                imm=intTo20BitBinaryString(offset);
                machine_code = imm.substr(8,1)+imm.substr(10,6)+rs1+rs2+funct3+imm.substr(16,4)+imm.substr(9,1)+opcode_value;}
            }            
        }
        else if (formats[opcode_value] == "UJ"){
            rd=register_map[tokens[1]];
            offset=4*processFileUntilLabel("input.asm",tokens[2])-address;
            imm=intTo20BitBinaryString(offset);
            if(rd==""){
                cout<<"ERROR in instruction "<<lineNumb<<endl;
            }
            else{
            machine_code=imm.substr(0,1)+imm.substr(10,10)+imm.substr(9,1)+imm.substr(1,8)+rd+opcode_value;}
        }
        else if (formats[opcode_value] == "U"){
            rd=register_map[tokens[1]];
            imm=intTo20BitBinaryString(stoi(tokens[2]));
            if(!(stoi(tokens[2]) >= 0 && stoi(tokens[2]) <= 1048575)){
                cout<<line;
                cout<<" immediate value is out of range"<<endl;
            }
            else if(rd==""){
                cout<<"ERROR in instruction "<<lineNumb<<endl;
            }
            else{
            machine_code=imm.substr(1,20)+rd+opcode_value;}
        }
        else{
            cout<<"ERROR"<<endl;
        }

        // Generate machine code        
        string hex_machine_code=binaryToHex(machine_code);
        
        // Write machine code to output file
        outfile << "0x" << intToHex(address) << " 0x" << hex_machine_code << endl;

        // Increment address by 4 bytes (32-bit token)
        address += 4;
    }
    // Close files
    infile.close();

    vector<string> data = dataSeg("input.asm");
    string datas="";
    for (const auto& hexValue : data) {
        datas+=reverseString(hexValue);
    }
    outfile << endl;

    unsigned int data_address = 0x10000000; // Starting address
    for (size_t i = 0; i < datas.length(); i += 8) {
        outfile << "0x" << uppercase << hex << data_address << " " << "0x" << reverseString(datas.substr(i,8)) << endl;
        data_address += 4; // Increment address by 4 for each word
    }
    outfile.close();

    std::cout << "Assembly code assembled successfully." << endl;

    return 0;
}
