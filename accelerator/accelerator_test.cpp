#include<iostream>
#include<algorithm>
#include"accelerator_test.h"

using concurrency::accelerator;

std::vector<accelerator> findAccelerators(){
	std::vector<accelerator> accels;
	accels = accelerator::get_all();

	for(int i=0; i<accels.size(); i++){
		std::wcout << i << "th device = " << accels[i].get_description() << "\n";
	}

	accels.erase(std::remove_if(accels.begin(),accels.end(),[](accelerator& accel){return accel.get_is_emulated();}),accels.end());

	return accels;
}

void getAccelDiscription(const accelerator& accel){

	std::wcout << "accelerator: "<< accel.get_description() << std::endl;
	std::cout << "version of the accelerator: " << accel.get_version() << std::endl;
	std::cout << "memory: " << accel.get_dedicated_memory()/1024./1000. << " [GB]" << std::endl;;
	std::cout << "is supporting double precision: " << (accel.get_supports_double_precision() ? "yes" : "no") << std::endl;
	std::cout << "is attached to a display: " << (accel.get_has_display() ? "yes" : "no") << std::endl;
	std::cout << "is supporting cpu shared memory: " << (accel.get_supports_cpu_shared_memory() ? "yes" : "no") << std::endl;
	return;
}

std::vector<accelerator>::iterator getBiggestMemoryAccelerator(std::vector<accelerator>& accels){
	return std::max_element(accels.begin(),accels.end(),[](const accelerator& rhs,const accelerator& lhs){return rhs.get_dedicated_memory() < lhs.get_dedicated_memory();});
}
