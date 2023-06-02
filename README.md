# Temporary Memory Allocator
The user reserves a large pool of memory from the heap at startup and then allocates from the pool as needed. The memory allocation happens only once, thus obviating malloc/free bottlenecks.
