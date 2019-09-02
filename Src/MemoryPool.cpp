/****************************************************************************************************************************
Name: MemoryPool.h.
Author: 叶伟(Lareyd).
Copyright: Copyright (C) 2018 - 2018, 叶伟(Lareyd). All rights reserved.
Version: 1.0.
Description: Implementation of MemoryPool settings.
****************************************************************************************************************************/


#include "MemoryPool.h"


namespace Memory
{
	MemoryPool::MemoryPool(uint32_t size, uint32_t extensionSize) : valid(false)
	{
		if (0 == size
			|| extensionSize == 0)
		{
			return;
		}

		memory = malloc(size);
		if (NULL == memory)
		{
			return;
		}
		
		valid = true;

		this->extensionSize = extensionSize;

		this->size = size;
	}

	MemoryPool::~MemoryPool()
	{
		Clear();
	}

	bool MemoryPool::IsValid()
	{
		return valid;
	}

	void* MemoryPool::Obtain(uint32_t size)
	{
		if (!valid
			|| 0 == size)
		{
			return NULL;
		}

		if (0 == memoryInfo.size())
		{
			return AppendMemoryElement(size, 0);
		}

		if (1 == memoryInfo.size())
		{
			return AppendMemoryElement(size, memoryInfo.begin()->second.size);
		}

		uint32_t usedAndGapSize = 0;
		void* result = NULL;
		for (std::map<void*, MEMORY_ELEMENT, std::less<void*>>::iterator iterator = memoryInfo.begin(), end = memoryInfo.end(), next;
			iterator != end; ++iterator)
		{
			usedAndGapSize += iterator->second.size;// add current memory block size

			next = iterator;
			++next;
			if (end == next)// reach last memory block
			{
				result = AppendMemoryElement(size, usedAndGapSize);
				break;
			}

			uint32_t gapSize = next->second.beginAddress - iterator->second.endAddress - 1;
			if (gapSize >= size)// memory block gap size is enough
			{
				result = AddMemoryElement(size, usedAndGapSize);
				break;
			}

			usedAndGapSize += gapSize;// add current memory block gap size
		}

		return result;
	}

	void MemoryPool::Recycle(void* address)
	{
		if (!valid
			|| memoryInfo.end() == memoryInfo.find(address))
		{
			return;
		}
		
		size -= memoryInfo[address].size;
		memoryInfo.erase(address);
	}

	void MemoryPool::RecycleAll()
	{
		if (!valid)
		{
			return;
		}

		size = 0;
		memoryInfo.clear();
	}

	void MemoryPool::Clear()
	{
		if (!valid)
		{
			return;
		}

		valid = false;

		extensionSize = 0;

		memoryInfo.clear();

		size = 0;
		if (NULL != memory)
		{
			free(memory);
			memory = NULL;
		}
	}

	void* MemoryPool::AddMemoryElement(uint32_t appliedSize, uint32_t usedSize)
	{
		void* obtainedMemory = (uint8_t*)memory + usedSize;
		MEMORY_ELEMENT memoryElement = { usedSize, usedSize + appliedSize - 1, appliedSize };
		memoryInfo.insert(std::pair<void*, MEMORY_ELEMENT>(obtainedMemory, memoryElement));

		return obtainedMemory;
	}

	void* MemoryPool::AppendMemoryElement(uint32_t appliedSize, uint32_t usedSize)
	{
		if (size - usedSize < appliedSize)// 无足够内存余额
		{
			void* newMemory = realloc(memory, size + appliedSize + extensionSize);
			if (NULL == newMemory)
			{
				return NULL;
			}

			memory = newMemory;
			size += appliedSize + extensionSize;
		}

		void* obtainedMemory = (uint8_t*)memory + usedSize;
		MEMORY_ELEMENT memoryElement = { usedSize, usedSize + appliedSize - 1, appliedSize };
		memoryInfo.insert(std::pair<void*, MEMORY_ELEMENT>(obtainedMemory, memoryElement));

		return obtainedMemory;
	}
}