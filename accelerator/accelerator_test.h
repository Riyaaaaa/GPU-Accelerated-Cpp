#ifndef __GPU_ACCELERATOR__
#define __GPU_ACCELERATOR__

#include<amp.h>
#include<vector>

std::vector<concurrency::accelerator> findAccelerators();
void getAccelDiscription(const concurrency::accelerator& accel);

std::vector<concurrency::accelerator>::iterator getBiggestMemoryAccelerator(std::vector<concurrency::accelerator>& accels);

#endif
