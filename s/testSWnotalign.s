lui $t1, 0x0040
addiu $t1, $t1, 0x1000 	# put a base memory location in $t1 (first possible word)
sw $s1, 1($t1)		# copy a word from invalid location to $s1

