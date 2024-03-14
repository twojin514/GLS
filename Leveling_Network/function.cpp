#include "function.h"


void ReadInputLineFile(std::string FileName, std::vector<Line>& lines_data)
{
    std::ifstream file(FileName);
    std::string line_header;
    std::string line_data;
    std::vector<Line> vetor_lines;

    if (!file.is_open())
    {
        printf("Error : Input Line File not found\n");
        return; // 파일 열기 실패 시 함수 종료
    }

    std::getline(file, line_header); // 첫 줄은 헤더이므로 읽지 않음

    while (std::getline(file, line_data))
    {
        std::istringstream iss(line_data); 
        Line line;
        std::string token;
        std::getline(iss, token, ',');
        line.line_number = std::stoi(token);
        std::getline(iss, token, ',');
        line.id_to = token;
        std::getline(iss, token, ',');
        line.id_from = token;
        std::getline(iss, token, ',');
        line.elevation_difference = std::stod(token);
        std::getline(iss, token, ',');
        line.standard_deviation = std::stod(token);
        vetor_lines.push_back(line);
    }

    lines_data = vetor_lines;
}

void ReadInputBMFile(std::string FileName, std::vector<Benchmark>& benchmarks_data)
{
    std::ifstream file(FileName);
    std::string benchmark_header;
    std::string benchmark_data;
    std::vector<Benchmark> vetor_benchmarks;

    if (!file.is_open())
    {
        printf("Error : Input Benchmark File not found\n");
        return; // 파일 열기 실패 시 함수 종료
    }

    std::getline(file, benchmark_header); // 첫 줄은 헤더이므로 읽지 않음

    while (std::getline(file, benchmark_data))
    {
        std::istringstream iss(benchmark_data);
        Benchmark benchmark;
        std::string token;
        std::getline(iss, token, ',');
        benchmark.benchmark_number = std::stoi(token);
        std::getline(iss, token, ',');
        benchmark.id = token;
        std::getline(iss, token, ',');
        benchmark.elevation = std::stod(token);
        std::getline(iss, token, ',');
        benchmark.standard_deviation = std::stod(token);
        vetor_benchmarks.push_back(benchmark);
    }

    benchmarks_data = vetor_benchmarks;
}
