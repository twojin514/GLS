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
        return; // ���� ���� ���� �� �Լ� ����
    }

    std::getline(file, line_header); // ù ���� ����̹Ƿ� ���� ����

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

    // ����: �Լ��� ��� �Ķ���Ϳ� ������ ����
    lines = vetor_lines;
}