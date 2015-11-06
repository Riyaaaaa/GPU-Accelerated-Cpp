#include <amp.h>
#include <iostream>
#include <vector>
#include <algorithm>

using  concurrency::accelerator;
int main()
{
    std::vector<accelerator> accels = accelerator::get_all();

	accels.erase(std::remove_if(accels.begin(),accels.end(),[](accelerator& accel){return accel.get_is_emulated();}),accels.end());

    for(int i=0;i<accels.size();i++)
    {
        std::wcout << i <<"th accelerator: "<< accels[i].get_description() << std::endl;
		std::cout << "version of the accelerator: " << accels[i].get_version() << std::endl;
        std::cout << "memory: " << accels[i].get_dedicated_memory()/1024./1000. << " [GB]" << std::endl;;
        std::cout << "is supporting double precision: " << (accels[i].get_supports_double_precision() ? "yes" : "no") << std::endl;

        std::cout << "is attached to a display: " << (accels[i].get_has_display() ? "yes" : "no") << std::endl;
        std::cout << "is supporting cpu shared memory: " << (accels[i].get_supports_cpu_shared_memory() ? "yes" : "no") << std::endl;
    }
	return 0;
}

