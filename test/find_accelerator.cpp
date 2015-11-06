#include<amp.h>
#include<iostream>
#include<vector>

using concurrency::accelerator;

int main(){
	std::vector<accelerator> vAcs;
	vAcs = accelerator::get_all();

	for(int i=0; i<vAcs.size(); i++){
		std::wcout << i << "th device = " << vAcs[i].get_description() << "\n";
	}
	return 0;
}
