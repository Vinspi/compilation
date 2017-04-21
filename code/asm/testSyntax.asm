.data
.text
__start:
	jal f_main
	li $v0, 10
	syscall	#exit
ERROR : impossible de trouver la fonction main
