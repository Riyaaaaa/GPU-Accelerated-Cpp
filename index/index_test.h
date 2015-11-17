#ifndef __INDEX_TEST_H__
#define __INDEX_TEST_H__

#include"amp.h"
#include"index_modules.hpp"
#include"tiled_index_modules.hpp"
#include<array>
#include<iostream>

void index_test(const concurrency::accelerator& accel){
	constexpr int COLS=6,ROWS=4;
	std::array<std::array<float,COLS>,ROWS> data={
		1,2,3,4,5,6,
		7,8,9,10,11,12,
		1,2,3,4,5,6,
		7,8,9,10,11,12
	};

	concurrency::array_view<float,2> data_view(ROWS,COLS,reinterpret_cast<float*>(&data[0][0]));
	//print data_view[3][2] by index<2>;
	std::cout << accessArrayByIndex(data_view,3,2);
	std::cout << std::endl;

	auto result = convolutionCalculateAverage<6,4,2,2>(data,accel);

	for(int i=0; i<ROWS; i++){
		for(int j=0; j<COLS; j++){
			std::cout << result[i][j] << ",";
		}
		std::cout << std::endl;
	}


}

#endif
