	.file	".\\listing_13_2.c"
	.text
	.globl	i
	.data
	.align 4
i:
	.long	1
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC2:
	.ascii "%u\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -8(%rbp)
	pxor	%xmm0, %xmm0
	ucomisd	-8(%rbp), %xmm0
	jp	.L8
	pxor	%xmm0, %xmm0
	ucomisd	-8(%rbp), %xmm0
	je	.L2
.L8:
	movl	$2, %eax
	jmp	.L4
.L2:
	movl	$3, %eax
.L4:
	movl	%eax, -12(%rbp)
	cmpl	$0, -12(%rbp)
	je	.L5
	movl	i(%rip), %eax
	movl	%eax, %edx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	printf
	jmp	.L6
.L5:
	movl	-12(%rbp), %eax
	movl	%eax, %edx
	leaq	.LC2(%rip), %rax
	movq	%rax, %rcx
	call	printf
.L6:
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC0:
	.long	-323031207
	.long	1072972277
	.ident	"GCC: (GNU) 11.3.0"
	.def	printf;	.scl	2;	.type	32;	.endef
