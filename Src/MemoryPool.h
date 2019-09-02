/****************************************************************************************************************************
Name: MemoryPool.h.
Author: 叶伟(Lareyd).
Copyright: Copyright (C) 2018 - 2018, 叶伟(Lareyd). All rights reserved.
Version: 1.0.
Description: Declaration of MemoryPool settings.
****************************************************************************************************************************/


#ifndef __MEMORY_POOL_H__
#define __MEMORY_POOL_H__


#include <stdint.h>

#include <map>


namespace Memory
{
	class MemoryPool
	{
	private:
		typedef struct _MemoryElement
		{
			uint32_t beginAddress;
			uint32_t endAddress;
			uint32_t size;
		} MEMORY_ELEMENT, *PMEMORY_ELEMENT;


	public:
		uint32_t extensionSize;// Size when memory pool to extend each time, in bytes.

	private:
		bool valid;// Whether valid.

	private:
		void* memory;
		uint32_t size;

		std::map<void*, MEMORY_ELEMENT, std::less<void*>> memoryInfo;// key: memory address returned from Obtain.

	public:
		// size：Memory pool size at creation, in bytes.
		// extensionSize：Size when memory pool to extend each time, in bytes.
		MemoryPool(uint32_t size, uint32_t extensionSize);

		~MemoryPool();

	public:
		// Whether valid.
		bool IsValid();

	public:
		// Obtain a memory block of size.
		// Return; Not NULL means success. NULL if valid is false, or input is zero, or failed to obtain.
		void* Obtain(uint32_t size);

		// Recycle memory, address was returned by Obtain.
		void Recycle(void* address);

		// Recycle all memory.
		void RecycleAll();

	public:
		// Release all memory pool resources, pool would be invalid. Memory pool manages application and release of resources.
		// generally, call this method when system is lacking memory.
		void Clear();

	private:
		// appliedSize：Applied memory size.
		// usedSize：Used memory pool size plus all the gap sizes.
		void* AddMemoryElement(uint32_t appliedSize, uint32_t usedSize);

		// appliedSize：Applied memory size.
		// usedSize：Used memory pool size plus all the gap sizes.
		void* AppendMemoryElement(uint32_t appliedSize, uint32_t usedSize);
	};
}


#endif// __MEMORY_POOL_H__