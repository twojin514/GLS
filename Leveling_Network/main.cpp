#include "function.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv[])
{
	/*********************************************************************
	 0.  PATH 설정 (입력 및 출력 자료)
	**********************************************************************/

	std::string input_file_dir = "..\\Input\\";
	std::string input_line_data = input_file_dir + "Line_Data_1.csv";
	std::string input_bm_data= input_file_dir + "BM_Data_1.csv";

	std::string output_file_dir = "..\\Output\\";
	std::string output_file = output_file_dir + "Leveling_Network_1.txt";
	std::ofstream outfile(output_file);  // 출력 자료 생성


	/*********************************************************************
	 1.  입력 자료 읽기
	**********************************************************************/


	std::vector<Line> lines;
	std::vector<Benchmark> benchmarks;

	ReadInputLineFile(input_line_data, lines);
	ReadInputBMFile(input_bm_data, benchmarks);

	/*********************************************************************
	2. 수준망 조정 시작
	*********************************************************************/
	// 2.0 측점 논리적 순서 정렬
	int32_t point_num;
	std::vector<std::string> point_names;

	SortInputData(lines, benchmarks, point_num, point_names);

	// 2.1 Matrix 설정
	int32_t n_measurement_point = point_num;
	int32_t m_observation_point = lines.size()+benchmarks.size();

	std::vector<std::vector<std::string>> x_matrix;
	std::vector<std::vector<int32_t>> a_matrix;
	std::vector<std::vector<double>> l_matrix;
	std::vector<std::vector<double>> w_matrix;
	std::vector<std::vector<double>> atwa_matrix;
	std::vector<std::vector<double>> atwl_matrix;

	SetXMatrix(lines, benchmarks, point_num, point_names, x_matrix);
	SetAMatrix(lines, benchmarks, m_observation_point, n_measurement_point,x_matrix ,a_matrix);
	SetLMatrix(lines, benchmarks, m_observation_point, l_matrix);
	SetWMatrix(lines, benchmarks, m_observation_point, w_matrix);
	SetATWAMatrix(lines, benchmarks, n_measurement_point, a_matrix, w_matrix, atwa_matrix);
	SetATWLMatrix(lines, benchmarks, n_measurement_point, a_matrix, w_matrix, l_matrix, atwl_matrix);
	
	// 2.2 Matrix 출력
	PrintLevelingProgram(outfile, lines, benchmarks, a_matrix, x_matrix, l_matrix, w_matrix, atwa_matrix, atwl_matrix);
	cv::Mat cv_x_matrix, cv_a_matrix, cv_l_matrix, cv_w_matrix, cv_atwa_matrix, cv_atwl_matrix, cv_inv_atwa_matrix;

	cv::Mat mat = cv::Mat(a_matrix.size(), a_matrix[0].size(), CV_32F);
	for (int i = 0; i < mat.rows; ++i) {
		for (int j = 0; j < mat.cols; ++j) {
			mat.at<float>(i, j) = a_matrix[i][j];
		}
	}

	cv::Mat mat2=cv::Mat(w_matrix.size(), w_matrix[0].size(), CV_32F);
	for (int i = 0; i < mat2.rows; ++i) {
		for (int j = 0; j < mat2.cols; ++j) {
			mat2.at<float>(i, j) = w_matrix[i][j];
		}
	}
	cv::Mat mat3 = cv::Mat(l_matrix.size(), l_matrix[0].size(), CV_32F);
	for (int i = 0; i < mat3.rows; ++i) {
		for (int j = 0; j < mat3.cols; ++j) {
			mat3.at<float>(i, j) = l_matrix[i][j];
		}
	}

	std::cout<<(mat.t()*mat2*mat)<<std::endl;
	std::cout<<(mat.t()*mat2*mat3)<<std::endl;
	std::cout<<(mat.t()*mat2*mat).inv()<<std::endl;
	std::cout<<(mat.t()*mat2*mat).inv()*(mat.t()*mat2*mat3)<<std::endl;



	outfile.close();

	return 0;
}

