	.file	"prediccion.c"
	.text
	.p2align 4
	.type	cambia_estado_saturacion.part.0, @function
cambia_estado_saturacion.part.0:
.LFB59:
	.cfi_startproc
	movl	%esi, %ecx
	movl	$1, %eax
	movl	(%rdi), %edx
	sall	%cl, %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	jge	.L1
	addl	$1, %edx
	movl	%edx, (%rdi)
.L1:
	ret
	.cfi_endproc
.LFE59:
	.size	cambia_estado_saturacion.part.0, .-cambia_estado_saturacion.part.0
	.p2align 4
	.type	cambia_estado_histeresis, @function
cambia_estado_histeresis:
.LFB55:
	.cfi_startproc
	movl	$1, %eax
	movl	%edi, %r9d
	leal	-1(%rdx), %ecx
	movl	%eax, %r8d
	sall	%cl, %r8d
	testl	%r9d, %r9d
	movl	(%rsi), %edi
	je	.L5
	movl	%edx, %ecx
	subl	$1, %r8d
	sall	%cl, %eax
	subl	$1, %eax
	cmpl	%r8d, %edi
	je	.L9
	cmpl	%eax, %edi
	jge	.L4
	addl	$1, %edi
	movl	%edi, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L5:
	cmpl	%r8d, %edi
	je	.L10
	testl	%edi, %edi
	.p2align 4,,2
	jle	.L4
	subl	$1, %edi
	movl	%edi, (%rsi)
.L4:
	.p2align 4,,1
	ret
	.p2align 4,,7
	.p2align 3
.L10:
	movl	$0, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L9:
	movl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE55:
	.size	cambia_estado_histeresis, .-cambia_estado_histeresis
	.p2align 4
	.globl	inicializa_prediccion
	.type	inicializa_prediccion, @function
inicializa_prediccion:
.LFB56:
	.cfi_startproc
	endbr64
	movq	branch_mask_global@GOTPCREL(%rip), %rax
	pxor	%xmm0, %xmm0
	movl	$0, (%rax)
	movq	branch_mask_local@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	movq	Global@GOTPCREL(%rip), %rax
	movdqu	%xmm0, (%rax)
	movdqu	%xmm0, 16(%rax)
	movdqu	%xmm0, 32(%rax)
	movdqu	%xmm0, 48(%rax)
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	testl	%edx, %edx
	jle	.L11
	movq	BTB@GOTPCREL(%rip), %rcx
	leal	-1(%rdx), %edx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$5, %rdx
	leaq	24(%rcx), %rax
	leaq	120(%rcx,%rdx), %rdx
	.p2align 4
	.p2align 3
.L13:
	pxor	%xmm0, %xmm0
	addq	$96, %rax
	movq	$-1, -120(%rax)
	movl	$0, -112(%rax)
	movdqu	%xmm0, -96(%rax)
	movdqu	%xmm0, -80(%rax)
	movdqu	%xmm0, -64(%rax)
	movq	$0, -104(%rax)
	movl	$0, -32(%rax)
	movl	$0, -28(%rax)
	movdqu	%xmm0, -48(%rax)
	cmpq	%rdx, %rax
	jne	.L13
.L11:
	ret
	.cfi_endproc
.LFE56:
	.size	inicializa_prediccion, .-inicializa_prediccion
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%u %ld %u %ld\n"
.LC1:
	.string	"prediccion.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Error: %s; funci\303\263n:%s; l\303\255nea:%d\n"
	.section	.rodata.str1.1
.LC3:
	.string	"Final de la traza de saltos\n"
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"ERROR (%s:%d): Predictor no implementado\n"
	.text
	.p2align 4
	.globl	obtener_prediccion
	.type	obtener_prediccion, @function
obtener_prediccion:
.LFB57:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-40(%rsp), %rsp
	.cfi_def_cfa_offset 96
	movq	branch_mask_global@GOTPCREL(%rip), %rax
	movq	$0, (%rcx)
	movq	tipo_predictor@GOTPCREL(%rip), %r15
	movl	(%rax), %eax
	movq	$0, 8(%r8)
	movl	%eax, (%r8)
	movq	branch_mask_local@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, 4(%r8)
	movl	(%r15), %eax
	cmpl	$10, %eax
	je	.L101
	movq	%rdx, %r13
	leal	-8(%rax), %edx
	cmpl	$1, %edx
	movq	%rdi, %rbx
	movl	%esi, %ebp
	movq	%rcx, %r14
	jbe	.L107
.L18:
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rsi
	movl	(%rsi), %edi
	testl	%edi, %edi
	jle	.L101
	movq	BTB@GOTPCREL(%rip), %rax
	xorl	%r12d, %r12d
	xorl	%edx, %edx
	leaq	.L52(%rip), %rdi
	movl	$1, %r11d
	movq	%r8, %r10
	jmp	.L64
	.p2align 4,,7
	.p2align 3
.L49:
	addl	$1, %edx
	addq	$96, %rax
	cmpl	%edx, (%rsi)
	jle	.L15
.L64:
	cmpq	%rbx, (%rax)
	jne	.L49
	movq	estat@GOTPCREL(%rip), %rcx
	movl	(%r15), %r12d
	movl	%ebp, 92(%rax)
	addq	$1, 32(%rcx)
	cmpl	$7, %r12d
	ja	.L50
	movl	%r12d, %ecx
	movslq	(%rdi,%rcx,4), %rcx
	addq	%rdi, %rcx
	notrack jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L52:
	.long	.L56-.L52
	.long	.L55-.L52
	.long	.L55-.L52
	.long	.L54-.L52
	.long	.L54-.L52
	.long	.L53-.L52
	.long	.L51-.L52
	.long	.L51-.L52
	.text
	.p2align 4,,7
	.p2align 3
.L54:
	movq	branch_mask_local@GOTPCREL(%rip), %rcx
	movl	%r11d, %r9d
	movslq	(%rcx), %r8
	movslq	%edx, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%r8,%rcx,8), %r8
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r9d
	movl	%r9d, %ecx
	movq	BTB@GOTPCREL(%rip), %r9
	cmpl	%ecx, 8(%r9,%r8,4)
	jl	.L95
.L100:
	movl	$1, 0(%r13)
	addl	$1, %edx
	movq	16(%rax), %rcx
	addq	$96, %rax
	cmpl	%edx, (%rsi)
	movl	$1, %r12d
	movq	%rcx, (%r14)
	jg	.L64
.L15:
	leaq	40(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%r12d, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L55:
	.cfi_restore_state
	movq	branch_mask_global@GOTPCREL(%rip), %rcx
	movl	%r11d, %r9d
	movslq	(%rcx), %r8
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r9d
	movl	%r9d, %ecx
	movq	Global@GOTPCREL(%rip), %r9
	cmpl	%ecx, (%r9,%r8,4)
	jge	.L100
.L95:
	movl	$0, 0(%r13)
	movl	$1, %r12d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L51:
	movl	8(%rax), %ecx
	subl	$2, %ecx
	cmpl	$1, %ecx
	jbe	.L100
	movl	$0, 0(%r13)
	movl	$1, %r12d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L56:
	movq	branch_mask_local@GOTPCREL(%rip), %rcx
	movl	$0, 8(%rsp)
	movslq	(%rcx), %r8
	movslq	%edx, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%r8,%rcx,8), %r9
	movl	%r11d, %r8d
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r8d
	movl	%r8d, %ecx
	movq	BTB@GOTPCREL(%rip), %r8
	cmpl	%ecx, 8(%r8,%r9,4)
	jl	.L61
	movq	16(%rax), %rcx
	movl	$1, %r12d
	movl	$1, 8(%rsp)
	movq	%rcx, 24(%rsp)
.L61:
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rcx
	movl	%r11d, %r9d
	movl	%r11d, %r8d
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r9d
	movq	branch_mask_global@GOTPCREL(%rip), %rcx
	movl	%r9d, 20(%rsp)
	movslq	(%rcx), %r9
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r8d
	movl	%r8d, %ecx
	movq	Global@GOTPCREL(%rip), %r8
	cmpl	%ecx, (%r8,%r9,4)
	jl	.L62
	movl	8(%rsp), %r9d
	movq	16(%rax), %rcx
	movl	$1, 8(%r10)
	movl	%r9d, 12(%r10)
	movl	20(%rsp), %r9d
	cmpl	88(%rax), %r9d
	jle	.L65
	movl	$1, 0(%r13)
	movl	$1, %r12d
	movq	%rcx, (%r14)
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L53:
	cmpl	$3, 8(%rax)
	je	.L100
	movl	$0, 0(%r13)
	movl	$1, %r12d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L62:
	movl	8(%rsp), %ecx
	movl	$0, 8(%r10)
	movl	%ecx, 12(%r10)
	movl	20(%rsp), %ecx
	cmpl	88(%rax), %ecx
	jg	.L95
.L65:
	movl	%r12d, 0(%r13)
	testl	%r12d, %r12d
	je	.L68
	movq	24(%rsp), %rcx
	movq	%rcx, (%r14)
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L107:
	movq	fase_ejecucion@GOTPCREL(%rip), %rdx
	movl	(%rdx), %r12d
	testl	%r12d, %r12d
	je	.L15
	cmpl	$1, %r12d
	jne	.L18
	movq	ciclo1@GOTPCREL(%rip), %rsi
	movq	Ciclo@GOTPCREL(%rip), %rdi
	movl	(%rsi), %esi
	cmpl	%esi, (%rdi)
	je	.L108
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	testl	%ecx, %ecx
	jne	.L109
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$1, (%r10)
	je	.L92
	movq	PC1@GOTPCREL(%rip), %rcx
.L31:
	cmpq	%rbx, (%rcx)
	jne	.L32
	movq	$-1, (%rcx)
	cmpl	$8, (%r15)
	je	.L33
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L35
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L39
	.p2align 4,,7
	.p2align 3
.L110:
	leaq	96(%rax), %rax
.L39:
	cmpq	%rbx, (%rdx)
	jne	.L38
	movl	%ebp, 92(%rdx)
	movl	$1, %esi
.L38:
	cmpq	%rax, %rcx
	movq	%rax, %rdx
	jne	.L110
	testl	%esi, %esi
	je	.L35
.L33:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 32(%rax)
	movq	cond1@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	testl	%edx, %edx
	jne	.L36
	movl	$0, 0(%r13)
.L37:
	movl	$1, (%r10)
	jmp	.L15
.L32:
	movq	IF_ISS_1@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	leal	-100(%rax), %edx
	cmpl	$5, %edx
	ja	.L111
.L42:
	cmpl	$8, (%r15)
	je	.L97
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L43
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L112:
	leaq	96(%rax), %rax
.L48:
	cmpq	%rbx, (%rdx)
	jne	.L47
	movl	%ebp, 92(%rdx)
	movl	$1, %esi
.L47:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L112
	testl	%esi, %esi
	je	.L43
.L97:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 32(%rax)
.L43:
	movl	$0, (%r10)
	.p2align 4
	.p2align 3
.L101:
	xorl	%r12d, %r12d
	jmp	.L15
	.p2align 4,,7
	.p2align 3
.L68:
	movl	$1, %r12d
	jmp	.L49
.L92:
	movq	f_trace@GOTPCREL(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	PC1@GOTPCREL(%rip), %rcx
	movq	dest1@GOTPCREL(%rip), %r9
	movq	cond1@GOTPCREL(%rip), %r8
	movq	orden1@GOTPCREL(%rip), %rdx
	movq	(%rax), %rdi
	xorl	%eax, %eax
	movq	%rcx, 8(%rsp)
	call	__isoc99_fscanf@PLT
	movq	orden1@GOTPCREL(%rip), %rax
	movq	8(%rsp), %rcx
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$-2, (%rax)
	jne	.L31
	movq	stderr@GOTPCREL(%rip), %rbx
	movl	$269, %r9d
	leaq	__func__.4209(%rip), %r8
	leaq	.LC1(%rip), %rcx
	leaq	.LC2(%rip), %rdx
	movl	$1, %esi
	movq	(%rbx), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movq	(%rbx), %rcx
	leaq	.LC3(%rip), %rdi
	movl	$28, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L109:
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L101
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L30
	.p2align 4,,7
	.p2align 3
.L113:
	leaq	96(%rax), %rax
.L30:
	cmpq	%rbx, (%rdx)
	jne	.L29
	movl	%ebp, 92(%rdx)
	movl	$1, %esi
.L29:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L113
.L91:
	testl	%esi, %esi
	je	.L101
.L103:
	movq	estat@GOTPCREL(%rip), %rax
	xorl	%r12d, %r12d
	addq	$1, 32(%rax)
	jmp	.L15
.L108:
	movq	IF_ISS_1@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edx
	leal	-100(%rdx), %ecx
	cmpl	$5, %ecx
	ja	.L114
.L23:
	cmpl	$8, %eax
	je	.L103
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L101
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L27
.L115:
	leaq	96(%rax), %rax
.L27:
	cmpq	%rbx, (%rdx)
	jne	.L26
	movl	%ebp, 92(%rdx)
	movl	$1, %esi
.L26:
	cmpq	%rax, %rcx
	movq	%rax, %rdx
	jne	.L115
	jmp	.L91
.L35:
	movq	cond1@GOTPCREL(%rip), %rax
	movl	$0, 0(%r13)
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L41
	movq	orden_clarividente@GOTPCREL(%rip), %rax
	movl	%ebp, (%rax)
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	$1, (%rax)
.L41:
	movl	$1, (%r10)
	xorl	%r12d, %r12d
	jmp	.L15
.L111:
	subl	$108, %eax
	cmpl	$3, %eax
	ja	.L43
	jmp	.L42
.L36:
	movq	dest1@GOTPCREL(%rip), %rax
	movl	$1, 0(%r13)
	movq	ciclo1@GOTPCREL(%rip), %rdi
	movq	(%rax), %rax
	movq	%rax, (%r14)
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, (%rdi)
	jmp	.L37
.L114:
	subl	$108, %edx
	cmpl	$3, %edx
	ja	.L101
	jmp	.L23
.L50:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$478, %r8d
	leaq	.LC1(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE57:
	.size	obtener_prediccion, .-obtener_prediccion
	.section	.rodata.str1.1
.LC5:
	.string	"%u %ld %d %ld\n"
	.text
	.p2align 4
	.globl	actualizar_prediccion
	.type	actualizar_prediccion, @function
actualizar_prediccion:
.LFB58:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	%ecx, %r15d
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%r8, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-88(%rsp), %rsp
	.cfi_def_cfa_offset 144
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	%edx, 56(%rsp)
	movl	%edi, 52(%rsp)
	movl	(%rax), %edx
	leal	-8(%rdx), %eax
	cmpl	$1, %eax
	jbe	.L195
.L117:
	movl	%edx, %eax
	andl	$-3, %eax
	cmpl	$8, %eax
	je	.L116
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %r14
	movl	(%r14), %eax
	testl	%eax, %eax
	jle	.L168
	cmpl	$1, %r15d
	movl	$-1, %ebx
	movslq	52(%rsp), %rdx
	sbbl	%eax, %eax
	movq	%r13, 24(%rsp)
	addl	$3, %eax
	movl	%eax, 60(%rsp)
	cmpl	$1, %r15d
	sbbl	%eax, %eax
	leaq	(%rdx,%rdx,2), %rdx
	andl	$-2, %eax
	addl	$3, %eax
	cmpl	$1, %r15d
	movl	%eax, 64(%rsp)
	sbbl	%eax, %eax
	notl	%eax
	xorl	%r11d, %r11d
	andl	$2, %eax
	xorl	%r8d, %r8d
	movl	%eax, 68(%rsp)
	movq	BTB@GOTPCREL(%rip), %rax
	salq	$5, %rdx
	movq	%rdx, 32(%rsp)
	movq	%rax, 40(%rsp)
	leaq	88(%rax), %r10
	movl	$-1, %eax
	movl	%eax, %r13d
	jmp	.L159
	.p2align 4,,7
	.p2align 3
.L121:
	movl	4(%r10), %edx
	cmpl	%ebx, %edx
	jnb	.L158
	movl	%ebp, %r13d
	movl	%edx, %ebx
.L158:
	addq	$1, %r11
	addq	$96, %r10
	cmpl	%r11d, (%r14)
	jle	.L196
.L159:
	cmpq	%r12, -88(%r10)
	movl	%r11d, %ebp
	jne	.L121
	movq	24(%rsp), %rax
	movq	%rax, -72(%r10)
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	cmpl	$9, %r8d
	ja	.L122
	leaq	.L124(%rip), %rsi
	movl	%r8d, %ecx
	movslq	(%rsi,%rcx,4), %rdx
	addq	%rsi, %rdx
	notrack jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L124:
	.long	.L131-.L124
	.long	.L130-.L124
	.long	.L129-.L124
	.long	.L128-.L124
	.long	.L127-.L124
	.long	.L123-.L124
	.long	.L126-.L124
	.long	.L125-.L124
	.long	.L122-.L124
	.long	.L123-.L124
	.text
.L161:
	cmpl	$9, %edx
	jne	.L116
.L162:
	movq	40(%rsp), %rdi
	leaq	(%rax,%rax,2), %rax
	salq	$5, %rax
	testl	%r15d, %r15d
	je	.L164
	movl	$3, 8(%rdi,%rax)
.L116:
	leaq	88(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L196:
	.cfi_restore_state
	testl	%r8d, %r8d
	movslq	%r13d, %rax
	movq	24(%rsp), %r13
	jne	.L116
	movq	tipo_predictor@GOTPCREL(%rip), %rdi
	movl	(%rdi), %edx
.L120:
	movl	56(%rsp), %edi
	leaq	(%rax,%rax,2), %rsi
	movq	%rsi, %rcx
	salq	$5, %rcx
	addq	40(%rsp), %rcx
	cmpl	$7, %edx
	movq	%r12, (%rcx)
	movl	%edi, 92(%rcx)
	movq	%r13, 16(%rcx)
	ja	.L161
	cmpl	$4, %edx
	ja	.L162
	movslq	52(%rsp), %rdx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$5, %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	52(%rdx), %rcx
	movslq	48(%rdx), %rdx
	jne	.L197
	movq	40(%rsp), %rdi
	leaq	4(%rcx,%rsi,8), %rcx
	movl	$0, 8(%rdi,%rcx,4)
	movq	Global@GOTPCREL(%rip), %rcx
	movl	$0, (%rcx,%rdx,4)
.L166:
	movq	40(%rsp), %rdi
	leaq	(%rax,%rax,2), %rdx
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rax
	salq	$5, %rdx
	movl	(%rax), %ecx
	movl	$1, %eax
	sall	%cl, %eax
	subl	$1, %eax
	movl	%eax, 88(%rdi,%rdx)
	leaq	88(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L123:
	.cfi_restore_state
	testl	%r15d, %r15d
	je	.L137
.L135:
	movl	$3, -80(%r10)
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L127:
	movq	32(%rsp), %rdx
	leaq	(%r11,%r11,2), %rcx
	movq	40(%rsp), %rax
	movl	%r15d, %edi
	addq	RB@GOTPCREL(%rip), %rdx
	movslq	52(%rdx), %rdx
	leaq	6(%rdx,%rcx,8), %rdx
	leaq	(%rax,%rdx,4), %rsi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edx
	call	cambia_estado_histeresis
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L128:
	movq	32(%rsp), %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	52(%rdx), %rdx
	je	.L144
	movq	40(%rsp), %rax
	leaq	(%r11,%r11,2), %rcx
	leaq	6(%rdx,%rcx,8), %rdx
	leaq	(%rax,%rdx,4), %rdi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L129:
	movq	32(%rsp), %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	movslq	48(%rdx), %rcx
	movq	Global@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rcx,4), %rsi
.L192:
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movl	%r15d, %edi
	movl	(%rdx), %edx
	call	cambia_estado_histeresis
.L194:
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L130:
	movq	32(%rsp), %rdx
	movq	Global@GOTPCREL(%rip), %rcx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	48(%rdx), %rdx
	je	.L146
	leaq	(%rcx,%rdx,4), %rdi
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L131:
	movq	RB@GOTPCREL(%rip), %r8
	movq	32(%rsp), %rax
	leaq	(%r8,%rax), %rdx
	movl	56(%rdx), %ecx
	movl	60(%rdx), %edx
	cmpl	%edx, %r15d
	jne	.L186
	cmpl	%ecx, %r15d
	jne	.L198
	cmpl	%edx, %r15d
	je	.L149
.L186:
	cmpl	%ecx, %r15d
	.p2align 4,,2
	jne	.L149
	movl	(%r10), %edx
	testl	%edx, %edx
	jle	.L149
	subl	$1, %edx
	movl	%edx, (%r10)
.L149:
	movq	32(%rsp), %rax
	leaq	(%r11,%r11,2), %rsi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	leaq	(%r8,%rax), %rcx
	movl	(%rdx), %r9d
	movslq	52(%rcx), %rdx
	movq	40(%rsp), %rax
	movq	%rcx, 72(%rsp)
	leaq	6(%rdx,%rsi,8), %rsi
	leaq	(%rax,%rsi,4), %rdi
	movq	subtipo_pred_local@GOTPCREL(%rip), %rsi
	cmpb	$104, (%rsi)
	je	.L199
	testl	%r15d, %r15d
	jne	.L200
	movq	40(%rsp), %rax
	movslq	%ebp, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%rdx,%rcx,8), %rcx
	movl	8(%rax,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L156
	subl	$1, %edx
	movl	%edx, 8(%rax,%rcx,4)
.L156:
	movq	subtipo_pred_global@GOTPCREL(%rip), %rcx
	movq	32(%rsp), %rax
	cmpb	$104, (%rcx)
	movslq	48(%r8,%rax), %rdx
	je	.L155
.L157:
	movq	Global@GOTPCREL(%rip), %rsi
	movl	(%rsi,%rdx,4), %ecx
	testl	%ecx, %ecx
	jle	.L194
	subl	$1, %ecx
	movl	%ecx, (%rsi,%rdx,4)
	jmp	.L194
	.p2align 4,,7
	.p2align 3
.L125:
	movl	-80(%r10), %edx
	cmpl	$2, %edx
	je	.L140
	ja	.L141
	testl	%edx, %edx
	.p2align 4,,2
	je	.L201
	movl	68(%rsp), %eax
	movl	%eax, -80(%r10)
	jmp	.L194
	.p2align 4,,7
	.p2align 3
.L126:
	movl	-80(%r10), %r8d
	testl	%r15d, %r15d
	je	.L133
	cmpl	$1, %r8d
	je	.L134
	cmpl	$2, %r8d
	je	.L135
	testl	%r8d, %r8d
	jne	.L194
	movl	$1, -80(%r10)
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L141:
	cmpl	$3, %edx
	movl	$1, %r8d
	jne	.L121
	movl	60(%rsp), %eax
	movl	%eax, -80(%r10)
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L195:
	movq	fase_ejecucion@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	testl	%ecx, %ecx
	jne	.L117
	movq	f_trace@GOTPCREL(%rip), %rax
	leaq	.LC5(%rip), %rdx
	movq	%r8, (%rsp)
	movl	%r15d, %r9d
	movl	56(%rsp), %ecx
	movq	%rsi, %r8
	movq	(%rax), %rdi
	movl	$1, %esi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	jmp	.L117
	.p2align 4,,7
	.p2align 3
.L133:
	cmpl	$2, %r8d
	jne	.L202
	.p2align 4
	.p2align 3
.L137:
	movl	$0, -80(%r10)
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L144:
	movq	40(%rsp), %rax
	movslq	%r11d, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%rdx,%rcx,8), %rcx
	movl	8(%rax,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L194
	movq	40(%rsp), %rax
	subl	$1, %edx
	movl	$1, %r8d
	movl	%edx, 8(%rax,%rcx,4)
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L146:
	movl	(%rcx,%rdx,4), %esi
	testl	%esi, %esi
	jle	.L194
	subl	$1, %esi
	movl	%esi, (%rcx,%rdx,4)
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L202:
	cmpl	$3, %r8d
	je	.L138
	cmpl	$1, %r8d
	jne	.L194
	movl	$0, -80(%r10)
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L200:
	movl	%r9d, %esi
	call	cambia_estado_saturacion.part.0
	movq	subtipo_pred_global@GOTPCREL(%rip), %rdx
	movq	72(%rsp), %rcx
	cmpb	$104, (%rdx)
	je	.L151
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movslq	48(%rcx), %rcx
	movl	(%rdx), %esi
	movq	Global@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rcx,4), %rdi
.L167:
	call	cambia_estado_saturacion.part.0
	movl	$1, %r8d
	jmp	.L121
	.p2align 4,,7
	.p2align 3
.L198:
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rdx
	movq	%r10, %rdi
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	jmp	.L149
.L197:
	leaq	4(%rcx,%rsi,8), %rdi
	movl	$1, %esi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	%esi, %ebx
	movl	(%rcx), %ecx
	sall	%cl, %ebx
	movl	%ebx, %ecx
	movq	40(%rsp), %rbx
	subl	$1, %ecx
	movl	%ecx, 8(%rbx,%rdi,4)
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	sall	%cl, %esi
	leal	-1(%rsi), %ecx
	movq	Global@GOTPCREL(%rip), %rsi
	movl	%ecx, (%rsi,%rdx,4)
	jmp	.L166
.L151:
	movslq	48(%rcx), %rdx
.L155:
	movq	Global@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rdx,4), %rsi
	jmp	.L192
.L199:
	movl	%r9d, %edx
	movq	%rdi, %rsi
	movl	%r15d, %edi
	call	cambia_estado_histeresis
	movq	subtipo_pred_global@GOTPCREL(%rip), %rdx
	movq	72(%rsp), %rcx
	cmpb	$104, (%rdx)
	je	.L151
	movslq	48(%rcx), %rdx
	testl	%r15d, %r15d
	je	.L157
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %esi
	movq	Global@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rdx,4), %rdi
	jmp	.L167
	.p2align 4,,7
	.p2align 3
.L201:
	xorl	%edx, %edx
	testl	%r15d, %r15d
	setne	%dl
	movl	$1, %r8d
	movl	%edx, -80(%r10)
	jmp	.L121
.L140:
	movl	64(%rsp), %eax
	movl	$1, %r8d
	movl	%eax, -80(%r10)
	jmp	.L121
.L164:
	movl	$0, 8(%rdi,%rax)
	jmp	.L116
	.p2align 4,,7
	.p2align 3
.L122:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$694, %r8d
	leaq	.LC1(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L138:
	movl	$2, -80(%r10)
	movl	$1, %r8d
	jmp	.L121
.L134:
	movl	$3, -80(%r10)
	jmp	.L121
.L168:
	movq	BTB@GOTPCREL(%rip), %rdi
	movq	$-1, %rax
	movq	%rdi, 40(%rsp)
	jmp	.L120
	.cfi_endproc
.LFE58:
	.size	actualizar_prediccion, .-actualizar_prediccion
	.section	.rodata
	.align 16
	.type	__func__.4209, @object
	.size	__func__.4209, 19
__func__.4209:
	.string	"obtener_prediccion"
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
