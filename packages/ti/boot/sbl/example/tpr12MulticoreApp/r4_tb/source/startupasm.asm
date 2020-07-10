    .sect ".intvecs"
    .arm
  
    .def _ResetEntry

;***************************************************************
;* Reset Vector table
;***************************************************************
_ResetEntry: .asmfunc

    B	_c_int00             ;  RESET INTERRUPT
    B	_undefined           ;  UNDEFINED INSTRUCTION INTERRUPT
    B   _swiInt              ;  SOFTWARE INTERRUPT
    B	_pabort              ;  ABORT (PREFETCH) INTERRUPT
    B	_dabort              ;  ABORT (DATA) INTERRUPT
    B	#-8                  ;  RESERVED
    B   _irqEntry             ;  IRQ entry
    B   _fiqEntry             ;  FIQ entry
;    ldr pc,[pc,#-0x1b0]     ;  IRQ INTERRUPT: This will branch to the pending IRQ routine
;    ldr pc,[pc,#-0x1b0]     ;  FIQ INTERRUPT: This will branch to the pending FIQ routine

        .endasmfunc
.end



    .sect ".startup"
    .arm

;    .cdecls C,LIST,"startup.h"
     .cdecls C,LIST,"common_header.h"

;****************************************************************************
;*  GLOBALS                                                      
;****************************************************************************
    .global c_startup
    .global c_undefined
    .global c_swiInt
    .global c_pabort
    .global c_dabort
    .global exit 
    .global _SP_USERMODE
    .global _SP_SVCMODE
    .global _SP_IRQMODE
    .global _SP_FIQMODE
    .global _SP_UDEFMODE
    .global _SP_ABTMODE
    .global IrqInt
    .global FiqInt
    
;***************************************************************
;* CONSTANTS USED BY THIS MODULE
;***************************************************************
c_sp_user       .long     _SP_USERMODE
c_sp_svc        .long     _SP_SVCMODE
c_sp_irq        .long     _SP_IRQMODE
c_sp_fiq        .long     _SP_FIQMODE
c_sp_udef       .long     _SP_UDEFMODE
c_sp_abt        .long     _SP_ABTMODE



    
;***************************************************************
;* FUNCTION DEF: _c_int00 ,  32 BIT STATE BOOT ROUTINE     
;***************************************************************
       .def    _c_int00
_c_int00: .asmfunc

        .if __TI_NEON_SUPPORT__ | __TI_VFP_SUPPORT__
        ;*------------------------------------------------------
        ;* SETUP PRIVILEGED AND USER MODE ACCESS TO COPROCESSORS
        ;* 10 AND 11, REQUIRED TO ENABLE NEON/VFP
        ;* COPROCESSOR ACCESS CONTROL REG
        ;* BITS [23:22] - CP11, [21:20] - CP10
        ;* SET TO 0b11 TO ENABLE USER AND PRIV MODE ACCESS
        ;*------------------------------------------------------
        MRC      p15,#0,r0,c1,c0,#2
        MOV      r3,#0xf00000
        ORR      r0,r0,r3
        MCR      p15,#0,r0,c1,c0,#2

        ;*------------------------------------------------------
        ;* SETUP PRIVILEGED AND USER MODE ACCESS TO COPROCESSORS
        ;* BITS [27:25] - C15 ECC Control for TCM
        ;* BITS [2:0] - C15 EXT ERROR Control
        ;*------------------------------------------------------
  .if $$defined(AR_ECC_DISABLE)
        MRC      p15,#0,r0,c1,c0,#1
        MOV      r3,#0x0000000
        ORR      r0,r0,r3
        MCR      p15,#0,r0,c1,c0,#1
  .else      
        MRC      p15,#0,r0,c1,c0,#1
        MOV      r3,#0xe000000
        ORR      r0,r0,r3
        MCR      p15,#0,r0,c1,c0,#1
  .endif ; $$defined(AR_FULL_BOOT)      

        ;;   ;*------------------------------------------------------
        ;;   ;* SETUP PRIVILEGED AND USER MODE ACCESS TO COPROCESSORS
        ;;   ;* SET TO 0b111 TO ENABLE USER AND PRIV MODE ACCESS
        ;;   ;*------------------------------------------------------
        ;;   MRC      p15,#0x0,r0,c1,c0,#1
        ;;   MOV      r3,#0x7
        ;;   ORR      r0,r0,r3
        ;;   MCR      p15,#0x0,r0,c1,c0,#1

        ;*------------------------------------------------------
        ; SET THE EN BIT, FPEXC[30] TO ENABLE NEON AND VFP
        ;*------------------------------------------------------
        MOV      r0,#0x40000000
        FMXR     FPEXC,r0
        .endif

        ;*------------------------------------------------------
        ;* SET TO USER/SYSTEM (PREVILAGED) MODE
        ;*------------------------------------------------------
        MRS     r0, cpsr
        MSR     spsr_cxsf, r0  ; store svc_cpsr value in svc_spsr 
        BIC     r0, r0, #0x1F  ; CLEAR MODES
        ORR     r0, r0, #0x1F  ; SET SYSTEM MODE
        MSR     cpsr_cf, r0

        ;*------------------------------------------------------
        ;* Reset Async Abort Disable bit in CPSR
        ;*------------------------------------------------------
        CPSIE    a

        ;*------------------------------------------------------
        ;* INITIALIZE THE SYSTEM MODE STACK
        ;*------------------------------------------------------
        LDR     sp, c_sp_user

        ;*------------------------------------------------------
        ;* CLK SWITCH & MEM-INIT TODO UNCOMMENT THIS LATER
        ;*------------------------------------------------------
;  .if $$defined(AR_TCM_INIT)
;        BL     _mss_tcmb_meminit
;  .endif ;$$defined(AR_TCM_INIT)

;  .if $$defined(DEF_FUNC_TDL)
;        BL    _mss_meminit
;  .endif ;$$defined(DEF_FUNC_TDL)

;  .if $$defined(DEF_IOCHAR_TDL)
;        BL    _mss_meminit
;  .endif ;$$defined(DEF_IOCHAR_TDL)

;  .if $$defined(DEF_PBIST_DEVINIT)
;        BL     _pbistTCMB_
;  .endif ;$$defined(DEF_PBIST_DEVINIT)

        ;*-----------------------------------------------------
        ;* ALIGN THE STACK TO 64-BITS IF EABI.
        ;*-----------------------------------------------------
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif

        ;*------------------------------------------------------
        ;* INITIALIZE ALL R4 REGISTERS 
        ;*------------------------------------------------------
        BL      _R4RegistersInit
        
        ;*------------------------------------------------------
        ;* INITIALIZE ALL SP
        ;*------------------------------------------------------
        BL      _StackPointersInit
        
        ;*------------------------------------------------------
        ;* CALL APPLICATION CODE
        ;*------------------------------------------------------
        BLX      c_startup

        ;*------------------------------------------------------
        ;* IF APPLICATION DIDN'T CALL EXIT, CALL EXIT(1)
        ;*------------------------------------------------------
        MOV     R0, #1
        BL      exit

        ;*------------------------------------------------------
        ;* DONE, LOOP FOREVER
        ;*------------------------------------------------------
L1:     B       L1

        .endasmfunc
        
;*------------------------------------------------------
;* UDEF exception routine
;*------------------------------------------------------
        .def _undefined 
_undefined:  .asmfunc
        
        ; write exception entry and exit asm code here
        BLX      c_undefined;
        
        .endasmfunc

;*------------------------------------------------------
;* SVC interrupt routine
;*------------------------------------------------------
        .def _swiInt 
_swiInt:  .asmfunc
        
        ; write interrupt entry and exit asm code here
        BLX      c_swiInt;
        
        .endasmfunc

;*------------------------------------------------------
;* PREFETCH exception routine
;*------------------------------------------------------
        .def _pabort 
_pabort:  .asmfunc
        
        ; write exception entry and exit asm code here
        BLX      c_pabort;
        
        .endasmfunc

;*------------------------------------------------------
;* DATA exception routine
;*------------------------------------------------------
        .def _dabort 
_dabort:  .asmfunc
        
    SUB      lr,lr,#4
    SRSDB    sp!, #0x1F               ; Save LR_abt and SPSR_abt to System mode stack
    CPS      #0x1F                    ; Switch to System mode
    PUSH     {r0-r3, r12, lr}         ; Store normal scratch registers

    ; write exception entry and exit asm code here
    BLX      c_dabort;
        
    POP      {r0-r3, r12, lr}         ; Restore registers
    RFEIA    sp!                      ; Return using RFE from System mode stack

        .endasmfunc

;/**
; * @name    R4RegistersInit
; * @brief   Initilizes Cortex-R4F CPU's internal registers.
; *
; * Hercules series of MCU include dual Cortex-R4F CPUs in lock-step. At power up registers are not
; * guaranteed to be in same state for both the CPUs. The CPU pushes the internal registers on to
; * the stack on a function call, which could lead to the detection of a core compare error.
; * Therefore, the CPU internal core registers need to be initialized to a predefined state before
; * any function call is made. This function is recommended to be executed right after a reset.
; *
; * @param [in]  None
; * @param [out] None
; *
; * @return      None
; *
; * Example Usage:
; * @code
; *      
; * @code
; *
; * @entrymode   Any Privilege Mode
; * @exitmode    SVC Mode
; *
; * @seealso     
; *
; * @note        This function does not initialize Stack Pointers. SL_Init_StackPointers() must be called
; *              to initialize the stack pointers.
; *
; * @reqtag      \todo Add requirement tag here
; */

    .def _R4RegistersInit
    
_R4RegistersInit: .asmfunc

        ; Processor is in User/System Mode, R13 & R14 are already updated
        MOV     R1, #0
        MOV     R2, #0
        MOV     R3, #0
        MOV     R4, #0
        MOV     R5, #0
        MOV     R6, #0
        MOV     R7, #0
        MOV     R8, #0
        MOV     R9, #0
        MOV     R10, #0
        MOV     R11, #0
        MOV     R12, #0
        MRS     R0, CPSR
        MSR     SPSR_CXSF, R0

;       CPS     #ARM_MODE_FIQ
        CPS     #0x11
        MOV     R8, #0
        MOV     R9, #0
        MOV     R10, #0
        MOV     R11, #0
        MOV     R12, #0
        MOV     R13, #0
        MOV     R14, #0
        MRS     R0, CPSR
        MSR     SPSR_CXSF, R0

;        CPS     #ARM_MODE_IRQ
		CPS     #0x12
        MOV     R13, #0
        MOV     R14, #0
        MRS     R0, CPSR
        MSR     SPSR_CXSF, R0

;        CPS     #ARM_MODE_ABT
        CPS     #0x17
        MOV     R13, #0
        MOV     R14, #0
        MRS     R0, CPSR
        MSR     SPSR_CXSF, R0

;        CPS     #ARM_MODE_UND
        CPS     #0x1B
        MOV     R13, #0
        MOV     R14, #0
        MRS     R0, CPSR
        MSR     SPSR_CXSF, R0

;      CPS     #ARM_MODE_SVC
       CPS     #0x13
        MOV     R13, #0
        MOV     R14, #0

;        CPS     #ARM_MODE_SYS
        CPS     #0x1F
        
        MOV     R0, #0 ; clear R0 being used above 

        ; Clear VFP registers 
        .if __TI_NEON_SUPPORT__ | __TI_VFP_SUPPORT__        

        fmdrr d0,         r1,     r1
        fmdrr d1,         r1,     r1
        fmdrr d2,         r1,     r1
        fmdrr d3,         r1,     r1
        fmdrr d4,         r1,     r1
        fmdrr d5,         r1,     r1
        fmdrr d6,         r1,     r1
        fmdrr d7,         r1,     r1
        fmdrr d8,         r1,     r1
        fmdrr d9,         r1,     r1
        fmdrr d10,        r1,     r1
        fmdrr d11,        r1,     r1
        fmdrr d12,        r1,     r1
        fmdrr d13,        r1,     r1
        fmdrr d14,        r1,     r1
        fmdrr d15,        r1,     r1
        
        .endif ; __TI_NEON_SUPPORT__ | __TI_VFP_SUPPORT__
        
        BX      LR
        
    .endasmfunc


;/**
; * @name    StackPointersInit
; * @brief   Initilizes CPU's stack pointer for SVC, FIQ, IRQ, ABT, UND & SYS modes.
; *
; * This function initializes the stack pointers for the privileged modes of the CPU.
; * Following variables are used as stack pointers and hence must be defined by the application.
; *                  StackModeSVC, StackModeFIQ, StackModeIRQ, StackModeABT, StackModeUND, StackModeSYS
; * Function uses following macros in bytes, from sl_config.h file, to calculate the stack start.
; *                  STACK_SIZE_MODE_SVC, STACK_SIZE_MODE_FIQ, STACK_SIZE_MODE_IRQ, STACK_SIZE_MODE_ABT,
; *                  STACK_SIZE_MODE_UND & STACK_SIZE_MODE_SYS
; *
; * @param [in]  None
; * @param [out] None
; *
; * @return      None
; *
; * Example Usage:
; * @code
; *      
; * @code
; *
; * @entrymode   Any Privilege Mode
; * @exitmode    SVC Mode
; *
; * @seealso     
; *
; * @note        This function does not initialize the SP for User Mode.
; *
; * @reqtag      \todo Add requirement tag here
; */

    .def _StackPointersInit
 
_StackPointersInit:  .asmfunc

        ; Processor is in User/System Mode, SP is already updated
;       CPS     #ARM_MODE_SVC
        CPS     #0x13
        LDR     SP, c_sp_svc
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif

;        CPS     #ARM_MODE_FIQ
		CPS     #0x11
        LDR     SP, c_sp_fiq
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif

;        CPS     #ARM_MODE_IRQ
		CPS     #0x12
        LDR     SP, c_sp_irq
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif
        
;        CPS     #ARM_MODE_ABT
        CPS     #0x17
        LDR     SP, c_sp_abt
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif
        
;        CPS     #ARM_MODE_UND
        CPS     #0x1B
        LDR     SP, c_sp_udef
        .if __TI_EABI_ASSEMBLER
        BIC     sp, sp, #0x07  ; Clear upper 3 bits for 64-bit alignment.
        .endif
        
;        CPS     #ARM_MODE_SYS
        CPS     #0x1F
		
        BX      LR

        .endasmfunc
        
;/** ------------------------------------------------------------------------------
; * @name    _pbistTCMB_
; * @brief
; *
; */


;TOPRCM_DFT_REG0 .word       0xFFFFE160
;PBIST_PACT      .word       0xFFFFE580
;PBIST_LO        .word       0xFFFFE510
;PBIST_ID        .word       0xFFFFE584
;PBIST_OVR       .word       0xFFFFE588
;PBIST_DLR       .word       0xFFFFE564
;PBIST_ALGO      .word       0xFFFFE5C4
;PBIST_RINFOL    .word       0xFFFFE5C8
;PBIST_RINFOU    .word       0xFFFFE5CC
;PBIST_ROM       .word       0xFFFFE5C0
;PBIST_INTR_REG  .word       0xFFFFFE28
;PBIST_INTR_VAL  .equ        0x00200000
;PBIST_FSCR0_REG .word       0xFFFFE598
;PBIST_FSCR1_REG .word       0xFFFFE59C
;STCGSTAT        .word       0xFFFFE614
;MEM_INIT_START  .word       0xFFFFFF5c ; wr 0x2027
;MEM_INIT_DONE   .word       0xFFFFFF6c ; check 0x2027
;MEM_INIT_START_DATA_TCMB .word   0xAD000002  ; 
;MEM_INIT_START_DATA .word   0xADFFFFFE  ; 
;MEM_INIT_DONE_DATA_TCMB  .word   0x00000002  ;
;MEM_INIT_DONE_DATA  .word   0x0003C3CE  ;




;    .def _pbistTCMB_
;    .asmfunc
;
;_pbistTCMB_
;        ; Check if STC Status   
;        ldr     r3, STCGSTAT
;        ldr     r4, [r3]
;	mov     r5, #0x3
;	and     r4, r4, r5
;        cmp     r4, #0
;        bne     PBIST_BYPASS
;
;        ; Setup Selftest Key, Active low reset contol bit during PBIST selftest
;        ; TCMB select is bit 12 of DFT REG0
;        ldr     r5, TOPRCM_DFT_REG0
;        mov     r6, #((0xA) | (0x1 << 12))
;        str     r6, [r5]
;
;        ;Enable Pbist Internal clocks
;        ldr     r5, PBIST_PACT
;        mov     r6, #0x3
;        str     r6, [r5]
;
;        ; Loop count
;        ldr     r5, PBIST_LO
;        mov     r6, #0x0
;        str     r6, [r5]
;
;        ; PBIST ID  - always ‘1’
;        ldr     r5, PBIST_ID
;        mov     r6, #0x1
;        str     r6, [r5]
;
;        ; overide – fixed programmation
;        ldr     r5, PBIST_OVR
;        mov     r6, #0xF
;        str     r6, [r5]
;
;        ; pbist controller settings
;        ldr     r5, PBIST_DLR
;        movw    r6, #((0x0 << 8) | (0x10))
;        movt    r6, #((0x0 << 8) | (0x0))
;        str     r6, [r5]
;
;        ; Setup the Algo
;        ldr     r5, PBIST_ALGO
;        movw    r6, #((0x4 << 8) | (0x0))
;        movt    r6, #((0x0 << 8) | (0x0))
;        str     r6, [r5]
;
;        ; To program the memory group – one hot coding
;        ldr     r5, PBIST_RINFOL
;        movw    r6, #((0x0 << 8) | (0x0))
;        movt    r6, #((0x1 << 8) | (0x0))
;        str     r6, [r5]
;
;        ldr     r5, PBIST_RINFOU
;        movw    r6, #((0x0 << 8) | (0x0))
;        movt    r6, #((0x0 << 8) | (0x0))
;        str     r6, [r5]
;
;        ; re-program the override
;        ldr     r5, PBIST_OVR
;        mov     r6, #0x0
;        str     r6, [r5]
;
;        ; PBIST ID  - set to 0
;        ldr     r5, PBIST_ID
;        mov     r6, #0x0
;        str     r6, [r5]
;
;        ; Microcode location from the OnChip ROM default is onchip ROM
;        ldr     r5, PBIST_ROM
;        mov     r6, #0x3
;        str     r6, [r5]
;
;        ; To start the test
;        ldr     r5, PBIST_DLR
;        movw    r6, #((0x0 << 8) | (0x1C))
;        movt    r6, #((0x0 << 8) | (0x0))
;        str     r6, [r5]
;
;        ;Wait for PBIST interrupt
;LPPBIST_INTR:
;        ldr     r5, PBIST_INTR_REG
;        ldr     r5, [r5]
;        tst     r5, #PBIST_INTR_VAL
;        beq     LPPBIST_INTR
;
;        ldr     r5, PBIST_FSCR0_REG
;        ldr     r5, [r5]
;        cmp     r5, #0
;        bne     LOOP_PBIST_ERR
;        ldr     r5, PBIST_FSCR1_REG
;        ldr     r5, [r5]
;        cmp     r5, #0
;        bne     LOOP_PBIST_ERR
;
;        ; Clear PBIST interrupt
;        ldr     r5, PBIST_INTR_REG
;        mov     r6, #PBIST_INTR_VAL
;        str     r6, [r5]
;
;        ;Disable Pbist Internal clocks
;        ldr     r5, PBIST_PACT
;        mov     r6, #0x0
;        str     r6, [r5]
;
;        ldr     r5, TOPRCM_DFT_REG0
;        mov     r6, #0
;        str     r6, [r5]
;
;        ; TCMA RAM & TCMB meminit     
;        ldr  r5, MEM_INIT_START
;        ldr  r6, MEM_INIT_START_DATA_TCMB
;        str  r6, [r5]
;        ldr  r5, MEM_INIT_DONE
;        ldr  r6, MEM_INIT_DONE_DATA_TCMB
;
;        ; check if meminit done  
;LPINIT_CHECK: 
;        ldr  r7, [r5]
;        cmp  r7, r6
;        bne  LPINIT_CHECK
;
;PBIST_BYPASS:
;	bx     lr
;
;LOOP_PBIST_ERR:
;        b       LOOP_PBIST_ERR
;
;        .endasmfunc
;
;

;/**
; * @name    _mss_meminit
; * @brief   
; *
; */


;    .def _mss_meminit
;    
;_mss_meminit: .asmfunc
;         ; TCMA RAM & TCMB meminit     
;         LDR  r5, MEM_INIT_START
;         LDR  r6, MEM_INIT_START_DATA
;         STR  r6, [r5]
;         LDR  r5, MEM_INIT_DONE
;         LDR  r6, MEM_INIT_DONE_DATA
;TDLCHECK: LDR  r7, [r5]
;         CMP  r7, r6
;         BNE  LPCHECK
;         BX      LR
;        .endasmfunc
;


;/**
; * @name    _mss_tcmb_meminit
; * @brief   
; *
; */

;
;    .def _mss_tcmb_meminit
;    
;_mss_tcmb_meminit: .asmfunc
;         ; TCMA RAM & TCMB meminit     
;         LDR  r5, MEM_INIT_START
;         LDR  r6, MEM_INIT_START_DATA_TCMB
;         STR  r6, [r5]
;         LDR  r5, MEM_INIT_DONE
;         LDR  r6, MEM_INIT_DONE_DATA_TCMB
;LPCHECK: LDR  r7, [r5]
;         CMP  r7, r6
;         BNE  LPCHECK
;         BX      LR
;        .endasmfunc
;


       .def _irqEntry 
_irqEntry:  .asmfunc
     
           SUB      lr, lr, #4
           SRSDB    sp!, #31                 ; Save LR_irq and SPSR_irq to System mode stack
           CPS      #031                     ; Switch to System mode
           PUSH     {r0-r3, r12, LR}             ; Store normal scratch registers
           BLX      IrqInt               ; DKS
           POP      {r0-r3, r12, LR}             ; Restore registers
           RFEIA    sp!                      ; Return using RFE from System mode stack
             
       .endasmfunc

       .def _fiqEntry 
_fiqEntry:  .asmfunc
     
           SUB      lr, lr, #4
           SRSDB    sp!, #31                 ; Save LR_irq and SPSR_irq to System mode stack
           CPS      #031                     ; Switch to System mode
           PUSH     {r0-r3, r12, LR}             ; Store normal scratch registers
           BLX      FiqInt                 ; DKS
           POP      {r0-r3, r12, LR}             ; Restore registers
           RFEIA    sp!                      ; Return using RFE from System mode stack
             
       .endasmfunc

;-------------------------------------------------------------------------------
; Enable Offset via Vic controller

;    .def     coreEnableIrqVicOffset
;coreEnableIrqVicOffset:    .asmfunc
;
;        stmfd sp!, {r0}
;        mrc   p15, #0, r0,         c1, c0,  #0
;        orr   r0,  r0,    #0x01000000
;        mcr   p15, #0, r0,         c1, c0,  #0
;        ldmfd sp!, {r0}        
;        bx    lr
;
;    .endasmfunc
; 
;
;    .def     burst_access
;burst_access:    .asmfunc
;        PUSH     {r2-r9}   ; Store normal scratch registers
;        ldmia r0!, {r2-r9}     
;        stmia r1!, {r2-r9}      
;        POP      {r2-r9}   ; Restore registers
;        bx    lr
;
;    .endasmfunc
; 

;-------------------------------------------------------------------------------
; Enable Mpu
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuEnable_
    .asmfunc

_mpuEnable_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        orr   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuDisable_
    .asmfunc

_mpuDisable_

        stmfd sp!, {r0}
        mrc   p15, #0, r0, c1, c0, #0
        bic   r0,  r0, #1
        dsb
        mcr   p15, #0, r0, c1, c0, #0
        isb
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Enable Mpu background region
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuEnableBackgroundRegion_
    .asmfunc

_mpuEnableBackgroundRegion_

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        orr   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Disable Mpu background region
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuDisableBackgroundRegion_
    .asmfunc

_mpuDisableBackgroundRegion_

        stmfd sp!, {r0}
        mrc   p15, #0, r0,      c1, c0, #0
        bic   r0,  r0, #0x20000
        mcr   p15, #0, r0,      c1, c0, #0
        ldmfd sp!, {r0}
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns number of implemented Mpu regions
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuGetNumberOfRegions_
    .asmfunc

_mpuGetNumberOfRegions_

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0, ROR #8
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Returns the type of the implemented mpu regions
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuAreRegionsSeparate_
    .asmfunc

_mpuAreRegionsSeparate_

        mrc   p15, #0, r0,      c0, c0, #4
        uxtb  r0,  r0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set mpu region number
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuSetRegion_
    .asmfunc

_mpuSetRegion_

        mcr   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get mpu region number
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuGetRegion_
    .asmfunc

_mpuGetRegion_

        mrc   p15, #0, r0, c6, c2, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set base address
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuSetRegionBaseAddress_
    .asmfunc

_mpuSetRegionBaseAddress_

        mcr   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get base address
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuGetRegionBaseAddress_
    .asmfunc

_mpuGetRegionBaseAddress_

        mrc   p15, #0, r0, c6, c1, #0
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set type and permission
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuSetRegionTypeAndPermission_
    .asmfunc

_mpuSetRegionTypeAndPermission_

        orr   r0,  r0, r1
        mcr   p15, #0, r0, c6, c1, #4
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get type
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuGetRegionType_
    .asmfunc

_mpuGetRegionType_

        mrc   p15, #0, r0,     c6, c1, #4
        bic   r0,  r0, #0xFF00
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Get permission
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuGetRegionPermission_
    .asmfunc

_mpuGetRegionPermission_

        mrc   p15, #0, r0,   c6, c1, #4
        bic   r0,  r0, #0xFF
        bx    lr

    .endasmfunc


;-------------------------------------------------------------------------------
; Set region size register value
; SourceId :
; DesignId :
; Requirements :

    .def     _mpuSetRegionSizeRegister_
    .asmfunc

_mpuSetRegionSizeRegister_


        mcr   p15, #0, r0, c6, c1, #2
        bx    lr

    .endasmfunc



    .end
