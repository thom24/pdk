/*
 * VIM Header File
 *
 * This is the header file which exposes the API to configure the VIM
 *
 * Copyright (C) 2019 Texas Instruments Incorporated - http://www.ti.com/
 * ALL RIGHTS RESERVED
 *
 */
#ifndef VIM_H
#define VIM_H

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
/* Standard Include Files. */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VIM_MEM_U_BASE              (0xFFF82000U)
#define VIM_U_BASE                  (0xFFFFFE00U)
#define VIM_PARITY_U_BASE           (0xFFFFFD00U)

#define SCIA_INT_REQ                (23U)


//-------------------------------------------------------VIM-----------------------------------------------------------------------

typedef volatile	struct
{
	struct
  	{
        unsigned char IRQIVEC_UB;	        	/* 0x00     */
        uint32_t : 24U;           
  	} IRQIVEC;	

	struct
  	{
        unsigned char FIQIVEC_UB;               /* 0x04      */
        uint32_t : 24U; 
  	} FIQIVEC;
	
	struct
  	{
        uint32_t  NEST_ENABLE:4U;		    /* 0x08      */
        uint32_t : 28U;	 
  	} NEST_EN;	
    	
	struct
   	{
		uint32_t NEST_OVRN :1U;              /* 0x0c      */
		uint32_t NEST_STAT :1U;
		uint32_t : 6U;
		uint32_t NEST_LEVEL: 8U;
		uint32_t : 16U;
 	} NEST_CTRL;		

    uint32_t FIRQPR0;                   /* 0x10      */

    uint32_t FIRQPR1;                   /* 0x14      */

    uint32_t FIRQPR2;                   /* 0x18      */

    uint32_t FIRQPR3;                   /* 0x1c      */
    
	struct
   	{
		uint32_t FTC_INT_GA_B1: 1U;
		uint32_t LFS_INT_GA_B1: 1U;
		uint32_t BTC_INT_GA_B1: 1U;
		uint32_t HBC_INT_GA_B1: 1U;
		uint32_t BER_INT_GA_B1: 1U;
		uint32_t SCI_TX_INT_B1: 1U;     
		uint32_t SCI_RX_INT_B1: 1U;
		uint32_t SCI_ER_INT_B1: 1U;
		uint32_t : 6U;
		uint32_t RTITB_RQ_B1: 1U;
		uint32_t RTIOVL_RQ1_B1: 1U;
		uint32_t RTIOVL_RQ0_B1: 1U;
		uint32_t RTI_INT_RQ3_B1: 1U;
		uint32_t RTI_INT_RQ2_B1: 1U;
		uint32_t RTI_INT_RQ1_B1: 1U;
		uint32_t RTI_INT_RQ0_B1: 1U;
		uint32_t : 1U;
		uint32_t CRC_INT_B1: 1U;
		uint32_t SYS_ER_INT_B1: 1U;
		uint32_t SYS_SW_INT_B1: 1U;
   	}INTREQ0;

    uint32_t INTREQ1;                   /* 0x24      */

    uint32_t INTREQ2;                   /* 0x28      */

    uint32_t INTREQ3;                   /* 0x2c      */

    uint32_t REQMASKSET0;               /* 0x30      */

    uint32_t REQMASKSET1;               /* 0x34      */

    uint32_t REQMASKSET2;               /* 0x38      */

    uint32_t REQMASKSET3;               /* 0x3c      */

    uint32_t REQMASKCLR0;               /* 0x40      */

    uint32_t REQMASKCLR1;               /* 0x44      */

    uint32_t REQMASKCLR2;               /* 0x48      */

    uint32_t REQMASKCLR3;               /* 0x4c      */

    uint32_t WAKEMASKSET0;              /* 0x50      */

    uint32_t WAKEMASKSET1;              /* 0x54      */

    uint32_t WAKEMASKSET2;              /* 0x58      */

    uint32_t WAKEMASKSET3;              /* 0x5c      */

    uint32_t WAKEMASKCLR0;              /* 0x60      */

    uint32_t WAKEMASKCLR1;              /* 0x64      */

    uint32_t WAKEMASKCLR2;              /* 0x68      */

    uint32_t WAKEMASKCLR3;              /* 0x6c      */

    uint32_t IRQVECREG;                 /* 0x70      */

    uint32_t FIQVECREQ;                 /* 0x74      */
    	
	struct
 	{		
		uint32_t CAPEVTSRC0_B7 :7U;  	/* 0x78 */
		uint32_t : 9U;  
		uint32_t CAPEVTSRC1_B7 :7U;
		uint32_t : 9U;  
 	}CAPEVT;

    uint32_t : 32U;                     /* 0x7c      */

	struct
   	{	     
	    uint32_t CHANMAPX3 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX2 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX1 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX0 : 7U;
	    uint32_t : 1U;

  	}CHANCTRL0;
	
	struct
   	{	     
	    uint32_t CHANMAPX3 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX2 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX1 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX0 : 7U;
	    uint32_t : 1U;

  	}CHANCTRL1;
	
	struct
   	{	     
	    uint32_t CHANMAPX3 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX2 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX1 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX0 : 7U;
	    uint32_t : 1U;

  	}CHANCTRL2;
	
	struct
   	{	     
	    uint32_t CHANMAPX3 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX2 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX1 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX0 : 7U;
	    uint32_t : 1U;

  	}CHANCTRL3;
	
	struct
   	{	     
	    uint32_t CHANMAPX3 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX2 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX1 : 7U;
	    uint32_t : 1U;
	    uint32_t CHANMAPX0 : 7U;
	    uint32_t : 1U;

  	}CHANCTRL4;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL5;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL6;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL7;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL8;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL9;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL10;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL11;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL12;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL13;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL14;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL15;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL16;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL17;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL18;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL19;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL20;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL21;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL22;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL23;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL24;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL25;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL26;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL27;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL28;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL29;
	
	struct
   	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL30;
	
	struct
  	{	     
		uint32_t CHANMAPX3 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX2 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX1 : 7U;
		uint32_t : 1U;
		uint32_t CHANMAPX0 : 7U;
		uint32_t : 1U;

  	}CHANCTRL31;


} VIM_ST;

typedef volatile	struct
{
	struct
  	{
		uint32_t PARFLG:1U; 
		uint32_t : 7U;
        uint32_t SBERR: 1U;
		uint32_t : 23U;
  	} PARFLG;

	struct
  	{      
        uint32_t PARENA: 4U; 
        uint32_t : 4U;
        uint32_t TEST : 4U;
        uint32_t :4U;
        uint32_t EDAC: 4U;
        uint32_t : 4U;
        uint32_t SBE: 4U;     
        uint32_t :4U;

  	} PARCTL;
	
	struct
  	{
        uint32_t : 2U;  
        uint32_t ADDERR: 8U;
        uint32_t RAMBASE: 22U;     

  	} ADDERR;

	uint32_t FBPAERR;                 /* 0xFDF8      */
	struct
  	{
        uint32_t : 2U;
        uint32_t SBERR: 8U;
        uint32_t RAMBASE: 22U;     
    
  	} SBERR;
} VIM_PARITY_ST;


typedef volatile	struct
{
    uint32_t IRQIVEC;         
    uint32_t FIQIVEC;
    uint32_t NEST_EN;         
    uint32_t NEST_CTRL;
    uint32_t FIRQPR0;         
    uint32_t FIRQPR1;         
    uint32_t FIRQPR2;         
    uint32_t FIRQPR3;         
    uint32_t INTREQ0;
    uint32_t INTREQ1;              
    uint32_t INTREQ2;              
    uint32_t INTREQ3;              
    uint32_t REQMASKSET0;          
    uint32_t REQMASKSET1;          
    uint32_t REQMASKSET2;          
    uint32_t REQMASKSET3;          
    uint32_t REQMASKCLR0;          
    uint32_t REQMASKCLR1;          
    uint32_t REQMASKCLR2;          
    uint32_t REQMASKCLR3;          
    uint32_t WAKEMASKSET0;         
    uint32_t WAKEMASKSET1;         
    uint32_t WAKEMASKSET2;         
    uint32_t WAKEMASKSET3;         
    uint32_t WAKEMASKCLR0;         
    uint32_t WAKEMASKCLR1;         
    uint32_t WAKEMASKCLR2;         
    uint32_t WAKEMASKCLR3;         
    uint32_t IRQVECREG;            
    uint32_t FIQVECREQ;            
    uint32_t CAPEVT; 
    uint32_t CHANCTRL0;
    uint32_t CHANCTRL1;
    uint32_t CHANCTRL2;
    uint32_t CHANCTRL3;
    uint32_t CHANCTRL4;
    uint32_t CHANCTRL5;
    uint32_t CHANCTRL6;
    uint32_t CHANCTRL7;
    uint32_t CHANCTRL8;
    uint32_t CHANCTRL9;
    uint32_t CHANCTRL10;
    uint32_t CHANCTRL11;
    uint32_t CHANCTRL12;
    uint32_t CHANCTRL13;
    uint32_t CHANCTRL14;
    uint32_t CHANCTRL15;
    uint32_t CHANCTRL16;
    uint32_t CHANCTRL17;
    uint32_t CHANCTRL18;
    uint32_t CHANCTRL19;
    uint32_t CHANCTRL20;
    uint32_t CHANCTRL21;
    uint32_t CHANCTRL22;
    uint32_t CHANCTRL23;
    uint32_t CHANCTRL24;
    uint32_t CHANCTRL25;
    uint32_t CHANCTRL26;
    uint32_t CHANCTRL27;
    uint32_t CHANCTRL28;
    uint32_t CHANCTRL29;
    uint32_t CHANCTRL30;
    uint32_t CHANCTRL31;
} VIM_ST_REG;

typedef volatile	struct
{
    uint32_t PARFLG;
    uint32_t PARCTL;
    uint32_t ADDERR;
    uint32_t FBPAERR;
    uint32_t SBERR;
} VIM_PARITY_ST_REG;

typedef volatile	struct
{
    FuncPtr VIM_MEM[128];
} VIM_MEM_ST;


VIM_PARITY_ST *TB_VIM_PARITY   = (VIM_PARITY_ST *) VIM_PARITY_U_BASE;
VIM_PARITY_ST_REG *TB_VIM_PARITY_REG   = (VIM_PARITY_ST_REG *) VIM_PARITY_U_BASE;

VIM_ST *TB_VIM   = (VIM_ST *) VIM_U_BASE;
VIM_ST_REG *TB_VIM_REG   = (VIM_ST_REG *) VIM_U_BASE;
VIM_MEM_ST *TB_VIM_MEM   = (VIM_MEM_ST *) VIM_MEM_U_BASE;

#ifdef __cplusplus
}
#endif

#endif /* VIM_H */

