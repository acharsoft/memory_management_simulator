About
--------
This program model the effect of different placement algorithms used to assign a
process to memory with dynamic partition. In particular, when there are more than one
free memory block of the sufficient size, we need to decide which one to allocate.
Here, we implement four popular algorithms (first-fit, next-fit, best-fit, worst-fit)
and a simulator as well to demonstrate the differences between them.

Constraints
--------
As this program is to simulate how the four placement algorithms work, there isn't any
memory allocation/deallocation operated on real physical memory. 
Also, as we aim to model the memory layout resulted by these algorithms, we don't consider time effecience.

How to use
--------
1. compile the source files:<br/>
  
  `g++ *cpp -o simulator`
2. configure requests.cfg:<br/>
  All memory requests are configured in an input file. There are two kinds of requests,
either to request allocation or to release allocation. Allocation is requested by
"alloc proc_id mem_size" and released by "free proc_id". Following is an example:
  ```
  alloc 1 20
  alloc 2 15
  free 1
  ```
  Please check out [here](https://raw.github.com/ceciliazhou/dynamic_partition_simulator/master/conf/test_case.conf) for another example. 
3. run the simulator with the file resulted in step 2 as input.
  
  `simulator requests.cfg`
