addu $0, $0, $0
addiu $0, $0, -1024
addiu $0, $0, -1
addiu $0, $0, 0
addiu $0, $0, 1
addiu $0, $0, 1024
subu $0, $0, $0
sll $0, $0, 0
srl $0, $0, 0
sll $0, $0, 0
srl $0, $0, 0
and $0, $0, $0
andi $0, $0, 0
or $0, $0, $0
ori $0, $0, 0
lui $0, 0
slt $0, $0, $0

beq $0, $0, 0
bne $0, $0, 0
j
jal
jr

lw
sw