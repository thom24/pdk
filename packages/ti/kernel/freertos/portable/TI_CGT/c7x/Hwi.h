/*
 *  Copyright (C) 2018-2021 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/*
 * ======== GENERATED SECTIONS ========
 *
 *     PROLOGUE
 *     INCLUDES
 *
 *     CREATE ARGS
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     PER-INSTANCE TYPES
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef Hwi__include
#define Hwi__include

/*
 * ======== INCLUDES ========
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <c7x.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ======== INCLUDES ========
 */

typedef struct Hwi_Params Hwi_Params;
typedef struct Hwi_Object Hwi_Object;
typedef struct Hwi_Object HwiC7x_Struct;
typedef Hwi_Object* Hwi_Handle;
typedef Hwi_Object* Hwi_Instance;



/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/* FuncPtr */
typedef void (*Hwi_FuncPtr)(unsigned int arg1);

/* Irp */
typedef unsigned int Hwi_Irp;

/* MaskingOption */
enum Hwi_MaskingOption {
    Hwi_MaskingOption_NONE,
    Hwi_MaskingOption_ALL,
    Hwi_MaskingOption_SELF,
    Hwi_MaskingOption_BITMASK,
    Hwi_MaskingOption_LOWER
};
typedef enum Hwi_MaskingOption Hwi_MaskingOption;

/* StackInfo */
struct Hwi_StackInfo {
    size_t hwiStackPeak;
    size_t hwiStackSize;
    void * hwiStackBase;
};
typedef struct Hwi_StackInfo Hwi_StackInfo;


/* NUM_INTERRUPTS */
#define Hwi_NUM_INTERRUPTS (64)

/* PlugFuncPtr */
typedef void (*Hwi_PlugFuncPtr)(void );

/* TSR_CXM */
enum Hwi_TSR_CXM {
    Hwi_TSR_CXM_GuestUser,
    Hwi_TSR_CXM_GuestSupervisor,
    Hwi_TSR_CXM_RootUser,
    Hwi_TSR_CXM_RootSupervisor,
    Hwi_TSR_CXM_SecureUser,
    Hwi_TSR_CXM_SecureSupervisor
};
typedef enum Hwi_TSR_CXM Hwi_TSR_CXM;


/*
 * ======== CREATE ARGS ========
 */

/* Args__create */
typedef struct Hwi_Args_create {
    int32_t intNum;
    Hwi_FuncPtr hwiFxn;
} Hwi_Args_create;


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/* Module_State */
typedef char Hwi_Module_State_intEvents[Hwi_NUM_INTERRUPTS];
typedef Hwi_Handle Hwi_Module_State_dispatchTable[Hwi_NUM_INTERRUPTS];


/*
 * ======== MODULE-WIDE CONFIGS ========
 */


/* dispatcherAutoNestingSupport */
extern const bool Hwi_dispatcherAutoNestingSupport;

/* dispatcherIrpTrackingSupport */
extern const bool Hwi_dispatcherIrpTrackingSupport;

/* DEFAULT_INT_PRIORITY */
extern const unsigned int Hwi_DEFAULT_INT_PRIORITY;

/* bootToNonSecure */
extern const bool Hwi_bootToNonSecure;

/* enableException */
extern const bool Hwi_enableException;

/* vectorTableBase */
extern const void * Hwi_vectorTableBase;

/* vectorTableBase_SS */
extern const void * Hwi_vectorTableBase_SS;


/*
 * ======== PER-INSTANCE TYPES ========
 */

/* Params */
struct Hwi_Params {
    Hwi_MaskingOption maskSetting;
    uintptr_t arg;
    bool enableInt;
    int32_t eventId;
    int32_t priority;
    unsigned long disableMask;
    unsigned long restoreMask;
};

/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
void Hwi_Module_startup( void );


/* Instance_init */
int32_t Hwi_Instance_init(Hwi_Object *obj, int32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* Instance_finalize */
void Hwi_Instance_finalize(Hwi_Object *obj, int32_t ec);

/* create */
Hwi_Handle Hwi_create( int32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* construct */
int32_t Hwi_construct(HwiC7x_Struct *obj, int32_t intNum, Hwi_FuncPtr hwiFxn, const Hwi_Params *prms);

/* delete */
void Hwi_delete(Hwi_Handle *instp);

/* destruct */
void Hwi_destruct(HwiC7x_Struct *obj);

/* getStackInfo */
bool Hwi_getStackInfo( Hwi_StackInfo *stkInfo, bool computeStackDepth);

/* getCoreStackInfo */
bool Hwi_getCoreStackInfo( Hwi_StackInfo *stkInfo, bool computeStackDepth, unsigned int coreId);

/* startup */
void Hwi_startup( void);

/* switchFromBootStack */
void Hwi_switchFromBootStack( void);

/* post */
void Hwi_post( unsigned int intNum);

/* getTaskSP */
char *Hwi_getTaskSP( void);

/* disableInterrupt */
unsigned int Hwi_disableInterrupt( unsigned int intNum);

/* enableInterrupt */
unsigned int Hwi_enableInterrupt( unsigned int intNum);

/* restoreInterrupt */
void Hwi_restoreInterrupt( unsigned int intNum, unsigned int key);

/* clearInterrupt */
void Hwi_clearInterrupt( unsigned int intNum);

/* getIrp */
Hwi_Irp Hwi_getIrp( Hwi_Handle instp);

/* eventMap */
void Hwi_eventMap( int intNum, int eventId);

/* plug */
void Hwi_plug( unsigned int intNum, Hwi_PlugFuncPtr fxn);

/* getEventId */
int32_t Hwi_getEventId( unsigned int intNum);

/* disableIER */
unsigned long Hwi_disableIER( unsigned long mask);

/* enableIER */
unsigned long Hwi_enableIER( unsigned long mask);

/* restoreIER */
unsigned long Hwi_restoreIER( unsigned long mask);

/* setPriority */
void Hwi_setPriority( unsigned int intNum, unsigned int priority);

/* getCXM */
Hwi_TSR_CXM Hwi_getCXM( void);

/* setCOP */
void Hwi_setCOP( int32_t cop);

/* secureStart */
void Hwi_secureStart( void);

/* reconfig */
void Hwi_reconfig( Hwi_Handle instp, Hwi_FuncPtr fxn, const Hwi_Params *params);

/* getIsrStackAddress */
char *Hwi_getIsrStackAddress( void);

void Hwi_dispatchC( int32_t intNum);

/* dispatchCore */
void Hwi_dispatchCore( int32_t intNum);

/* unPluggedInterrupt */
void Hwi_unPluggedInterrupt( void);

/* switchAndDispatch */
void Hwi_switchAndDispatch( int32_t intNum);

/* postInit */
int32_t Hwi_postInit( Hwi_Object *hwi);


/* Params_init */
void Hwi_Params_init(Hwi_Params *prms);




unsigned int Hwi_disable(void);
unsigned int Hwi_enable(void);
void Hwi_restore(unsigned int key);





/*
 * ======== STATE STRUCTURES ========
 */


/* Module_State */
struct Hwi_Module_State {
    Hwi_Module_State_intEvents intEvents;
    unsigned long ierMask;
    volatile int32_t intNum;
    char *taskSP;
    char *isrStack;
    int32_t scw;
    Hwi_Module_State_dispatchTable dispatchTable;
};

/* Module__state__V */
extern struct Hwi_Module_State Hwi_Module_state;

/* Object */
struct Hwi_Object {
    unsigned long disableMask;
    unsigned long restoreMask;
    unsigned int arg;
    Hwi_FuncPtr fxn;
    int32_t intNum;
    int32_t priority;
    Hwi_Irp irp;
};




#ifdef __cplusplus
    } /* extern C */
#endif

#endif

