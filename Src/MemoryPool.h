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
	public:
		enum ByteAlignment
		{
			_1Byte,
			_2Byte,
			_4Byte,
			_8Byte,
			_16Byte,
			_32Byte,
			_64Byte
		};

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
		ByteAlignment byteAlignment;

		std::map<void*, MEMORY_ELEMENT, std::less<void*>> memoryInfo;// key: memory address returned from Obtain.

	public:
		// size; Memory pool size at creation, in bytes.
		// extensionSize; Size when memory pool to extend each time, in bytes.
		// byteAlignment: Byte alignment.
		MemoryPool(uint32_t size, uint32_t extensionSize, ByteAlignment byteAlignment = _4Byte);

		~MemoryPool();

	public:
		// Whether valid.
		bool IsValid();

	public:
		// Obtain a memory block of size.
		// size: Size of memory to obtain. The result is byte aligned.
		// Return; Not NULL means success. NULL if valid is false, or input is zero, or failed to obtain.
		void* Obtain(uint32_t size);

		// Set memory pool byte alignment.
		void SetByteAlignment(ByteAlignment byteAlignment);

		// Recycle memory, address was returned by Obtain.
		void Recycle(void* address);

		// Recycle all memory.
		void RecycleAll();

	public:
		// Release all memory pool resources, pool would be invalid. Memory pool manages application and release of resources.
		// generally, call this method when system is lacking memory.
		void Clear();

	private:
		// appliedSize: Applied memory size.
		// usedSize: Used memory pool size plus all the gap sizes.
		void* AddMemoryElement(uint32_t appliedSize, uint32_t usedSize);

		// appliedSize: Applied memory size.
		// usedSize: Used memory pool size plus all the gap sizes.
		void* AppendMemoryElement(uint32_t appliedSize, uint32_t usedSize);
	};
}


#endif// __MEMORY_POOL_H__