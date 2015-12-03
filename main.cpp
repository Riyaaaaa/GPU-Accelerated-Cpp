#include<iostream>
#include<amp.h>
#include<iostream>
#include<array>
#include"accelerator/accelerator_test.h"
#include"array/array_test.h"
#include"index/index_test.h"
#include"sample/image_processing.h"

using namespace concurrency;

int main(){
	std::vector<accelerator> accels = findAccelerators();

	if(accels.empty()){
		std::cout << "not found valid GPU" << std::endl;
		return -1;
	}

	for(auto& accel : accels){
		getAccelDiscription(accel);
	}
	
	//choose the accelerator that has largest memory
	accelerator accel = *(getBiggestMemoryAccelerator(accels));

	//run the test of array
	//array_test(accel);

	//run the test of index
	/*index_test(accel);*/

	image_processing_test(accel);
	
	return 0;
}
