#include "function.h"


void ReadInputLineFile(std::string FileName, std::vector<Line>& lines)
{
    std::ifstream file(FileName);
    std::string line_header;
    std::string line_data;
    std::vector<Line> vetor_lines;

    if (!file.is_open())
    {
        printf("Error: Input Line File not found\n");
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

    // 변경: 함수의 출력 파라미터에 데이터 저장
    lines = vetor_lines;
}