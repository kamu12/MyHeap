/**
* @file		Heap.h
* @brief	Declaration of functions to work with heap
*
* @author	Yura Kaminskyi
* @date		Apr, 2015
*
*/

#ifndef _HEAP_H_
#define _HEAP_H_

#define MIN_HEAP_SIZE 256

#define FREE_BLOCK 0x00U
#define USED_BLOCK 0xFFU

#define SHORT_SIZE 2
#define INT_SIZE 4
#define DOUBLE_SIZE 8

#define TRUE 1
#define FALSE 0

struct BlockInfo
{
	BlockInfo* next;
	int size : 31;
	int isBusy : 1;
};

struct Heap
{	
	BlockInfo* head;

	void* bottom;
	void* top;
		
	char* shortMap;
	char* lastReleasedShort;

	char* intMap;
	char* lastReleasedInt;

	char* doubleMap;
	char* lastReleasedDouble;

	size_t cacheOffset;
	char* heapBegin;

	/**
	* @brief		Initialise all necessary variables to work with heap
	* @param		[in]	size of heap to initialize
	* @return		void*	pointer to heap begin
	* @warning		In case to increase efficiency size of heap should be eqal or greater
					than MIN_HEAP_SIZE; if parameter is less than MIN_HEAP_SIZE, heap
					size will be MIN_HEAP_SIZE
	*/
	void* MakeHeap(size_t size);

	/**
	* @brief		Delete heap
	* @return		void
	*/
	void DeleteHeap();

	/**
	* @brief		This function allocates memory in my heap
	* @param		[in]	number of bytes to be allocated
	* @return		void*	pointer to allocated memory
	*/
	void* Alloc(size_t sz);

	/**
	* @brief		This function allocates memory in two-sized cache of heap
	* @return		void*	pointer to allocated cache memory
	*/
	void* ShortAlloc();

	/**
	* @brief		This function allocates memory in four-sized cache of heap
	* @return		void*	pointer to allocated cache memory
	*/
	void* IntAlloc();

	/**
	* @brief		This function allocates memory in eight-sized cache of heap
	* @return		void*	pointer to allocated cache memory
	*/
	void* DoubleAlloc();

	/**
	* @brief		This function allocates memory in uncached part of my heap
	* @param		[in]	number of bytes to be allocated
	* @return		void*	pointer to allocated memory
	*/
	void* HeapAlloc(size_t sz);

	/**
	* @brief		This function free memory in my heap
	* @param		[in]	pointer to memory to free
	* @return		void
	*/
	void Dealloc(void* ptr);

	/**
	* @brief		This function prints state of my heap, including cached and uncached
					parts
	* @return		void
	*/
	void Map();

	/**
	* @brief		This function prints state of uncached part of my heap
	* @return		void
	*/
	void HeapMap();

	/**
	* @brief		This function prints state of two-size cache of my heap
	* @return		void
	*/
	void ShortMap();

	/**
	* @brief		This function prints state of four-size cache of my heap
	* @return		void
	*/
	void IntMap();

	/**
	* @brief		This function prints state of eight-size cache of my heap
	* @return		void
	*/
	void DoubleMap();
};

#endif /*_HEAP_H_*/
