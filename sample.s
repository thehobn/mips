# Short test case for your project.
#
# Note that this is by no means a comprehensive test!
#

		.text
00		addiu	$a0,$0,3
04		addiu	$a1,$0,2
08		jal	Mystery
0c		addi	$0,$0,0 #unsupported instruction, terminate


Mystery:
10		addiu	$v0,$0,0
Loop:
14		beq	$a0,$0,Done
18		addu	$v0,$v0,$a1
1c		addiu	$a0,$a0,-1
20		j Loop
Done:	
24		jr		$ra
