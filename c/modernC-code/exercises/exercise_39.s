	.file	".\\exercise_39.c"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "equal\0"
.LC1:
	.ascii "unequal\0"
.LC2:
	.ascii "%u: p and q are %s, *p is %u\12\0"
	.text
	.globl	fgoto
	.def	fgoto;	.scl	2;	.type	32;	.endef
	.seh_proc	fgoto
fgoto:
	pushq	%rbp				# save base pointer
	.seh_pushreg	%rbp
	movq	%rsp, %rbp			# Load stack pointer
	.seh_setframe	%rbp, 0
	subq	$64, %rsp			# Adjust stack pointer
	.seh_stackalloc	64
	.seh_endprologue
	movl	%ecx, 16(%rbp)		# fgoto#0 => n
	movl	$0, -4(%rbp)		# init j
	movq	$0, -16(%rbp)		# init p
.L2:
	cmpq	$0, -16(%rbp) 		# if (p)
	je	.L3
	movq	-16(%rbp), %rax		# p => rax
	movl	(%rax), %ecx		# *p => edx
	movq	-24(%rbp), %rax		# ( == q)?
	cmpq	-16(%rbp), %rax		# (p == )?
	jne	.L4
	leaq	.LC0(%rip), %rax 	# "equal"
	jmp	.L5
.L4:
	leaq	.LC1(%rip), %rax 	# "unequal"
.L5:
	movl	-4(%rbp), %edx		# j => printf#1
	movl	%ecx, %r9d			# *p => printf#3
	movq	%rax, %r8			# eq/ne => printf#2
	leaq	.LC2(%rip), %rax	# format => printf#0
	movq	%rax, %rcx
	call	printf
.L3:
	movq	-16(%rbp), %rax		# p ==|
	movq	%rax, -24(%rbp)		# 		==> q
	movl	-4(%rbp), %eax		# j ==|
	movl	%eax, -28(%rbp) 	#		==> cmp_lit
	leaq	-28(%rbp), %rax		# &cmp_lit ==|
	movq	%rax, -16(%rbp)		# 			  ==>p
	addl	$1, -4(%rbp)		# ++j
	movl	-4(%rbp), %eax		# if (j
	cmpl	16(%rbp), %eax		#		<= n)
	ja	.L7
	jmp	.L2						# goto AGAIN
.L7:
	nop
	addq	$64, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	call	__main
	movl	$2, %ecx
	call	fgoto
	movl	$0, %eax
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.ident	"GCC: (GNU) 11.3.0"
	.def	printf;	.scl	2;	.type	32;	.endef
