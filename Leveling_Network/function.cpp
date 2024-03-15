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

void PrintInputData(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data)
{
    printf("\nLine, ID(To), ID(From), Elevation_Difference, Standard Deviation(m)\n");
    for (int i = 0; i < lines_data.size(); i++)
    {
		printf("%d, %s, %s, %f, %f\n", lines_data[i].line_number, lines_data[i].id_to.c_str(), lines_data[i].id_from.c_str(), lines_data[i].elevation_difference, lines_data[i].standard_deviation);
	}


	printf("\nNumber, ID, Elevation_Difference, Standard Deviation(m)\n");
    for (int i = 0; i < benchmarks_data.size(); i++)
    {
		printf("%d, %s, %f, %f\n", benchmarks_data[i].benchmark_number, benchmarks_data[i].id.c_str(), benchmarks_data[i].elevation, benchmarks_data[i].standard_deviation);
	}

    printf("\n");


}

void SortInputData(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int& point_number, std::vector<std::string>& point_names)
{
    std::vector<std::string> point_names_temp;

    // Collect point names from lines_data and benchmarks_data
    for (const auto& line : lines_data) {
        point_names_temp.push_back(line.id_from);
        point_names_temp.push_back(line.id_to);
    }
    for (const auto& benchmark : benchmarks_data) {
        point_names_temp.push_back(benchmark.id);
    }

    // Sort point_names_temp
    std::sort(point_names_temp.begin(), point_names_temp.end());

    // Remove duplicates
    point_names_temp.erase(std::unique(point_names_temp.begin(), point_names_temp.end()), point_names_temp.end());

    // Update point_number and point_names
    point_number = point_names_temp.size();
    point_names = point_names_temp;

    // Print sorted point names
    for (const auto& str : point_names_temp) {
        std::cout << str << std::endl;
    }
}

void SetXMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& n_measurement_point, std::vector<std::string>& point_names, std::vector<std::vector<std::string>>& x_matrix)
{
    std::vector<std::vector<std::string>> new_x_matrix (n_measurement_point, std::vector<std::string>(1));
    for (int i = 0; i < n_measurement_point; ++i)
    {
        new_x_matrix[i][0] = point_names[i];
	}

    x_matrix = new_x_matrix;


}

void SetAMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, int32_t& n_measurement_point, std::vector<std::vector<std::string>>& x_matrix, std::vector<std::vector<std::int32_t>>& a_matrix)
{
    std::vector<std::vector<std::int32_t>> new_a_matrix(m_observation_point, std::vector<std::int32_t>(n_measurement_point));
    
    for (int i = 0; i < lines_data.size(); ++i) {
        for (int j = 0; j < n_measurement_point; ++j) {
            if (lines_data[i].id_from == x_matrix[j][0]) {
                new_a_matrix[i][j] = -1;
            }
            
            else if (lines_data[i].id_to == x_matrix[j][0]) {
                new_a_matrix[i][j] = 1;
            }
            else {
                new_a_matrix[i][j] = 0;
            }
        }
    }



    for (int i = lines_data.size(); i < m_observation_point; ++i) // 수정된 부분: lines_data의 크기 대신 m_observation_point 사용
    {
        for (int j = 0; j < n_measurement_point; ++j)
        {
            if (benchmarks_data[i - lines_data.size()].id == x_matrix[j][0])
            {
				new_a_matrix[i][j] = 1;
			}
            else
            {
				new_a_matrix[i][j] = 0;
			}
		}
	}


    a_matrix = new_a_matrix;
}

void SetLMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, std::vector<std::vector<float>>& l_matrix)
{
    	std::vector<std::vector<float>> new_l_matrix(m_observation_point, std::vector<float>(1));
        for (int i = 0; i < lines_data.size(); ++i)
        {
		new_l_matrix[i][0] = lines_data[i].elevation_difference;
	}

        for (int i = lines_data.size(); i < m_observation_point; ++i)
        {
		new_l_matrix[i][0] = benchmarks_data[i - lines_data.size()].elevation;
	}

       // printf("L Matrix\n");
         for (int i = 0; i < m_observation_point; ++i)
         {
             printf("%f\n", new_l_matrix[i][0]);
         }


	l_matrix = new_l_matrix;
}

void SetWMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, std::vector<std::vector<double>>& w_matrix)
{
    std::vector<std::vector<double>> new_w_matrix(m_observation_point, std::vector<double>(m_observation_point));
    for (int i = 0; i < m_observation_point; ++i)
    {
        for (int j = 0; j < m_observation_point; ++j)
        {
            if (i == j)
            {
                if (i < lines_data.size())
                {
					new_w_matrix[i][j] = 1./(lines_data[i].standard_deviation * lines_data[i].standard_deviation);
				}
                else
                {
					new_w_matrix[i][j] = 1./(benchmarks_data[i - lines_data.size()].standard_deviation * benchmarks_data[i - lines_data.size()].standard_deviation);
				}
			}
            else
            {
				new_w_matrix[i][j] = 0;
			}
		}
	}

	w_matrix = new_w_matrix;
}


