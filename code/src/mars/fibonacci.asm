.data
.text
__start:
	jal f_main
	li $v0, 10
	syscall	#exit
f_fibonacci:
	subi	$sp, $sp, 4	#empile registre
	sw	$fp, 0($sp)
	move	$fp, $sp	#nouvelle valeur de $fp
	subi	$sp, $sp, 4	# empile registre
	sw	$ra, 0($sp)
	subi $sp $sp 16	#on alloue les variables locales
	li $t1, 1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, -8($fp) 	#sauve la variable
	li $t1, 0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, -12($fp) 	#sauve la variable
	li $t1, 1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, -16($fp) 	#sauve la variable
	lw $t1, 4($fp) 	#sauve la variable
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	li $t1, 0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	lw $t2, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	seq $t0, $t2, $t1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t0, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	beq $t1, $0, si0
	li $t1, 0
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
	li $t1, 0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, 8($fp)
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
si0:
	lw $t1, 4($fp) 	#sauve la variable
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	li $t1, 1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	lw $t2, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	seq $t0, $t2, $t1
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t0, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	beq $t1, $0, si2
	li $t1, 1
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
	li $t1, 0
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, 8($fp)
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
si2:
tq0:
	lw $t1 -8($fp)
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 4($fp) 	#sauve la variable
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
	lw $t1 -8($fp)
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
	sw $t1, -8($fp) 	#sauve la variable
	lw $t1 -12($fp)
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1 -16($fp)
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
	sw $t1, -20($fp) 	#sauve la variable
	lw $t1 -16($fp)
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, -12($fp) 	#sauve la variable
	lw $t1 -20($fp)
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	lw $t1, 0($sp)	# copie sommet de pile vers reg
	addu $sp, $sp, 4	# desalloue un mot sur la pile
	sw $t1, -16($fp) 	#sauve la variable
	lw $t1 -20($fp)
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
	j tq0
tq1:
	addi $sp $sp 16	#on désalloue les variables locales
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
f_main:
	subi	$sp, $sp, 4	#empile registre
	sw	$fp, 0($sp)
	move	$fp, $sp	#nouvelle valeur de $fp
	subi	$sp, $sp, 4	# empile registre
	sw	$ra, 0($sp)
	subi	$sp, $sp, 4	# allocation valeur de retour
	li $t1, 42
	subu $sp, $sp, 4	# alloue un mot sur la pile
	sw $t1, 0($sp)	# copie reg vers sommet de pile
	jal f_fibonacci
	addi $sp $sp 4
	addi $sp $sp 4	#ignore valeur de retour
	lw	$ra, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	lw	$fp, 0($sp)	# depile vers registre
	addi	$sp, $sp, 4
	jr	$ra
