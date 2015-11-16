#ifndef __ARRAY_TEST_H__
#define __ARRAY_TEST_H__

#include"amp.h"
#include"array_modules.hpp"
#include<array>
#include<iostream>

void array_test(concurrency::accelerator& accel){

	const int thread = 100;
	constexpr std::size_t dim = 1;
	std::array<int,dim> number_of_threads({thread});

	std::unique_ptr<concurrency::array<int,dim>> vGArray(createArray<int,dim>(accel,thread));
	accessArray(*vGArray.get(),[&](concurrency::array_view<int,dim>& _array){ 
		for(int i=0; i<thread; i++)_array[i] = i;
		});

	concurrency::array_view<int,dim> view=*vGArray;

	parallelCalculation(accel,
			number_of_threads,
			[=](concurrency::index<dim> gindex) restrict(amp){
			view[gindex] *= view[gindex];
			}
			);
	
	for(int i=0; i<thread; i++){
		std::cout << view[i] << ",";
	}
	std::cout << std::endl;

}

#endif
