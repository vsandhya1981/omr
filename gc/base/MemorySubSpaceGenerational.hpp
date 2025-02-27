/*******************************************************************************
 * Copyright (c) 1991, 2022 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/


#if !defined(MEMORYSUBSPACEGENERATIONAL_HPP_)
#define MEMORYSUBSPACEGENERATIONAL_HPP_

#include "omrcfg.h"

#if defined(OMR_GC_MODRON_SCAVENGER)

#include "MemorySubSpace.hpp"

class MM_AllocateDescription;
class MM_EnvironmentBase;
class MM_MemoryPool;
class MM_MemorySpace;
class MM_MemorySubSpace;
class MM_ObjectAllocationInterface;

/**
 * @todo Provide class documentation
 * @ingroup GC_Base
 */
class MM_MemorySubSpaceGenerational : public MM_MemorySubSpace
{
	/*
	 * Data members
	 */
private:
	uintptr_t _initialSizeNew;
	uintptr_t _minimumSizeNew;
	uintptr_t _maximumSizeNew;
	uintptr_t _initialSizeOld;
	uintptr_t _minimumSizeOld;
	uintptr_t _maximumSizeOld;

	MM_MemorySubSpace *_memorySubSpaceNew;
	MM_MemorySubSpace *_memorySubSpaceOld;

protected:
public:
	
	/*
	 * Function members
	 */
private:
	MMINLINE uintptr_t getInitialSizeNew() { return _initialSizeNew; };
	MMINLINE uintptr_t getInitialSizeOld() { return _initialSizeOld; };
	MMINLINE uintptr_t getMinimumSizeNew() { return _minimumSizeNew; };
	MMINLINE uintptr_t getMinimumSizeOld() { return _minimumSizeOld; };
	MMINLINE uintptr_t getMaximumSizeNew() { return _maximumSizeNew; };
	MMINLINE uintptr_t getMaximumSizeOld() { return _maximumSizeOld; };

	MMINLINE MM_MemorySubSpace *getMemorySubSpaceNew() { return _memorySubSpaceNew; };
	MMINLINE MM_MemorySubSpace *getMemorySubSpaceOld() { return _memorySubSpaceOld; };
	MMINLINE void setMemorySubSpaceNew(MM_MemorySubSpace *memorySubSpace) { _memorySubSpaceNew = memorySubSpace; };
	MMINLINE void setMemorySubSpaceOld(MM_MemorySubSpace *memorySubSpace) { _memorySubSpaceOld = memorySubSpace; };

	bool initialize(MM_EnvironmentBase *env);
	virtual void tearDown(MM_EnvironmentBase *env);

protected:
	virtual void *allocationRequestFailed(MM_EnvironmentBase *env, MM_AllocateDescription *allocateDescription, AllocationType allocationType, MM_ObjectAllocationInterface *objectAllocationInterface, MM_MemorySubSpace *baseSubSpace, MM_MemorySubSpace *previousSubSpace);

public:
	static MM_MemorySubSpaceGenerational *newInstance(MM_EnvironmentBase *env, MM_MemorySubSpace *memorySubSpaceNew, MM_MemorySubSpace *memorySubSpaceOld, bool usesGlobalCollector, uintptr_t minimumSize, uintptr_t minimumSizeNew, uintptr_t initialSizeNew, uintptr_t maximumSizeNew, uintptr_t minimumSizeOld, uintptr_t initialSizeOld, uintptr_t maximumSizeOld, uintptr_t maximumSize);

	virtual const char *getName() { return MEMORY_SUBSPACE_NAME_GENERATIONAL; }
	virtual const char *getDescription() { return MEMORY_SUBSPACE_DESCRIPTION_GENERATIONAL; }

	virtual void *allocateObject(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, MM_MemorySubSpace *baseSubSpace, MM_MemorySubSpace *previousSubSpace, bool shouldCollectOnFailure);
	virtual void *allocateArrayletLeaf(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, MM_MemorySubSpace *baseSubSpace, MM_MemorySubSpace *previousSubSpace, bool shouldCollectOnFailure);
	
	virtual void *allocateTLH(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, MM_ObjectAllocationInterface *objectAllocationInterface, MM_MemorySubSpace *baseSubSpace, MM_MemorySubSpace *previousSubSpace, bool shouldCollectOnFailure);

	/* Calls for internal collection routines */
	virtual void abandonHeapChunk(void *addrBase, void *addrTop);

	virtual MM_MemorySubSpace *getDefaultMemorySubSpace();
	virtual MM_MemorySubSpace *getTenureMemorySubSpace();

	virtual void checkResize(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription, bool _systemGC);
	virtual intptr_t performResize(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription);
	virtual uintptr_t getAvailableContractionSize(MM_EnvironmentBase *env, MM_AllocateDescription *allocDescription);

	virtual uintptr_t counterBalanceContract(MM_EnvironmentBase *env, MM_MemorySubSpace *previousSubSpace, MM_MemorySubSpace *contractSubSpace, uintptr_t contractSize, uintptr_t contractAlignment);

	virtual uintptr_t releaseFreeMemoryPages(MM_EnvironmentBase* env);

	virtual MMINLINE uintptr_t getContractionSize() const { return _memorySubSpaceOld->getContractionSize(); }

	/**
	 * Create a MemorySubSpaceGenerational object.
	 */
	MM_MemorySubSpaceGenerational(MM_EnvironmentBase *env, MM_MemorySubSpace *memorySubSpaceNew, MM_MemorySubSpace *memorySubSpaceOld, bool usesGlobalCollector, uintptr_t minimumSize, uintptr_t minimumSizeNew, uintptr_t initialSizeNew, uintptr_t maximumSizeNew, uintptr_t minimumSizeOld, uintptr_t initialSizeOld, uintptr_t maximumSizeOld, uintptr_t maximumSize) :
		MM_MemorySubSpace(env, NULL, NULL, usesGlobalCollector, minimumSize, initialSizeNew + initialSizeOld, maximumSize, MEMORY_TYPE_OLD, 0),
		_initialSizeNew(initialSizeNew),
		_minimumSizeNew(minimumSizeNew),
		_maximumSizeNew(maximumSizeNew),
		_initialSizeOld(initialSizeOld),
		_minimumSizeOld(minimumSizeOld),
		_maximumSizeOld(maximumSizeOld),
		_memorySubSpaceNew(memorySubSpaceNew), 
		_memorySubSpaceOld(memorySubSpaceOld)
	{
		_typeId = __FUNCTION__;
	}
};

#endif /* OMR_GC_MODRON_SCAVENGER */

#endif /* MEMORYSUBSPACEGENERATIONAL_HPP_ */
