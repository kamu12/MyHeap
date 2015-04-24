/**
* @file		Source.cpp
* @brief	File for testing my heap
*
* @author	Yura Kaminskyi
* @date		Apr, 2015
*
*/

#include <iostream>
#include "Heap.h"

using std::cout;
using std::endl;

#define SEP "********************************"

int main()
{
	Heap h;
	h.MakeHeap(100);

	// short cache test
	/*short* i = (short*)h.Alloc(sizeof(short));

	short* k = (short*)h.Alloc(sizeof(short));

	short* j = (short*)h.Alloc(sizeof(short));

	h.Dealloc(k);
	h.Dealloc(i);
	h.ShortMap();

	short* s = (short*)h.Alloc(sizeof(short));
	cout << (long)s << endl;
	h.ShortMap();

	short* w = (short*)h.Alloc(sizeof(short));
	cout << (long)w << endl;
	h.ShortMap();

	short* e = (short*)h.Alloc(sizeof(short));
	cout << (long)e << endl;
	h.ShortMap();*/
	// int cache test
	/*int* z = (int*)h.Alloc(sizeof(int));

	int* x = (int*)h.Alloc(sizeof(int));

	int* c = (int*)h.Alloc(sizeof(int));

	h.Dealloc(x);
	h.Dealloc(z);
	h.IntMap();

	int* v = (int*)h.Alloc(sizeof(int));
	cout << (long)v << endl;
	h.IntMap();

	int* b = (int*)h.Alloc(sizeof(int));
	cout << (long)b << endl;
	h.IntMap();

	int* n = (int*)h.Alloc(sizeof(int));
	cout << (long)n << endl;*/
	h.IntMap();
	// double cache test
	/*double* m = (double*)h.Alloc(sizeof(double));

	double* l = (double*)h.Alloc(sizeof(double));

	double* g = (double*)h.Alloc(sizeof(double));

	h.Dealloc(l);
	h.Dealloc(m);
	h.DoubleMap();

	double* sp = (double*)h.Alloc(sizeof(double));
	cout << (long)sp << endl;
	h.DoubleMap();

	double* wa = (double*)h.Alloc(sizeof(double));
	cout << (long)wa << endl;
	h.DoubleMap();

	double* ew = (double*)h.Alloc(sizeof(double));
	cout << (long)ew << endl;
	h.DoubleMap();*/
	// uncached  test
	h.HeapMap();
	void * t = h.Alloc(20);
	void * p = h.Alloc(10);
	void * r = h.Alloc(40);
	h.HeapMap();
	h.Dealloc(p);
	h.HeapMap();
	h.Dealloc(t);
	h.HeapMap();

	cout << SEP << endl;
	h.Map();

	h.DeleteHeap();

	system("pause>>void");
	return 0;
}

