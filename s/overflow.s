# addu
# addiu
# subu

lui $1, 0x8000
addiu $1, $1, -1
add $3, $2, $1

# 32 bit register values