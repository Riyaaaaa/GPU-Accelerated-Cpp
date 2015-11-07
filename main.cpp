#include<iostream>
#include<amp.h>
#include<iostream>

#include"accelerator/accelerator_test.h"
#include"array/array_test.h"

using namespace concurrency;

int main(){
	std::vector<accelerator> accels = find_accelerators();

	if(accels.empty()){
		std::cout << "not found valid GPU" << std::endl;
		return -1;
	}

	for(auto& accel : accels){
		get_accel_discription(accel);
	}

	const std::size_t array_size = 3;
	constexpr int dim = 1;

	std::unique_ptr<array<int,dim>> vGArray(create_array<int,dim>(accels[0],array_size));
	access_array(*vGArray.get(),[&](array_view<int,dim>& _array){ for(int i=0; i<array_size; i++)_array[i] = i;});

	for(int i=0; i<array_size; i++){
		std::cout << (*vGArray.get())[i] << std::endl;
	}

	return 0;
}
