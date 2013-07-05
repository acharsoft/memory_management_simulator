##Test scenario 1:
+**Purpose:**
Demonstrate the behavior of the four algorithms.

+**Assumption:**
max_mem_size = 100, mem_start_address = 0

+**Test Case:**
>1. set up memory environment by following requests:
    alloc 1 8 -> alloc 2 15 -> alloc 3 5 -> alloc 4 10 -> alloc 5 11 -> free 2
    -> free 4 -> alloc 6 20 -> alloc 7 18 -> free 6
>2. submit request: alloc 8 8

+**Test Result:**
++Step 1 results following memory blocks:
![test1_step1](https://github.com/ceciliazhou/dynamic_partition_simulator/blob/master/doc/resource/test1_step1.jpeg)
