**Input has 10000 random numbers**

1. **Normal Mergesort**

gcc merge_sort.c ; time ./a.out < input1.txt > norout1.txt
    
2. **Mergesort using child processes created using fork**

It uses **shmget**  (for shared memory allocation) and **shmat** (for shared memory operations) functions. We create a shared memory space between the child process that we fork.  Each segment is split into left and right child which is sorted, the interesting part being they are working concurrently! The shmget()  requests the kernel to allocate a shared page for both the processes. shmat() attaches the System V shared memory segment identified by **shmid** to the address space of the calling process. 

gcc merge_sort_process.c ; time ./a.out < input1.txt > out1.txt

**Why we need shared memory?**


The traditional fork does not work because the child process and the parent process run in separate memory spaces and memory writes performed by one of the processes do not affect the other. Hence we need a shared memory segment.
**Time Analysis for 10000 numbers :**

| Cases                         | Real     | User     | Sys      |
| ----------------------------- | -------- | -------- | -------- |
| Normal mergesort              | 0m0.021s | 0m0.012s | 0m0.010s |
| Mergesort using forkprocesses | 0m0.952s | 0m2.549s | 0m0.898s |

**Conclusion**

The time taken in **2nd case** is more than normal mergesort because when, say left child, access the left array, the array is loaded into the cache of a processor. Now when the right array is accessed (because of concurrent accesses), there is a cache miss since the cache is filled  with left segment and then right segment is copied to the cache memory. This to-and-fro process continues and it degrades the performance to such a level that it performs poorer than the sequential code.
