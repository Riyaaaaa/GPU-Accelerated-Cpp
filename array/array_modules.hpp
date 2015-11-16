#ifndef __ARRAY_MODULES_H__
#define __ARRAY_MODULES_H__

#include<amp.h>
#include<utility>

template<class T,std::size_t dim>
concurrency::array<T,dim>* createArray(const concurrency::accelerator& accel,std::size_t size){
	return new concurrency::array<T,dim>(size,accel.get_default_view());
}

template<class T,int dim,class F>
void accessArray(concurrency::array<T,dim>& vGArray,F&& function){
	concurrency::array_view<T,dim> vGArrayView = vGArray;
	function(vGArrayView);
}

/* Define the signatures of function object used as a void(index<Dim>)*/
/* Use the copy[=] when using the lambda expression. Do not use the capture([&])*/
template<std::size_t Dim,class F>
void parallelCalculation(concurrency::accelerator& accel,std::array<int,Dim> nums_thread,F&& function){
	concurrency::extent<static_cast<int>(Dim)> ex;
	for(int i=0; i<static_cast<int>(Dim); i++){
		ex[i] = nums_thread[i];
	}
	parallel_for_each(
		accel.get_default_view(),
		ex,
		function
	);
}
#endif
