;******************************************************************************
;*                                                                            *
;* Copyright (c) 2020 Texas Instruments Incorporated                          *
;* http://www.ti.com/                                                         *
;*                                                                            *
;*  Redistribution and  use in source  and binary forms, with  or without     *
;*  modification,  are permitted provided  that the  following conditions     *
;*  are met:                                                                  *
;*                                                                            *
;*     Redistributions  of source  code must  retain the  above copyright     *
;*     notice, this list of conditions and the following disclaimer.          *
;*                                                                            *
;*     Redistributions in binary form  must reproduce the above copyright     *
;*     notice, this  list of conditions  and the following  disclaimer in     *
;*     the  documentation  and/or   other  materials  provided  with  the     *
;*     distribution.                                                          *
;*                                                                            *
;*     Neither the  name of Texas Instruments Incorporated  nor the names     *
;*     of its  contributors may  be used to  endorse or  promote products     *
;*     derived  from   this  software  without   specific  prior  written     *
;*     permission.                                                            *
;*                                                                            *
;*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS     *
;*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT     *
;*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR     *
;*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT     *
;*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,     *
;*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT     *
;*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     *
;*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY     *
;*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT     *
;*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE     *
;*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.      *
;*                                                                            *
;******************************************************************************
	.arm


;****************************************************************************
; SBL Error Loop
;****************************************************************************
	.sect   ".text"
	.global SblErrLoop
SblErrLoop:
	.asmfunc
	B	SblErrLoop
	.endasmfunc


;****************************************************************************
; Supress Klockworks Ptr Errors
;****************************************************************************
	.sect   ".text"
	.global uint32_to_void_ptr
	.global uint32_to_const_void_ptr
	.global uint32_to_uint32_ptr
	.global uint32_ptr_to_void_ptr
	.global const_uint8_ptr_to_void_ptr
	.global uint32_to_int32
	.global uint64_to_uint32
	.global uint64_to_int32

uint32_to_int32:
uint32_to_void_ptr:
uint32_to_const_void_ptr:
uint32_to_uint32_ptr:
uint32_ptr_to_void_ptr:
const_uint8_ptr_to_void_ptr:
uint64_to_uint32:
uint64_to_int32:

	.asmfunc

	BX	lr	
	.endasmfunc

