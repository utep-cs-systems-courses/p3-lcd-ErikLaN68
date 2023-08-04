	.arch msp430g2553
	.p2align 1,0
	
	.data
	.global state_switch
	.align 2
jt:				;jump table
	.word default
	.word case1
	.word case2
	.word case3

	.text

	.extern state_1	;methods used
	.extern state_2
	.extern state_3
	
state_switch:
	cmp #4, r12		;check if 0-3
	jc default
	add r12, r12		;2*s
	mov jt(r12),r0		;goto jt[s]

case1:
	call #state_1
	jmp return

case2:
	call #state_2
	jmp return

case3:
	call #state_3
	jmp return

default:
	jmp return

return:
	mov #0,r12
	pop r0
	
