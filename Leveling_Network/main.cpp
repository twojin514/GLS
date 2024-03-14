#include <iostream>
#include "function.h"

int main(int argc, char* argv[])
{
	std::printf("Leveling Network\n");



	/*********************************************************************
	********************* 0. input data path *****************************
	**********************************************************************/

	std::string input_file_dir = "..\\Input\\";
	std::string input_line_data = input_file_dir + "Line_Data_1.csv";
	std::string input_bm_data= input_file_dir + "BM_Data_1.csv";




	/*********************************************************************
												
	**********************************************************************/
	std::vector<Line> lines;
	std::vector<Benchmark> benchmarks;

	ReadInputLineFile(input_line_data, lines);

		
	
	//std::vector<Line> lines;
	//std::vector<Benchmark> benchmarks;
	//ReadInputData(input_line_data, input_bm_data, lines, benchmarks);

	//// 2. Create leveling network
	//LevelingNetwork leveling_network;
	//leveling_network.CreateLevelingNetwork(lines, benchmarks);

	//// 3. Solve leveling network
	//leveling_network.SolveLevelingNetwork();

	//// 4. Write output data
	//std::string output_file_dir = "..\\OutputData\\";
	//std::string output_file = output_file_dir + "Leveling_Network_1.csv";
	//WriteOutputData(output_file, leveling_network);

	//// 5. Print output data
	//PrintOutputData(leveling_network);

	//// 6. Plot output data
	//PlotOutputData(leveling_network);

	//// 7. Return





	return 0;
}

