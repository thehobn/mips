lui $t0, 0x0040
addiu $t0, $t0, 0x3ffc 	# put a base memory location in $t0 (last possible word)
sw $s1, 4($t0)		# copy a word from invalid location to $s1
