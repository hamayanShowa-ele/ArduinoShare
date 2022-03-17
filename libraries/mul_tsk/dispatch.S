/* ----------------------------------------
 not pre-empt multi task system dispatcher for cortex M3.

  Copyright (c) 2020 hamayan (hamayan.contact@gmail.com).
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Created 2020 by hamayan (hamayan.contact@gmail.com)
---------------------------------------- */

#if  defined( STM32L0xx )
  .arch armv6-m
  .cpu cortex-m0plus
//  .fpu softvfp  /* Software floating decimal point linkage specification. */
#elif  defined( STM32F1xx )
  .arch armv7-m
  .cpu cortex-m3
//  .fpu softvfp  /* Software floating decimal point linkage specification. */
#elif  defined( STM32F4xx ) || defined( STM32L4xx )
  .arch armv7e-m
  .cpu cortex-m4
//  .fpu softvfp  /* Software floating decimal point linkage specification. */
#endif  /* defined( STM32L0xx ) */
  .syntax unified  /* It is written in the unified assembly syntax. */
  .thumb  /* Interpret subsequent instructions as T32. */
  .text

  .thumb_func
  .global swi_ctx /* Switch execution context */
  .global sta_ctx /* Transition the task to READY. */
  .type	swi_ctx,%function	@LSB=1
  .type	sta_ctx,%function	@LSB=1

/*
  void sta_ctx( void *exe )
  Transition the task to READY.
  The argument exe(R0) is the destination of the execution context.
*/
sta_ctx:
#if  defined( STM32L0xx )
  PUSH {LR}
  LDR  R2,[R0]
  MOV  R13,R2
  POP  {R4,R5,R6,R7}
//  POP  {R2}
//  MOV  R4,R2  // pop r4
//  POP  {R2}
//  MOV  R5,R2  // pop r5
//  POP  {R2}
//  MOV  R6,R2  // pop r6
//  POP  {R2}
//  MOV  R7,R2  // pop r7
  POP  {R2}
  MOV  R8,R2  // pop r8
  POP  {R2}
  MOV  R9,R2  // pop r9
  POP  {R2}
  MOV  R10,R2  // pop r10
  POP  {R2}
  MOV  R11,R2  // pop r11
  POP  {R2}
  MOV  R12,R2  // pop r12
  POP  {PC}  // pop pc
#elif  defined( STM32F1xx ) || defined( STM32F4xx ) || defined( STM32L4xx )
  PUSH {LR}
  LDR  R13,[R0]
  POP  {R4-R12,PC}
#endif  /* defined( STM32L0xx ) */

/*
  void swi_ctx( void *pre, void *post)
  Switch execution context.
  The argument pre(R0) is the destination of the current context.
  The argument post(R1) is the source of the context to switch to.
*/
swi_ctx:
#if  defined( STM32L0xx )
  PUSH {LR}
  MOV  R2,R12  // push r12
  PUSH {R2}
  MOV  R2,R11  // push r11
  PUSH {R2}
  MOV  R2,R10  // push r10
  PUSH {R2}
  MOV  R2,R9  // push r9
  PUSH {R2}
  MOV  R2,R8  // push r8
  PUSH {R2}  //
//  MOV  R2,R7  // push r7
//  PUSH {R2}
//  MOV  R2,R6  // push r6
//  PUSH {R2}
//  MOV  R2,R5  // push r5
//  PUSH {R2}
//  MOV  R2,R4  // push r4
//  PUSH {R2}  //
//  PUSH  {R7,R6,R5,R4}
  PUSH  {R4,R5,R6,R7}
  MOV  R2,R13
  STR  R2,[R0]
#elif  defined( STM32F1xx ) || defined( STM32F4xx ) || defined( STM32L4xx )
  PUSH {R4-R12,LR}
  STR  R13,[R0]
#endif  /* defined( STM32L0xx ) */

#if  defined( STM32L0xx )
  LDR  R2,[R1]
  MOV  R13,R2
  POP  {R4,R5,R6,R7}
//  POP  {R2}  //
//  MOV  R4,R2  // pop r4
//  POP  {R2}  //
//  MOV  R5,R2  // pop r5
//  POP  {R2}  //
//  MOV  R6,R2  // pop r6
//  POP  {R2}  //
//  MOV  R7,R2  // pop r7
  POP  {R2}  //
  MOV  R8,R2  // pop r8
  POP  {R2}  //
  MOV  R9,R2  // pop r9
  POP  {R2}  //
  MOV  R10,R2  // pop r10
  POP  {R2}  //
  MOV  R11,R2  // pop r11
  POP  {R2}  //
  MOV  R12,R2  // pop r12
  POP  {PC}  // pop pc
#elif  defined( STM32F1xx ) || defined( STM32F4xx ) || defined( STM32L4xx )
  LDR  R13,[R1]
  POP  {R4-R12,PC}
#endif  /* defined( STM32L0xx ) */

  .end
 
