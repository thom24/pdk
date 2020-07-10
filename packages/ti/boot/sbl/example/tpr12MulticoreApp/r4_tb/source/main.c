/*
 * R4 TB Main Implementation
 *
 * The file implements the entry point for the R4 Test code
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
 
#include "common_header.h"
#include "xmodem_test.h"
#include "uart_test.h"
#include "vim.h"

void c_undefined(void);
void c_pabort(void);
void c_dabort(void);
void c_swiInt(void);
void c_startup(void);
void InterruptEnDis (unsigned nval);

extern void __TI_auto_init(void);
void DefaultMpuConfig(void);

void c_undefined(void)
{
    for (;;);
}

void c_pabort(void)
{
}

void c_dabort(void)
{
}

void c_swiInt(void)
{
    for (;;);
}

void c_startup(void)
{
    /* Auto init all data section */
    __TI_auto_init();

    DefaultMpuConfig();
    
    /* Call Uart Test function */
    //while(1U);
    
    //uart_test();
    xmodemTest();
}

void IrqInt()
{
  FuncPtr irq_address_in_vim = 0x0;
  irq_address_in_vim = (FuncPtr )TB_VIM->IRQVECREG;
  (*irq_address_in_vim)();
}

void FiqInt()
{
  FuncPtr fiq_address_in_vim = 0x0;
  fiq_address_in_vim = (FuncPtr )TB_VIM->FIQVECREQ;
  (*fiq_address_in_vim)();

}

void DefaultMpuConfig(void)
{
    _mpuDisable_();
    _mpuDisableBackgroundRegion_();

    // Background Region
    _mpuSetRegion_(mpuREGION1);
    _mpuSetRegionBaseAddress_(0x0);
    _mpuSetRegionTypeAndPermission_(MPU_NORMAL_OINC_NONSHARED, MPU_PRIV_RW_USER_RW_EXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_4_GB);

    // QSPI Space
    _mpuSetRegion_(mpuREGION2);
    _mpuSetRegionBaseAddress_(0xC0000000);
    _mpuSetRegionTypeAndPermission_(MPU_STRONGLYORDERED_SHAREABLE, MPU_PRIV_RW_USER_RW_NOEXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_16_MB);

    // BSS PCR
    _mpuSetRegion_(mpuREGION3);
    _mpuSetRegionBaseAddress_(0x40000000);
    _mpuSetRegionTypeAndPermission_(MPU_STRONGLYORDERED_SHAREABLE, MPU_PRIV_RW_USER_RW_EXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_256_MB);

    // DSS VBUSP Config
    _mpuSetRegion_(mpuREGION4);
    _mpuSetRegionBaseAddress_(0x50000000);
    _mpuSetRegionTypeAndPermission_(MPU_STRONGLYORDERED_SHAREABLE, MPU_PRIV_RW_USER_RW_EXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_16_MB);

    // MSS PCR
    _mpuSetRegion_(mpuREGION5);
    _mpuSetRegionBaseAddress_(0xF0000000);
    _mpuSetRegionTypeAndPermission_(MPU_STRONGLYORDERED_SHAREABLE, MPU_PRIV_RW_USER_RW_EXEC);
    _mpuSetRegionSizeRegister_(mpuREGION_ENABLE | MPU_256_MB);

    _mpuEnableBackgroundRegion_();
    _mpuEnable_();

}

//-------------------------------------------------
// Interrupt enable disable function
//-------------------------------------------------
void InterruptEnDis (unsigned nval)
{
    switch (nval)
    {
    case 1:
       /* enable IRQ interrupt */
       asm(" mrs   r0, cpsr");
           asm(" msr   spsr_cxsf, r0"); //  store svc_cpsr value in svc_spsr
       asm(" bic   r0, r0, #0x80");
       asm(" msr   cpsr_cf, r0");
       return;

    case 2:
       /* enable FIQ interrupt */
       asm(" mrs   r0, cpsr");
           asm(" msr   spsr_cxsf, r0"); //  store svc_cpsr value in svc_spsr
       asm(" bic   r0, r0, #0x40");
       asm(" msr   cpsr_cf, r0");
       return;

    case 3:
       /* enable both interrupts */
       asm(" mrs   r0, cpsr");
           asm(" msr   spsr_cxsf, r0"); //  store svc_cpsr value in svc_spsr
       asm(" bic   r0, r0, #0xC0");
       asm(" msr   cpsr_cf, r0");
       return;

    case 4:
       /* disable IRQ interrupt */
       asm(" mrs   r0, cpsr");
           asm(" msr   spsr_cxsf, r0"); //  store svc_cpsr value in svc_spsr
       asm(" orr   r0, r0, #0x80");
       asm(" msr   cpsr_cf, r0");
       return;

    case 5:
       /* disable FIQ interrupt */
       asm(" mrs   r0, cpsr");
       asm(" orr   r0, r0, #0x40");
       asm(" msr   cpsr_cf, r0");
       return;
    case 6:
       /* disable both interrupts */
       asm(" mrs   r0, cpsr");
           asm(" msr   spsr_cxsf, r0"); //  store svc_cpsr value in svc_spsr
       asm(" orr   r0, r0, #0xC0");
       asm(" msr   cpsr_cf, r0");
       return;
    }
}


