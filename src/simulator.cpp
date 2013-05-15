#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "placementAlgorithms.h"
using namespace std;

const int MAX_MEMORY_SIZE=100;		// total available memory size is 100 MB

/*
	This struct is used to store a memory request (alloc or free).

	Members:
		@cmd: "alloc": request memory; "free": free memory
		@proc: process ID
		@size: size of memory in KB, only meaningful when cmd is 'alloc'
*/
struct mem_request
{
	string cmd;
	int proc;
	int size;
};




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
void process_requests( alloc_algorithm algorithm, vector<mem_request>& requests )
{
	string enMyErrorValueNames[] = { "FIRST_FIT", "NEXT_FIT", "BEST_FIT", "WORST_FIT" };
	cout << endl << string( enMyErrorValueNames[static_cast<int> (algorithm)] )<<endl;
	Memory memory(MAX_MEMORY_SIZE);
	
	for(vector<mem_request>::iterator it = requests.begin(); it != requests.end(); ++it) {
		if (it->cmd == "alloc" ) {
			if ( memory.alloc_memory(algorithm, it->proc, it->size) == Memory::ALLOC_ERROR ) {
				cout << it->cmd << " "<< it-> proc << " "<< it->size << ": failed!" << endl;
				break;
			} else {
				stringstream ss;
				ss << it->cmd << " "<< it-> proc << " "<< it->size << ": ";
				ss << string(15-ss.str().length(), ' ') << memory.to_string();
				cout << ss.str() << endl;
			}

		} else if (it->cmd == "free") {
			if (memory.free_memory(it->proc) == Memory::FREE_ERROR) {
				cout << it->cmd << " "<< it-> proc << ": failed!" <<endl;
				break;
			} else {
				stringstream ss;
				ss << it->cmd << " "<< it-> proc << ": ";
				ss << string(15-ss.str().length(), ' ') << memory.to_string();
				cout << ss.str() << endl;
			}
		}
	}

	cout << endl;
}


int main(int argc, char* argv[])
{
	// read allocation/deallocation requests from file
	if(argc < 2) {
		cerr << "Error: no file specified. " << endl;
		cerr << "Usage: " << argv[0] << " requests.conf" << endl;
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

	//process memory requests with first_fit algorithms.
	process_requests( first_fit, request_queue );

	//next_fit algorithm
	process_requests( next_fit, request_queue );

	//best_fit algorithm
	process_requests( best_fit, request_queue );

	//worst_fit algorithm
	process_requests( worst_fit, request_queue );

	return 0;
}
