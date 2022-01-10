# Named Memory Regions

Memory, regions are a technique that can supplement the standard C memory allocation system, making it more efficient to allocate and free memory, and to prevent memory leaks.

Implemented named memory-regions where each region is given a name (a string) and referred to by that name. Each region has a fixed maximum size that is specified when it is created. Within each region, blocks of memory can be allocated and freed, totalling up to its maximum size.

* `rsize_t` represents the amount of memory in bytes, holds values in multiple of 8 only.

* `boolean rinit(const char  *regionname, rsize_t region_size);` Create and initialize a region with the given name and the given size. The name must be unique and the size must be greater than zero. Return false on error. If the call succeeds, choose the newly-created region. 

* `boolean rchoose(const char *regionname);` Choose a previously-initialized memory region for subsequent allot, size, and free calls. Return false on error. 

* `const char *rchosen();` Return the name of the currently-chosen region, or NULL if no region is currently chosen.

* `void *ralloc(rsize_t blocksize);` Allocate a block that is the given number of bytes large from the currently-chosen region. Clear its contents to zero. Return a pointer to the block or NULL if an error occurs. Size 0 blocks are not allowed. 

* `rsize_t rsize(void * block_ptr);` Find out how many bytes the block of memory pointed to by the pointer is, in the currently-chosen region. Return 0 on error.

* `boolean rfree(void *block_ptr);` Frees the block identified by the given pointer in the currently-chosen region. Return false on error. 

* `void rdestroy (const char  *regionname);` Destroy the region with the given name, freeing all memory associated with it. Note that after calling this function, any library functions that attempt to access this region should return an error (but you could use rinit to create a new region with the same name). If the region was chosen, it will no longer be. 

* `void rdump();` Print all data structures, but not block contents. Show the name of each region; underneath each, show the blocks allocated in them and the, block sizes (use "%p" printf format for block pointers), and the percentage of the region that is still free.

To compile the code,
```bash
make
```

To run the program,
```bash
./regions
```