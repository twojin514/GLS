#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <locale>
#include <cctype>

struct Line
{
	int32_t  line_number;
	std::string id_to;
	std::string id_from;
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

void ReadInputLineFile(std::string FileName, std::vector<Line>& lines_data);
void ReadInputBMFile(std::string FileName, std::vector<Benchmark>& benchmarks_data);
void PrintInputData(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data);
void SortInputData(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& point_number, std::vector<std::string>& point_names);
void SetXMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& n_measurement_point, std::vector<std::string>& point_names, std::vector<std::vector<std::string>>& x_matrix);
void SetAMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, int32_t& n_measurement_point, std::vector<std::vector<std::string>>& x_matrix, std::vector<std::vector<std::int32_t>>& a_matrix);
void SetLMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, std::vector<std::vector<float>>& l_matrix);
void SetWMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, std::vector<std::vector<double>>& w_matrix);
void SetATWAMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data);
