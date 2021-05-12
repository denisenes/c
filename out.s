	.text
.LC0:
	.string	"%d\n"
printint:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	ret

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	.comm	a,8,8
	.comm	b,8,8
	.comm	c,8,8
	movq	$150, %r8
	movq	%r8, a(%rip)
	movq	$200, %r8
	movq	%r8, b(%rip)
	movq	$300, %r8
	movq	%r8, c(%rip)
	movq	a(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	b(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	c(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	a(%rip), %r8
	movq	a(%rip), %r9
	addq	%r9, %r8
	movq	%r8, a(%rip)
	movq	a(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	a(%rip), %r8
	movq	b(%rip), %r9
	movq	c(%rip), %r10
	imulq	%r10, %r9
	addq	%r9, %r8
	movq	%r8, b(%rip)
	movq	b(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movq	$10, %r8
	movq	%r8, c(%rip)
	movq	$10, %r8
	movq	%r8, %rdi
	call	printint
	movq	c(%rip), %r8
	movq	%r8, %rdi
	call	printint
	.comm	vvv,8,8
	movq	$666, %r8
	movq	%r8, vvv(%rip)
	movq	vvv(%rip), %r8
	movq	%r8, %rdi
	call	printint
	movl	$0, %eax
	popq	%rbp
	ret
