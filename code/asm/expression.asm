.data
	v_$a: .space 4
	v_$b: .space 4
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
	li $v0, 5
	syscall
	move $t1, $v0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, v_$a 	#sauve la variable
	li $v0, 5
	syscall
	move $t1, $v0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, v_$b 	#sauve la variable
	lw $t1 v_$a
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1 v_$b
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	lw $t2, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	add $t1, $t1, $t2
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	li $v0, 1
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0,'\n'
	syscall
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
