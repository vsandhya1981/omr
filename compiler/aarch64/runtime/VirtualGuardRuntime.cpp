/*******************************************************************************
 * Copyright (c) 2019, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include <stdint.h>
#include "codegen/ARM64Instruction.hpp"
#include "codegen/InstOpCode.hpp"
#include "env/CompilerEnv.hpp"
#include "env/jittypes.h"
#include <infra/Assert.hpp>

#if defined(OSX)
#include <pthread.h> // for pthread_jit_write_protect_np
#endif

extern void arm64CodeSync(unsigned char *codeStart, unsigned int codeSize);

extern "C" void _patchVirtualGuard(uint8_t *locationAddr, uint8_t *destinationAddr, int32_t smpFlag)
   {
   int64_t distance = (int64_t)destinationAddr - (int64_t)locationAddr;

#if defined(OSX)
   pthread_jit_write_protect_np(0);
#endif
   *(uint32_t *)locationAddr = TR::InstOpCode::getOpCodeBinaryEncoding(TR::InstOpCode::b) | ((distance >> 2) & 0x3ffffff); /* imm26 */
   arm64CodeSync((unsigned char *)locationAddr, ARM64_INSTRUCTION_LENGTH);
#if defined(OSX)
   pthread_jit_write_protect_np(1);
#endif
   }
