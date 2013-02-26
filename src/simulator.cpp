#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "placementAlgorithms.h"
using namespace std;

struct mem_request
{
/*
	This struct is used to store a memory request (alloc or free).

	Members:
		@cmd: "alloc": request memory; "free": free memory
		@proc: process ID
		@size: size of memory in KB, only meaningful when cmd is 'alloc'
*/
	string cmd;
	int proc;
	int size;
};


const int MAX_MEMORY_SIZE=100;		// total available memory size is 100 MB


/*
	Generate and return a string representing current memory usage.

	Parameters:
		None.

	Return value:
		A string representing current memory usage.
*/
string analyze_mem_usage( )
{
	string rt = "";
	free_blocks.sort(sort_by_address);
	used_blocks.sort(sort_by_address);

	list<mem_block>::iterator free_it = free_blocks.begin();
	list<mem_block>::iterator used_it = used_blocks.begin();
	while( (free_it != free_blocks.end()) && (used_it != used_blocks.end()) ) {
		if( free_it->address < used_it->address) {
			int tempN = static_cast<int> (((static_cast<float>(free_it->size)) / MAX_MEMORY_SIZE) * 100);
			rt += string(tempN, '-');
			++free_it;

		} else {
			int tempN = static_cast<int> (((static_cast<float>(used_it->size)) / MAX_MEMORY_SIZE) * 100);
			rt += string(tempN, static_cast<char>('A'+ used_it->proc - 1));
			++used_it;
		}
	}

	while(free_it != free_blocks.end()) {
			int tempN = static_cast<int> (((static_cast<float>(free_it->size)) / MAX_MEMORY_SIZE) * 100);
			rt += string(tempN, '-');
			++free_it;
	}

	while(used_it != used_blocks.end()) {
			int tempN = static_cast<int> (((static_cast<float>(used_it->size)) / MAX_MEMORY_SIZE) * 100);
			rt += string(tempN, static_cast<char>('A'+ used_it->proc - 1));
			++used_it;
	}

	//rt += string(100-rt.length(), '-'); //in case the string is not of full length (100) due to loss of precision caused by casting.
	return rt;
}

/*
	Handle a sequence of memory request according to the passed-in algorithm, and output the memory usage into specified file
	after every request, stop at the first failure.

	Parameters:
		@algorithm: the placement algorithm adpoted. (first_fit or next_fit or best_fit or worst_fit)
		@requests: a reference to the vector containing all the memory requests.
		@output_file_name: a pointer to the output file name.

	Return value:
		None.
*/
void process_requests( alloc_algorithm algorithm, vector<mem_request>& requests, char* output_file_name )
{
	ofstream output_file;
	output_file.open(output_file_name, ios::out | ios::app);

	const char* enMyErrorValueNames[] = { "FIRST_FIT", "NEXT_FIT", "BEST_FIT", "WORST_FIT" };
	output_file<< string( enMyErrorValueNames[static_cast<int> (algorithm)] )<<endl;
	cout << endl << string( enMyErrorValueNames[static_cast<int> (algorithm)] )<<endl;

	for(vector<mem_request>::iterator it = requests.begin(); it != requests.end(); ++it) {
		if (it->cmd == "alloc" ) {
			if ( alloc_memory(algorithm, it->proc, it->size) == ALLOC_ERROR ) {
				cout << it->cmd << " "<< it-> proc << " "<< it->size << ": failed!" << endl;
				output_file << it->cmd << " "<< it-> proc << " "<< it->size << ": failed!" << endl;
				break;
			} else {
				stringstream ss;
				ss << it->cmd << " "<< it-> proc << " "<< it->size << ": ";
				ss << string(15-ss.str().length(), ' ') << analyze_mem_usage();
				cout << ss.str() << endl;
				output_file  << ss.str() << endl;
			}

		} else if (it->cmd == "free") {
			if (free_memory(it->proc) == FREE_ERROR) {
				cout << it->cmd << " "<< it-> proc << ": failed!" <<endl;
				output_file << it->cmd << " "<< it-> proc << ": failed!" <<endl;
				break;
			} else {
				stringstream ss;
				ss << it->cmd << " "<< it-> proc << ": ";
				ss << string(15-ss.str().length(), ' ') << analyze_mem_usage();
				cout << ss.str() << endl;
				output_file << ss.str() << endl;
			}
		}
	}

	cout << endl;
	output_file << endl << endl;
	output_file.close();

}


int main(int argc, char* argv[])
{
	// read allocation/deallocation requests from file
	if(argc < 3) {
		cerr << "Error: no file specified. " << endl;
		cerr << "Usage: MmoryManagement.exe input_requests_file output_file" << endl;
		return -1;
	}
	ifstream requests_file(argv[1]);
	if (!requests_file) {
		cerr << "can't open " << argv[1] << endl;
		return -1;
	}

	// save all requests in a vector for later execution
	vector<mem_request> request_queue;
	string request;

	while (getline(requests_file, request)) {
		istringstream iss(request);
		mem_request rqst;
		iss >> rqst.cmd >> rqst.proc;
		if (rqst.cmd == "alloc")
			iss >> rqst.size;
		request_queue.push_back( rqst );
	}

	//initialize memory block list, only 1 big free block at the beginning, none is used.
	init_memory(MAX_MEMORY_SIZE);

	//process memory requests with first_fit algorithms.
	process_requests( first_fit, request_queue, argv[2] );

	//next_fit algorithm
	init_memory(MAX_MEMORY_SIZE);
	process_requests( next_fit, request_queue, argv[2] );

	//best_fit algorithm
	init_memory(MAX_MEMORY_SIZE);
	process_requests( best_fit, request_queue, argv[2] );

	//worst_fit algorithm
	init_memory(MAX_MEMORY_SIZE);
	process_requests( worst_fit, request_queue, argv[2] );

	return 0;
}
