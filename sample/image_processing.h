#pragma once

#include"opencv_include.h"

#include"amp.h"
#include"index/tiled_index_modules.hpp"

#include<iostream>
#include<chrono>

void image_processing_test(concurrency::accelerator& accel)
{                                                                                                                                                                                    
	cv::Mat input;
	cv::Mat_<float> gray,gray_cpu;
	input = cv::imread("image_middle.jpg",cv::IMREAD_GRAYSCALE);
	input.convertTo(gray, CV_32FC1);
	input.convertTo(gray_cpu, CV_32FC1);

	for (int rows = 0; rows < input.rows; rows++) {
		for (int cols = 0; cols < input.cols; cols++) {
			gray.at<float>(rows, cols) /= 255.;
		}
	}
	std::cout << gray.elemSize1() << std::endl;
	std::cout << gray.channels() << std::endl;
	std::cout << gray.step << "/" << gray.elemSize() * gray.cols << std::endl;
	std::cout << gray.isContinuous() << std::endl;

	const int convolution_size = 15;
	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		for (int rows = 0; rows < gray.rows; rows++) {
			for (int cols = 0; cols < gray.cols; cols++) {

				int sum;

				for (int y = -convolution_size; y <= convolution_size; y++) {
					for (int x = -convolution_size; x <= convolution_size; x++) {
						if (rows + y >= 0 && rows + y < gray.rows && cols + x >= 0 && cols + x < gray.cols)
							sum += gray.data[(rows + y) * gray.step + (cols + x) * gray.elemSize()];
						else sum += gray.data[rows * gray.step + cols * gray.elemSize()];
					}
				}
				gray_cpu.data[rows * gray.step + cols * gray.elemSize()] = sum / pow(2 * convolution_size + 1, 2);

			}
		}
		std::chrono::time_point<std::chrono::system_clock> after = std::chrono::system_clock::now();
		std::cout << "----------------cpu calculation suceeded---------------" << std::endl;
		std::chrono::duration<double> diff = after - now;
		std::cout << "score " << diff.count() << "[s]" << std::endl;
	}

	{
		std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
		auto result = convolutionCalculateAverage<float, 30, 30>(reinterpret_cast<float*>(&gray.data[0]), input.rows, input.cols, accel);
		std::chrono::time_point<std::chrono::system_clock> after = std::chrono::system_clock::now();

		std::cout << "----------------gpu calculation suceeded---------------" << std::endl;

		std::chrono::duration<double> diff = after - now;

		std::cout << "score " << diff.count() << "[s]" << std::endl;

		for (int rows = 0; rows < input.rows; rows++) {
			for (int cols = 0; cols < input.cols; cols++) {
				gray.at<float>(rows, cols) = result[rows*input.cols + cols];
			}
		}
	}

	cv::namedWindow("window", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("window2", CV_WINDOW_AUTOSIZE);

	cv::imshow("window", gray);
	cv::imshow("window2", input);

	cv::waitKey(0);
}