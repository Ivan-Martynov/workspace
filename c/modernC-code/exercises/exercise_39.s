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
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$64, %rsp
	.seh_stackalloc	64
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	movl	$0, -4(%rbp)
	movq	$0, -16(%rbp)
.L2:
	cmpq	$0, -16(%rbp)
	je	.L3
	movq	-16(%rbp), %rax
	movl	(%rax), %ecx
	movq	-24(%rbp), %rax
	cmpq	-16(%rbp), %rax
	jne	.L4
	leaq	.LC0(%rip), %rax
	jmp	.L5
.L4:
	leaq	.LC1(%rip), %rax
.L5:
	movl	-4(%rbp), %edx
	movl	%ecx, %r9d
	movq	%rax, %r8
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	printf
.L3:
	movq	-16(%rbp), %rax
	movq	%rax, -24(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, -28(%rbp)
	leaq	-28(%rbp), %rax
	movq	%rax, -16(%rbp)
	addl	$1, -4(%rbp)
	movl	-4(%rbp), %eax
	cmpl	16(%rbp), %eax
	ja	.L7
	jmp	.L2
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
