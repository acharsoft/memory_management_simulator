#ifndef PLACEMENT_ALGORITHMS_H
#define PLACEMENT_ALGORITHMS_H

#include <string>
#include <list>
using namespace std;

enum alloc_algorithm { first_fit, next_fit, best_fit, worst_fit };

/*
	This struct is used to record a memory block.

	Members:
		@start: the start address of the memory block
		@size: size of the memory block in MB
		@proc: process ID, only meaningful for used block
*/
struct mem_block{
	int	address;	
	int	size;	
	int	proc;	
	mem_block(int ad = 0, int sz = 0, int p = 0):address(ad), size(sz), proc(p){}
};

class Memory{
private:
	list<mem_block> free_blocks;	// a list of free memory blocks.
	list<mem_block> used_blocks;	// a list of used memory blocks.
	int prev_alloc_block;			// the start address of the last allocation (allocated to the latest process), 
									// only useful for next_fit algorithm
	int max_memory_size;		// maximum available memory size

public:
	static const int ALLOC_ERROR = -1;
	static const int FREE_ERROR = -2;

	/* initialize the memory infomation (start address and total available size) */
	Memory(int size, int start_address=0);

	/* request memory */
	int alloc_memory(alloc_algorithm algorithm, int proc, int size);

	/* release memory */
	int free_memory(int proc);
	
	string to_string( );

private:	
	/* comparison functions used to sort a list of mem_block in different ways */
	static bool sort_by_address (mem_block& first, mem_block& second);
	static bool sort_by_size_asceding (mem_block& first, mem_block& second);
	static bool sort_by_size_desceding (mem_block& first, mem_block& second);
};
#endif
