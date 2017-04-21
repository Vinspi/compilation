.data
	v_$a: .space 4
.text
__start:
	jal f_main
	li $v0, 10
	syscall	#exit
f_main:
	subi	$sp, $sp, 4	#empile registre
	sw	$fp, 0($sp)
	move	$fp, $sp	#nouvelle valeur de $fp
	subi	$sp, $sp, 4	# empile registre
	sw	$ra, 0($sp)
	li $t1, 123
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, v_$a 	#sauve la variable
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
