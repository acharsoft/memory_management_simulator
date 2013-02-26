#include "placementAlgorithms.h"

list<mem_block> free_blocks;	// a list of free memory blocks.
list<mem_block> used_blocks;	// a list of used memory blocks.
static int prev_alloc_block;	// the start address of the last allocation (allocated to the latest process), 
								// only useful for next_fit algorithm
static int max_memory_size = 0;	// maximum available memory size


bool sort_by_address (mem_block& first, mem_block& second)
{
	return first.address<second.address;
}

bool sort_by_size_asceding (mem_block& first, mem_block& second)
{
	return first.size<second.size;
}

bool sort_by_size_desceding (mem_block& first, mem_block& second)
{
	return first.size>second.size;
}


/*
	Allocate memory to specified process based on the passed-in algorithm.

	Parameters: 
		@algorithm: the placement algorithm adpoted. (first_fit or next_fit or best_fit or worst_fit).
		@proc: process ID.
		@size: the size of memory requested by the process.

	Return value:
		0 is returned if success, ALLOC_ERROR otherwise.
*/
int alloc_memory(alloc_algorithm algorithm, int proc, int size)
{
	if((size > max_memory_size) || (size < 0)) 
		return ALLOC_ERROR;
	
	// sort free_blocks before searching
	list<mem_block>::iterator it;
	switch(algorithm) {

		case first_fit: 
			free_blocks.sort(sort_by_address);
			break;

		case next_fit: 
			free_blocks.sort(sort_by_address);
			it = free_blocks.begin();
			while(it->address < prev_alloc_block) {
				free_blocks.push_back(*it);
				free_blocks.erase(it);
				it = free_blocks.begin();
			}
			break;

		case best_fit: 
			free_blocks.sort(sort_by_size_asceding);
			break;

		case worst_fit: 
			free_blocks.sort(sort_by_size_desceding);
			break;
	}

	for(it = free_blocks.begin(); it != free_blocks.end(); ++it) {
		
		/* 
			Search the free_blocks list until the first free block of sufficient size is found.
			Assign the process to the memory block (generate a used block and insert it into used_blocks list). 
			There are two cases to be considered:
			1. if the free block is of the exactly same size with what is requested by the process, then need to remove the free block from the list.
			2. if the free block is larger then requested size, then need to modify the start address and size of the free block.
		*/
		if(it->size >= size) {
			mem_block used_mem_block;
			used_mem_block.address = it->address;
			used_mem_block.size = size;
			used_mem_block.proc = proc;
			used_blocks.push_back(used_mem_block);
			prev_alloc_block = it->address;

			if(it->size == size) {
				free_blocks.erase(it);
			} else {
				it->address += size;
				it->size -= size;
			}
			return 0;
		}

	}
	
	return ALLOC_ERROR; //return error if no block of sufficient size is found.
}

/*
	Free the memory block occupied by the specified process.

	Parameters: 
		@proc: process ID.
		
	Return value:
		0 is returned if success, FREE_ERROR otherwise.
*/
int free_memory(int proc)
{
	if (proc <= 0) //invalid process ID
		return FREE_ERROR;

	//Find and remove the process from used_block list, and generate a new free block.
	mem_block new_free_block;
	new_free_block.address = -1;

	for(list<mem_block>::iterator it = used_blocks.begin(); it != used_blocks.end(); ++it) {
		if (it->proc == proc) {
			new_free_block.address = it->address;
			new_free_block.size = it->size;
			new_free_block.proc = -1;
			used_blocks.erase(it);
			break;
		}
	}

	//no such process exists.
	if (new_free_block.address < 0) 
		return FREE_ERROR;

	/*
		Insert the new free block into free_blocks list.
		Since the newly insterted block may be contiguous with one or two blocks, we need to merge the contiguous blocks if there are:
		An easy way is to sort the list by start address and then compare the end address of each free block with the start address of
		the next block.
	*/
	free_blocks.push_back(new_free_block);
	free_blocks.sort(sort_by_address);	
	for(list<mem_block>::iterator it = free_blocks.begin(); it != free_blocks.end(); ) {

		list<mem_block>::iterator current = it++;
		if ((it!=free_blocks.end()) && (current->address + current->size == it->address)) {
			current->size += it->size;
			free_blocks.erase(it);
			it = ++current;
		}

	}

	return 0;
}

/*
	Initialize the infomation of memory.

	Parameters: 
		@size: total available memory size.
		@start_address: the beginning of the memory address.
		
	Return value:
		None.
*/
void init_memory(int size, int start_address)
{
	free_blocks.clear();
	used_blocks.clear();

	mem_block init_block;
	init_block.address = start_address;
	init_block.size = size;
	init_block.proc = -1;
	free_blocks.push_back(init_block);

	max_memory_size = size;
	prev_alloc_block = start_address - 1;
}
