#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

using namespace std;

#define ENTITY_SIZE 100
#define HEAP_SIZE 1024
#define HEADER 4

uint16_t IN_USE = 0;

typedef struct virtual_memory {
	uint8_t heap[HEAP_SIZE];
}virtual_memory_t;

typedef struct entity {
	uint8_t* ptr;
	int size;
}entity_t;

entity_t LIST[ENTITY_SIZE];

void LOG()
{
	printf("OUR LIST\n");
	for (unsigned i = 0; i < IN_USE; i++)
		printf("Data + HEADER. [%p]. Memory of our heap free: [%u]\n", LIST[i].ptr, LIST[i].size);
	printf("Entities in use:[%d]\n", IN_USE);
}

entity_t* new_entity(size_t size) {
	if (LIST[0].ptr == NULL && LIST[0].size == 0) {
		static virtual_memory vm;
		LIST[0].ptr = vm.heap;
		LIST[0].size = HEAP_SIZE;
		IN_USE++;
		LOG();
	}
	entity_t* best = LIST;
	for (unsigned i = 0; i < IN_USE; i++)
		if (LIST[i].size >= size && LIST[i].size < best->size)
			best = &LIST[i];
	return best;
}

void* w_malloc(size_t size) {
	size += HEADER;
	assert(size <= HEAP_SIZE);
	entity_t* e = new_entity(size);
	uint8_t* start = e->ptr;
	uint8_t* user_ptr = start + HEADER;
	*start = size;
	e->ptr += size;
	e->size -= size;
	assert(e->size >= 0);
	LOG();
	return user_ptr;
}

void w_free(void* ptr) {
	uint8_t* start = (uint8_t*)ptr - HEADER;
	entity_t *prev = 0, *next = 0;
	for (size_t i = 0; i < IN_USE; i++) {
		if (LIST[i].ptr + LIST[i].size == start) prev = &LIST[i];
		if (start + *start == LIST[i].ptr) next = &LIST[i];
	}
	if (next) {
		next->ptr = start;
		next->size += *start;
		*start = next->size;
		IN_USE--;
	}
	if (prev) {
		prev->size += *start;
		if (next)
		{
			next->ptr = LIST[IN_USE].ptr;
			next->size = LIST[IN_USE].size;
		}
		IN_USE--;
	}
	if (!(prev || next)) {
		LIST[IN_USE].ptr = start;
		LIST[IN_USE].size = *start;
	}
	IN_USE++;
	LOG();
}

void* w_realloc(void* ptr, size_t size) {
	uint8_t* new_ptr = (uint8_t*)w_malloc(size);
	for (size_t i = 0; i < size; i++) 
		new_ptr[i] = 0;
	for (size_t i = 0; i < size && i < *(int*)((uint8_t*)ptr - HEADER) - HEADER; i++)
		new_ptr[i] = ((uint8_t*)ptr)[i];
	w_free(ptr);
	return new_ptr;
}

void test() {
	typedef struct foo {
		int a;
		int b;
	};

	foo* mda;
	int* bazz;

	mda = (foo*)w_malloc(sizeof(foo));
	bazz = (int*)w_malloc(sizeof(int));

	mda->a = 5;
	mda->b = 10;

	*bazz = mda->b;

	printf("Address: [%p], data: [%d] [%d]\n", mda, mda->a, mda->b);
	printf("Address: [%p], data: [%d] \n", bazz, *bazz);

	w_free(bazz);

	char* other = (char*)w_malloc(4);

	mda = (foo*)w_realloc(mda, 3 * sizeof(foo));

	for (int i = 0; i < 3; i++) 
		printf("Address: [%p], data: [%d] [%d]\n", mda + i, mda[i].a, mda[i].b);

	char* d = (char*)w_malloc(5);

//	w_free(mda);
}

int main(int argc, char** argv) {
	test();
	return 0;
}