# this will run a short test on all the assigned commands.

finishup:
addiu $t0, $0, 0x3ffc 	# put a base memory location in $t0 (last possible word)
lui $t0, 0x0040
addiu $t1, $0, 0x1000 	# put a base memory location in $t1 (first possible word)
lui $t1, 0x0040
# addiu $t3, $0, 0 	# put a base memory location in $t0 (too low)
addiu $a0, $a0, 1
sw $s0, 0($t0)		# store the contents of $s0 at the top of available memory
lw $s1, 0($t0)		# copy the word back from mem to $s1
sw $s0, 0($t1)		# store the contents of $s0 at the bottom of available memory
lw $s1, 0($t1)		# copy the word back from mem to $s1


sw $s0, 4($t0)		# store the contents of $s0 at above the top of available memory
lw $s1, 1($t0)		# copy the word back from mem to $s1


addi	$0,$0,0 #unsupported instruction, terminate
