;******************************************************************************
; SDL UTILS
;
; SafeTI Diagnostics Library Utils module
;
; Copyright (C) 2018-2019 Texas Instruments Incorporated - http://www.ti.com/
; ALL RIGHTS RESERVED
;
;******************************************************************************
;  @file sdl_utils.asm
;
;  @brief
;   Implementation file for the ARM R5 utils
;
;   Contains the different control command and status query functions definitions
;
;   \par
;******************************************************************************
    .text

;==============================================================================
;   Get Correctable Fault Location Register (CFLR) value
;==============================================================================
    .global SDL_UTILS_getCFLR
SDL_UTILS_getCFLR:
    MRC p15, #0, r0, c15, c3, #0
    BX      lr

;==============================================================================
;   Get Performance Monitors Overflow Flag Status Register (PMOVSR) value
;==============================================================================
    .global SDL_UTILS_getPMOVSR
SDL_UTILS_getPMOVSR:
    MRC p15, #0, r0, c9, c12, #3
    BX      lr

;==============================================================================
;   Get Data Fault Status Register (DFSR) value
;==============================================================================
    .global SDL_UTILS_getDFSR
SDL_UTILS_getDFSR:
    MRC p15, #0, r0, c5, c0, #0
    BX      lr

;==============================================================================
;   Get Data Fault Address Register (DFAR) value
;==============================================================================
    .global SDL_UTILS_getDFAR
SDL_UTILS_getDFAR:
    MRC p15, #0, r0, c6, c0, #0
    BX      lr

;==============================================================================
;   Get Instruction Fault Status Register Register (IFSR) value
;==============================================================================
    .global SDL_UTILS_getIFSR
SDL_UTILS_getIFSR:
    MRC p15, #0, r0, c5, c0, #1
    BX      lr

;==============================================================================
;   Get Instruction Fault Address Register Register (IFAR) value
;==============================================================================
    .global SDL_UTILS_getIFAR
SDL_UTILS_getIFAR:
    MRC p15, #0, r0, c6, c0, #2
    BX      lr
