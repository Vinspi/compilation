.data
	v_$i: .space 4
	v_$carre: .space 4
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
	li $t1, 0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, v_$i 	#sauve la variable
tq0:
	lw $t1 v_$i
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	li $t1, 10
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	lw $t2, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	slt $t0, $t2, $t1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t0, 0($sp)	# copie reg vers sommet de pile
	lw $t0, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	beq $t0 $0 tq1
	lw $t1 v_$i
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1 v_$i
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	lw $t2, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	mult $t1, $t2
	mflo $t1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, v_$carre 	#sauve la variable
	lw $t1 v_$carre
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
	lw $t1 v_$i
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	li $t1, 1
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
	sw $t1, v_$i 	#sauve la variable
	j tq0
tq1:
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
