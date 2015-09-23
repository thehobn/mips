lui $t1, 0x0040
addiu $t1, $t1, 0x00001000 	# put a base memory location in $t0 (first possible word)

addiu $t2, $0, 99	# put 0 in $t0 (0x63)

sw $t2, 4($t1)		# store the contents of $t2 at the indicated mem location (0x63)
lw $t3, 4($t1)		# copy the word back from mem (0x63)
lw $t4, 0($t1)		# copy an empty word back from mem (0x63)

addi	$0,$0,0 #unsupported instruction, terminate
