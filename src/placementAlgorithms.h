#ifndef PLACEMENT_ALGORITHMS_H
#define PLACEMENT_ALGORITHMS_H

#include <string>
#include <list>
using namespace std;

struct mem_block
{
/*
	This struct is used to record a memory block.

	Members:
		@start: the start address of the memory block
		@size: size of the memory block in MB
		@proc: process ID, only meaningful for used block
*/
	int	address;	
	int	size;	
	int	proc;	
};

extern list<mem_block> free_blocks;	//a list of all free memory blocks
extern list<mem_block> used_blocks;	//a list of all used memory blocks

enum alloc_algorithm { first_fit, next_fit, best_fit, worst_fit };

const int ALLOC_ERROR = -1;
const int FREE_ERROR = -2;

/* initialize the memory infomation (start address and total available size) */
void init_memory(int size, int start_address=0);

/* request memory */
int alloc_memory(alloc_algorithm algorithm, int proc, int size);

/* release memory */
int free_memory(int proc);

/* comparison functions used to sort a list of mem_block in different ways */
bool sort_by_address (mem_block& first, mem_block& second);
bool sort_by_size_asceding (mem_block& first, mem_block& second);
bool sort_by_size_desceding (mem_block& first, mem_block& second);

#endif
