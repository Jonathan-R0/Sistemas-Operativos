/*
 * It should restores the register values in the Trapframe with the 'iret' instruction.
 * This exits the kernel and starts executing some environment's code.
 *
 * This function does not return.
 */

.globl context_switch;
context_switch:
	movl 0x8(%ebp), %esp
	popal
	popl %es
	popl %ds
	addl $0x8, %esp
	iret

spin:
	jmp spin
