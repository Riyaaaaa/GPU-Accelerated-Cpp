#ifndef __TILED_INDEX_MODULES_H__
#define __TILED_INDEX_MODULES_H__

#include<array>
#include"opencv_include.h"

// Read character at index idx from array arr.
template <typename T>
unsigned int read_uchar(T& arr, int idx) restrict(amp)
{
	return (arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3))) >> ((idx & 0x3) << 3);
}

// Increment character at index idx in array arr.
template<typename T>
void increment_uchar(T& arr, int idx) restrict(amp)
{
	atomic_fetch_add(&arr[idx >> 2], 1 << ((idx & 0x3) << 3));
}

// Add value val to character at index idx in array arr.
template<typename T>
void addto_uchar(T& arr, int idx, unsigned int val) restrict(amp)
{
	atomic_fetch_add(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// Write value val to character at index idx in array arr.
template<typename T>
void write_uchar(T& arr, int idx, unsigned int val) restrict(amp)
{
	atomic_fetch_xor(&arr[idx >> 2], arr[idx >> 2] & (0xFF << ((idx & 0x3) << 3)));
	atomic_fetch_xor(&arr[idx >> 2], (val & 0xFF) << ((idx & 0x3) << 3));
}

// Helper function to accept 1D indices of index<1> type instead of integers.
template <typename T>
unsigned int read_uchar(T& arr, concurrency::index<1> idx) restrict(amp) { return read_uchar(arr, idx[0]); }
template<typename T>
void increment_uchar(T& arr, concurrency::index<1> idx) restrict(amp) { increment_uchar(arr, idx[0]); }
template<typename T>
void addto_uchar(T& arr, concurrency::index<1> idx, unsigned int val) restrict(amp) { addto_uchar(arr, idx[0], val); }
template<typename T>
void write_uchar(T& arr, concurrency::index<1> idx, unsigned int val) restrict(amp) { write_uchar(arr, idx[0], val); }

template<std::size_t COLS,std::size_t ROWS,int TILE_COLS,int TILE_ROWS>
std::array< std::array<float,COLS>, ROWS > convolutionCalculateAverage(const std::array< std::array<float,COLS>, ROWS >& data,const  concurrency::accelerator& accel){
	std::array< std::array<float,COLS>,ROWS > average;

	concurrency::array_view<const float,2> data_view(ROWS,COLS,reinterpret_cast<const float*>(&data[0]));
	concurrency::array_view<float,2> average_view(ROWS,COLS,reinterpret_cast<float*>(&average[0]));

	average_view.discard_data();
	parallel_for_each(
		data_view.get_extent().tile<TILE_ROWS,TILE_COLS>(),
		[=](concurrency::tiled_index<TILE_ROWS,TILE_COLS> idx) restrict(amp){
			tile_static float nums[TILE_ROWS][TILE_COLS];
			nums[idx.local[1]][idx.local[0]] = data_view[idx.global];
			idx.barrier.wait();
			float sum=0;
			for(int i=0; i<TILE_ROWS; i++){
				for(int j=0; j<TILE_COLS; j++){
					sum += nums[i][j];		
				}
			}
			average_view[idx.global] = sum / static_cast<float>(TILE_ROWS*TILE_COLS);
		}
	);	
	average_view.synchronize();
	
	return average;
}

template<typename T,int TILE_COLS, int TILE_ROWS>
std::unique_ptr<T[]> convolutionCalculateAverage(T* data, int rows, int cols,const concurrency::accelerator& accel)
{
	std::unique_ptr<T[]> average(new T[rows*cols]);

	concurrency::array_view<T, 2> data_view(rows, cols, data);
	concurrency::array_view<float, 2> average_view(rows, cols, reinterpret_cast<float*>(average.get()));

	std::cout << "\n-------------------parallel calculation--------------------" << std::endl;
	std::cout << "rows/cols " << rows << "/" << cols << std::endl;

	average_view.discard_data();
	parallel_for_each(
		data_view.get_extent().tile<TILE_ROWS, TILE_COLS>(),
		[=](concurrency::tiled_index<TILE_ROWS, TILE_COLS> idx) restrict(amp) {
		tile_static T nums[TILE_ROWS][TILE_COLS];
		nums[idx.local[1]][idx.local[0]] = data_view[idx.global];
		idx.barrier.wait();
		T sum=0;
		for (int i = 0; i<TILE_ROWS; i++) {
			for (int j = 0; j<TILE_COLS; j++) {
				sum += nums[i][j];
			}
		}
		average_view[idx.global] = sum / static_cast<T>(TILE_ROWS*TILE_COLS);
	}
	);
	average_view.synchronize();

	return std::move(average);
}

template<typename T,int TILE_COLS, int TILE_ROWS,int CHANNELS>
std::vector< std::vector< std::array<T, CHANNELS> > > convolutionCalculateAverageWithChannel(const T* data, int rows, int cols,const concurrency::accelerator& accel) {
	std::vector< std::vector< std::array<T,CHANNELS> > > average( cols, std::vector<std::array<T,CHANNELS>>(rows) );

	concurrency::array_view<const T, 3> data_view(rows, cols, CHANNELS, data);
	concurrency::array_view<T, 3> average_view(rows, cols, CHANNELS, reinterpret_cast<T*>(&average[0]));

	average_view.discard_data();
	parallel_for_each(
		data_view.get_extent().tile<TILE_ROWS, TILE_COLS, CHANNELS>(),
		[=](concurrency::tiled_index<TILE_ROWS, TILE_COLS, CHANNELS> idx) restrict(amp) {
		tile_static int nums[TILE_ROWS][TILE_COLS][CHANNELS];
		nums[idx.local[2]][idx.local[1]][idx.local[0]] = data_view[idx.global];
		idx.barrier.wait();
		int sum[CHANNELS];
		for (int i = 0; i<TILE_ROWS; i++) {
			for (int j = 0; j < TILE_COLS; j++) {
				for (int k = 0; k < CHANNELS; k++) {
					sum[k] = nums[i][j][k];
				}
			}
		}
		for (int k = 0; k < CHANNELS; k++)
			average_view[idx.global] = sum[k] / ( TILE_ROWS*TILE_COLS*CHANNELS );
	}
	);
	average_view.synchronize();

	return average;
}
#endif
