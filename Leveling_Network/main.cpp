#include <iostream>
#include "function.h"

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
	 1.  입력 자료 읽기 및 출력
	**********************************************************************/

	std::printf("************************************************************************\n");
	std::printf("************************* 수준망 조정프로그램 **************************\n");
	std::printf("************************************************************************\n");

	outfile << "************************************************************************\n";
	outfile << "************************ 수준망 조정프로그램 ***********************\n";
	outfile << "************************************************************************\n";

	std::vector<Line> lines;
	std::vector<Benchmark> benchmarks;

	ReadInputLineFile(input_line_data, lines);
	ReadInputBMFile(input_bm_data, benchmarks);
	PrintInputData(lines, benchmarks);

	/*********************************************************************
	 2. 측점 논리적 순서 정렬
	**********************************************************************/

	int32_t point_num;
	std::vector<std::string> point_names;

	SortInputData(lines, benchmarks, point_num, point_names);
		

	/*********************************************************************
	3.  수준망 조정
	*********************************************************************/
	// 3.1 Matrix 설정
	int32_t n_measurement_point = point_num;
	int32_t m_observation_point = lines.size()+benchmarks.size();

	std::vector<std::vector<std::string>> x_matrix;
	SetXMatrix(lines, benchmarks, point_num, point_names, x_matrix);

	std::vector<std::vector<int32_t>> a_matrix;
	SetAMatrix(lines, benchmarks, m_observation_point, n_measurement_point,x_matrix ,a_matrix);

	std::vector<std::vector<float>> l_matrix;
	SetLMatrix(lines, benchmarks, m_observation_point, l_matrix);

	std::vector<std::vector<double>> w_matrix;
	SetWMatrix(lines, benchmarks, m_observation_point, w_matrix);


	return 0;
}

