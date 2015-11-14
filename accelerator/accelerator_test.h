#ifndef __GPU_ACCELERATOR__
#define __GPU_ACCELERATOR__

#include<amp.h>
#include<vector>

std::vector<concurrency::accelerator> find_accelerators();
void get_accel_discription(concurrency::accelerator& accel);

std::vector<concurrency::accelerator>::iterator getBiggestMemoryAccelerator(std::vector<concurrency::accelerator> accels);

#endif
