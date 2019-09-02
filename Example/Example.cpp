MemoryPool memoryPool(1024 * 1024 * 100, 1024 * 1024 * 50);// Initialize a 100M memory pool, 50M each time to extend.
void* memory = memoryPool.Obtain(1024 * 1024 * 10);// Obtain 10M memory from pool.
memoryPool.Recycle(memory);// Recycle memory block back to pool.