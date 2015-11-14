#ifndef __TILED_INDEX_TEST_H__
#define __TILED_INDEX_TEST_H__

#include<array>

template<int COLS,int ROWS,int TILE_COLS,int TILE_ROWS>
std::array< std::array<float,COLS>, ROWS > convolutionCalculateAverage(std::array< std::array<int,COLS>, ROWS >& data, concurrency::accelerator& accel){
	std::array< std::array<float,COLS>,ROWS > average;

	concurrency::array_view<int,2> data_view(ROWS,COLS,reinterpret_cast<int*>(&data[0]));
	concurrency::array_view<float,2> average_view(ROWS,COLS,reinterpret_cast<float*>(&average[0]));

	parallel_for_each(
		data_view.get_extent().tile<TILE_ROWS,TILE_COLS>(),
		[=](concurrency::tiled_index<TILE_ROWS,TILE_COLS> idx) restrict(amp){
			tile_static int nums[TILE_ROWS][TILE_COLS];
			nums[idx.local[1]][idx.local[0]] = data_view[idx.global];
			idx.barrier.wait();
			int sum;
			for(int i=0; i<TILE_ROWS; i++){
				for(int j=0; j<TILE_COLS; j++){
					sum = nums[i][j];		
				}
			}
			average_view[idx.global] = sum / 4;
		}
	);
	
	average_view.synchronize();
	return average;
}
#endif
