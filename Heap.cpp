/**
* @file		Heap.cpp
* @brief	Implementation of functions to work with heap
*
* @author	Yura Kaminskyi
* @date		Apr, 2015
*
*/


#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Heap.h"

using std::cout;
using std::endl;

void* Heap::MakeHeap(size_t sz)
{
	if (sz < MIN_HEAP_SIZE)
	{
		sz = MIN_HEAP_SIZE;
	}

	bottom = calloc(sz, 1);
	if (bottom == NULL)
	{
		return NULL;
	}
	top = (char*)bottom + sz;

	cacheOffset = sz / 10;

	shortMap = (char*)bottom;
	lastReleasedShort = NULL;

	intMap = shortMap + cacheOffset;
	lastReleasedInt = NULL;

	doubleMap = intMap + cacheOffset;
	lastReleasedDouble = NULL;

	heapBegin = doubleMap + cacheOffset;

	head = (BlockInfo*)(malloc(sizeof(BlockInfo)));
	if (head == NULL)
	{
		return NULL;
	}
	head->next = NULL;
	head->size = sz - 3 * cacheOffset;
	head->isBusy = FALSE;

	return bottom;
}

void* Heap::Alloc(size_t sz)
{
	void* alloc;
	if (sz == SHORT_SIZE)
	{
		alloc = ShortAlloc();
		if (!alloc)
		{
			alloc = HeapAlloc(SHORT_SIZE);
		}
	}
	else if (sz == INT_SIZE)
	{
		alloc = IntAlloc();
		if (!alloc)
		{
			alloc = HeapAlloc(INT_SIZE);
		}
	}
	else if (sz == DOUBLE_SIZE)
	{
		alloc = DoubleAlloc();
		if (!alloc)
		{
			alloc = HeapAlloc(DOUBLE_SIZE);
		}
	}
	else
	{
		alloc = HeapAlloc(sz);
	}
	return alloc;
}

void* Heap::ShortAlloc()
{
	void* alloc;
	// check if there's free memory from previous memory release
	if (lastReleasedShort)
	{
		*lastReleasedShort = USED_BLOCK;
		alloc = lastReleasedShort;
		lastReleasedShort = NULL;
	}
	// if there's not - find new free block
	else
	{
		char* ptr = (char*)shortMap;
		while (*ptr != FREE_BLOCK)
		{
			++ptr;
		}
		*ptr = USED_BLOCK;
		alloc = shortMap + (ptr - shortMap);
	}
	return alloc;
}

void* Heap::IntAlloc()
{
	void* alloc;
	// check if there's free memory from previous memory release
	if (lastReleasedInt)
	{
		*lastReleasedInt = USED_BLOCK;
		alloc = lastReleasedInt;
		lastReleasedInt = NULL;
	}
	// if there's not - find new free block
	else
	{
		char* ptr = (char*)intMap;
		while (*ptr != FREE_BLOCK)
		{
			++ptr;
		}
		*ptr = USED_BLOCK;
		alloc = intMap + (ptr - intMap);
	}
	return alloc;
}

void* Heap::DoubleAlloc()
{
	void* alloc;
	// check if there's free memory from previous memory release
	if (lastReleasedDouble)
	{
		*lastReleasedDouble = USED_BLOCK;
		alloc = lastReleasedDouble;
		lastReleasedDouble = NULL;
	}
	// if there's not - find new free block
	else
	{
		char* ptr = (char*)doubleMap;
		while (*ptr != FREE_BLOCK)
		{
			++ptr;
		}
		*ptr = USED_BLOCK;
		alloc = doubleMap + (ptr - doubleMap);
	}
	return alloc;
}

void* Heap::HeapAlloc(size_t sz)
{
	BlockInfo* ptr = head;
	size_t offset = 0;
	// find free block with proper size
	while (ptr)
	{
		if (!ptr->isBusy && ptr->size >= sz)
		{
			break;
		}
		else
		{
			offset += ptr->size;
			ptr = ptr->next;
		}
	}

	if (!ptr)
	{
		return NULL;
	}
	//if size fit exactly - allocate memory 
	if (ptr->size == sz)
	{
		ptr->isBusy = TRUE;
	}
	// allocate new service block and add it to servise list
	else
	{
		BlockInfo* newBlock = (BlockInfo*)(malloc(sizeof(BlockInfo)));
		if (newBlock == NULL)
		{
			return NULL;
		}
		newBlock->size = sz;
		newBlock->isBusy = TRUE;
		if (ptr == head)
		{
			newBlock->next = head;
			ptr->size -= sz;
			head = newBlock;
		}
		else
		{
			BlockInfo* prev = head;
			while (prev->next != ptr)
			{
				prev = prev->next;
			}
			prev->next = newBlock;
			newBlock->next = ptr;
			ptr->size -= sz;
		}
	}
	return heapBegin + offset;
}

void Heap::Dealloc(void* ptr)
{
	if (ptr < bottom || ptr > top )
	{
		return;
	}
	else if (ptr < intMap)
	{
		*(shortMap + ((char*)ptr - shortMap)) = FREE_BLOCK;
		lastReleasedShort = (char*)ptr;
	}
	else if (ptr < doubleMap)
	{
		*(intMap + ((char*)ptr - intMap)) = FREE_BLOCK;
		lastReleasedInt = (char*)ptr;
	}
	else if (ptr < doubleMap + cacheOffset)
	{
		*(doubleMap + ((char*)ptr - doubleMap)) = FREE_BLOCK;
		lastReleasedDouble = (char*)ptr;
	}
	else
	{
		size_t offset = (char*)ptr - heapBegin;
		BlockInfo* ptr = head;
		BlockInfo* temp;
		//find block to free
		while (ptr && offset >= ptr->size)
		{
			offset -= ptr->size;
			ptr = ptr->next;
		}

		ptr->isBusy = FALSE;
		// if previous block is free - merge it with current
		if (ptr != head)
		{
			BlockInfo* prev = head;
			while (prev->next != ptr)
			{
				prev = prev->next;
			}
				
			if (!prev->isBusy)
			{
				temp = ptr->next;
				prev->size += ptr->size;
				free(ptr);
				prev->next = temp;
			}
		}
		// if next block is free - merge it with current
		if (ptr->next && !ptr->next->isBusy)
		{										
			temp = ptr->next->next;
			ptr->size += ptr->next->size;
			free(ptr->next);
			ptr->next = temp;
		}
		
	}
}

void Heap::Map()
{
	
	ShortMap();
	IntMap();
	DoubleMap();
	HeapMap();
}

void Heap::HeapMap()
{
	cout << "Heap contents:" << endl;
	BlockInfo* ptr = head;
	size_t offset = 0;
	do
	{
		cout << long(heapBegin + offset) << "  " << ptr->size << "  ";
		if (ptr->isBusy == FALSE)
		{
			cout << "Free\t\t\t";
		}
		else
		{
			cout << "Busy\t\t\t";
		}
		offset += ptr->size;
		ptr = ptr->next;
	} 
	while (ptr);
	cout << endl;
}

void Heap::ShortMap()
{
	cout << "Short cache:" << endl;
	char* ptr = shortMap;
	while (ptr != intMap)
	{
		cout << (long)ptr << "  " << ptr - shortMap << "  ";
		if (*ptr == FREE_BLOCK)
		{
			cout << "Free\t\t\t";
		}
		else
		{
			cout << "Busy\t\t\t";
		}
		++ptr;
	}
	cout << endl;
}

void Heap::IntMap()
{
	cout << "Int cache:" << endl;
	char* ptr = intMap;
	while (ptr != doubleMap)
	{
		cout << (long)ptr << "  " << ptr - intMap << "  ";
		if (*ptr == FREE_BLOCK)
		{
			cout << "Free\t\t\t";
		}
		else
		{
			cout << "Busy\t\t\t";
		}
		++ptr;
	}
	cout << endl;
}

void Heap::DoubleMap()
{
	cout << "Double cache:" << endl;
	char* cacheEnd = doubleMap + cacheOffset;
	char* ptr = doubleMap;
	while (ptr != cacheEnd)
	{
		cout << (long)ptr << "  " << ptr - doubleMap << "  ";
		if (*ptr == FREE_BLOCK)
		{
			cout << "Free\t\t\t";
		}
		else
		{
			cout << "Busy\t\t\t";
		}
		++ptr;
	}
	cout << endl;
}

void Heap::DeleteHeap()
{
	free(bottom);
}
