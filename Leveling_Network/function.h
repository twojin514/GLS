#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>


struct Line
{
	int32_t  line_number;
	std::string id_from;
	std::string id_to;
	double elevation_difference;
	double standard_deviation;
};

struct Benchmark
{
	int32_t benchmark_number;
	std::string id;
	double elevation;
	double standard_deviation;
};

void ReadInputLineFile(std::string FileName, std::vector<Line>& lines);
//int ReadInputBMFile(std::string FileName, std::vector<BM>& Out_BM);