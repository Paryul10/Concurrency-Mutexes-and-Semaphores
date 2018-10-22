**Input has 10000 random numbers**

1. **Normal Mergesort**

gcc merge_sort.c ; time ./a.out < input1.txt > norout1.txt
    
2. **Mergesort using child processes created using fork**

It uses **shmget**  (for shared memory allocation) and **shmat** (for shared memory operations) functions. We create a shared memory space between the child process that we fork.  Each segment is split into left and right child which is sorted, the interesting part being they are working concurrently! The shmget()  requests the kernel to allocate a shared page for both the processes. shmat() attaches the System V shared memory segment identified by **shmid** to the address space of the calling process. 

gcc merge_sort_process.c ; time ./a.out < input1.txt > procout1.txt

**Why we need shared memory?**


The traditional fork does not work because the child process and the parent process run in separate memory spaces and memory writes performed by one of the processes do not affect the other. Hence we need a shared memory segment.

3. **Mergesort using threading**

It uses threads and  lock.
There is one thread for every left and right child
Lock is used when we are writing in array, basically when we call merge().

gcc  merge_sort_threads.c -lpthread ; time ./a.out < input1.txt > thout1.txt

**Time Analysis for 10000 numbers :**

| Cases                         | Real     | User     | Sys      |
| ----------------------------- | -------- | -------- | -------- |
| Normal mergesort              | 0m0.021s | 0m0.012s | 0m0.010s |
| Mergesort using forkprocesses | 0m0.952s | 0m2.549s | 0m0.898s |
| Mergesort using threads       | 0m0.612s | 0m0.148s | 0m0.916s |


**Conclusion**

The time taken in **2nd case** is more than normal mergesort because when, say left child, access the left array, the array is loaded into the cache of a processor. Now when the right array is accessed (because of concurrent accesses), there is a cache miss since the cache is filled  with left segment and then right segment is copied to the cache memory. This to-and-fro process continues and it degrades the performance to such a level that it performs poorer than the sequential code.

The time taken in **3rd case** is more than 1st but less than  2nd as we are creating large amount of threads each of which is doing very little work, therefore  the overhead from creating all those threads far outweighs the gains you get from parallel processing. To speed this up we can make sure that each thread has a reasonable amount of work to do. For example, if one thread finds that it only has to sort <10000 numbers it can simply sort them itself with a normal merge sort, instead of spawning new threads. As you increase the number of threads beyond the number of cores on the system, you get less and less benefit from the multiple threads. 
Also since cost of creating multiple processes is far more than just creating threads , the time consumed is less.