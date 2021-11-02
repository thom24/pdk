;
;	Copyright (C)2006 onward WITTENSTEIN aerospace & simulation limited. All rights reserved.
;
;	This file is part of the SafeRTOS product, see projdefs.h for version number
;	information.
;
;	SafeRTOS is distributed exclusively by WITTENSTEIN high integrity systems,
;	and is subject to the terms of the License granted to your organization,
;	including its warranties and limitations on use and distribution. It cannot be
;	copied or reproduced in any way except as permitted by the License.
;
;	Licenses authorize use by processor, compiler, business unit, and product.
;
;	WITTENSTEIN high integrity systems is a trading name of WITTENSTEIN
;	aerospace & simulation ltd, Registered Office: Brown's Court, Long Ashton
;	Business Park, Yanley Lane, Long Ashton, Bristol, BS41 9LB, UK.
;	Tel: +44 (0) 1275 395 600, fax: +44 (0) 1275 393 630.
;	E-mail: info@HighIntegritySystems.com
;
;	www.HighIntegritySystems.com
;-------------------------------------------------------------------------------

	.text
	.arm
	.sect "KERNEL_FUNCTION"
;-------------------------------------------------------------------------------
; Public Functions
;-------------------------------------------------------------------------------

	.global ulGetDataFaultStatusRegister
	.global ulGetDataFaultAddressRegister
	.global ulGetInstructionFaultStatusRegister
	.global ulGetInstructionFaultAddressRegister

;-------------------------------------------------------------------------------
; portUInt32Type ulGetDataFaultStatusRegister( void )
;-------------------------------------------------------------------------------
ulGetDataFaultStatusRegister:
        MRC     p15, #0, r0, c5, c0, #0
        BX      LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetDataFaultAddressRegister( void )
;-------------------------------------------------------------------------------
ulGetDataFaultAddressRegister:
        MRC     p15, #0, r0, c6, c0, #0
        BX      LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetInstructionFaultStatusRegister( void )
;-------------------------------------------------------------------------------
ulGetInstructionFaultStatusRegister:
        MRC     p15, #0, r0, c5, c0, #1
        BX      LR

;-------------------------------------------------------------------------------
; portUInt32Type ulGetInstructionFaultAddressRegister( void )
;-------------------------------------------------------------------------------
ulGetInstructionFaultAddressRegister:
        MRC     p15, #0, r0, c6, c0, #2
        BX      LR

;-------------------------------------------------------------------------------

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
