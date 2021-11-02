;
;  Copyright (C) 2018-2021 Texas Instruments Incorporated
;
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
;
;    Redistributions of source code must retain the above copyright
;    notice, this list of conditions and the following disclaimer.
;
;    Redistributions in binary form must reproduce the above copyright
;    notice, this list of conditions and the following disclaimer in the
;    documentation and/or other materials provided with the
;    distribution.
;
;    Neither the name of Texas Instruments Incorporated nor the names of
;    its contributors may be used to endorse or promote products derived
;    from this software without specific prior written permission.
;
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

;
;  \file   SafeRTOS_Utilsasm.asm
;

        .text
        .arm
        .ref vDataAbort_c
		.sect "KERNEL_FUNCTION"

;------------------------------------------------------------------------------
; Constant Definitions
;------------------------------------------------------------------------------

HIGHEST_SVC                             .equ ( 0x05 )

ARM_MODE_MASK                           .equ ( 0x1F )
ISR_MODE                               	.equ ( 0x12 )

TRUE                                    .equ ( 1 )
FALSE                                   .equ ( 0 )


;  FUNCTION DEF: void vDataAbort(void)
        .global vDataAbort
        .arm
vDataAbort:
	;  Return to the instruction following the interrupted.
	SUB		lr, lr, #4

	;  Push the return address and SPSR.
	PUSH	{lr}
	MRS	lr, SPSR
	PUSH	{lr}

	;  Push used registers.
	PUSH	{r0-r4, r12}

	;  Call the interrupt handler.
	LDR	r1, vApplicationDataAbortHandlerConst
	BLX	r1

	;  Restore used registers, LR and SPSR before  returning.
	POP	{r0-r4, r12}
	POP	{LR}
	MSR	SPSR_cxsf, LR
	POP	{LR}
	MOVS	PC, LR


vApplicationDataAbortHandlerConst: .word vDataAbort_c

;-------------------------------------------------------------------------------
; portBaseType xPortIsISRMode( void )
;------------------------------------------------------------------------------
    .global xPortIsISRMode


xPortIsISRMode:
        MRS     R0, CPSR
        AND     R0, R0, #ARM_MODE_MASK
        CMP     R0, #ISR_MODE                	; Is the task running ISR?
        MOVNE   R0, #FALSE                      ; Return false.
        MOVEQ   R0, #TRUE                       ; Return true.

        BX      LR



;------------------------------------------------------------------------------
;  void vPortSetDLFOBit( void )
;------------------------------------------------------------------------------
	.global vPortSetDLFOBit
vPortSetDLFOBit:
        PUSH    {r8}                            ; Save r8 register contents
        MRC     p15, #0, r8, c1, c0, #1         ; Read ACTRL register into r8
        ORR     r8, r8, #(1<<13)                ; Set DLFO bit in ACTRL register
        MCR     p15, #0, r8, c1, c0, #1         ; Write back ACTRL register
        POP     {r8}                            ; Restore r8 register contents
        BX      lr

    ;.size vPortSetDLFOBit, . - vPortSetDLFOBit

