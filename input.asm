#this i a sample input
.data
 name:.asciiz "Harry"
 array: .word -21,-14,-4,34,56,67,78,83
 size: .byte 8
 searchfor: .half -4    
.text
lw x20,0(x3) #commnet
lw x11,4(x3) #  commnet
lw x12,8(x3) 
lw x10,12(x3)
# this is a comment  
sw x11,0(x20)
sw x12,4(x20)
addi x20,x20,8
jal x1,fibonacci
fibonacci: # this is a comment  
           addi x5,x10,-2
           beq x5,x0,exit
           add x13,x12,x11
           sw x13,0(x20)
           addi x20,x20,4
           addi x11,x12,0
           addi x12,x13,0
           addi x5,x5,-1
           jal x0,fibonacci
exit:
