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

void PrintLevelingProgram(std::ofstream& outfile, std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, std::vector<std::vector<int32_t>>& a_matrix, std::vector<std::vector<std::string>>& x_matrix, std::vector<std::vector<double>>& l_matrix, std::vector<std::vector<double>>& w_matrix, std::vector<std::vector<double>>& atwa_matrix, std::vector<std::vector<double>>& atwl_matrix)
{
    cv::Mat cv_a_matrix, cv_l_matrix, cv_w_matrix, cv_atwa_matrix, cv_atwl_matrix;

    VectoMatrix(atwa_matrix, cv_atwa_matrix);
    VectoMatrix(atwl_matrix, cv_atwl_matrix);
    VectoMatrix(w_matrix, cv_w_matrix);
    VectoMatrix(a_matrix, cv_a_matrix);
    VectoMatrix(l_matrix, cv_l_matrix);
    


    outfile << "**********************************************************************************************\n";
    outfile << "************************* 고급 조정계산론 - 수준망 조정프로그램 ***********************\n";
    outfile << "**********************************************************************************************\n";
    outfile << "********************************************** 작성자 : 김홍진 *****************************\n";

    time_t timer;
    struct tm curr_timer;
    timer = time(NULL);

    outfile << "********************************************** ";
    if (localtime_s(&curr_timer, &timer) == 0) {
        outfile << "조정시각 : " << curr_timer.tm_year + 1900 << "년 " << curr_timer.tm_mon + 1 << "월 " << curr_timer.tm_mday << "일"<< curr_timer.tm_hour << "시 " << curr_timer.tm_min << "분 " << curr_timer.tm_sec << "초\n";
    }
    else {
        outfile << "시간 정보를 가져오는 데 실패했습니다." << std::endl;
    }
    outfile << "**********************************************************************************************\n\n";


    outfile << "\n*************************************** 1.  입력 자료 ***************************************\n";
    outfile << "************************************* Line Input Data **************************************\n";
    outfile << "\nLine\tID(To)\tID(From)\tElevation_Difference\tStandard Deviation(m)\n";
    for (int i = 0; i < lines_data.size(); i++)
    {
        outfile << lines_data[i].line_number << "\t" << lines_data[i].id_to << "\t" << lines_data[i].id_from << "\t" << lines_data[i].elevation_difference << "\t" << lines_data[i].standard_deviation << "\n";
    }

    outfile<<"\nNumber\tID\tElevation_Difference\tStandard Deviation(m)\n";
    for (int i = 0; i < benchmarks_data.size(); i++)
    {
        outfile << benchmarks_data[i].benchmark_number << "\t" << benchmarks_data[i].id << "\t" << benchmarks_data[i].elevation << "\t" << benchmarks_data[i].standard_deviation << "\n";
    }

    outfile << "\n************************************ 2. 수준망 조정 시작 ***********************************\n\n";
    outfile << "측점의 수 : " << x_matrix.size() << "\t\t"<<"관측 수 : "<<lines_data.size()+benchmarks_data.size()<<"\n\n";
    outfile << "**************************************** Matrix 출력 ***************************************\n\n";

    outfile<<"[ X Matrix ]\n";
    for (int i = 0; i < x_matrix.size(); i++)
    {
		outfile << x_matrix[i][0] << "\n";
	}

    outfile << "\n[ A Matrix ]\n";
    for (int i = 0; i < a_matrix.size(); i++)
    {
        for (int j = 0; j < a_matrix[i].size(); j++)
        {
			outfile << a_matrix[i][j] << " ";
		}
		outfile << "\n";
	}

    outfile << "\n[ L Matrix ]\n";
    for (int i = 0; i < l_matrix.size(); i++)
    {
		outfile << l_matrix[i][0] << "\n";
	}

	outfile << "\n[ W Matrix ]\n";
    for (int i = 0; i < w_matrix.size(); i++)
    {
        for (int j = 0; j < w_matrix[i].size(); j++)
        {
			outfile << w_matrix[i][j] << " ";
		}
		outfile << "\n";
	}

	outfile << "\n[ ATWA Matrix ]\n";
    for (int i = 0; i < atwa_matrix.size(); i++)
    {
        for (int j = 0; j < atwa_matrix[i].size(); j++)
        {
			outfile << atwa_matrix[i][j] << " ";
		}
		outfile << "\n";
	}

	outfile << "\n[ ATWL Matrix ]\n";
    for (int i = 0; i < atwl_matrix.size(); i++)
    {
		outfile << atwl_matrix[i][0] << "\n";
	}



	outfile << "\n\n*************************************** 3. 조정결과 ***************************************\n\n";

    cv::Mat cv_inv_atwa_matrix = cv_atwa_matrix.inv();
    cv::Mat calculate_x_matrix = cv_inv_atwa_matrix * cv_atwl_matrix;

    outfile << "[ X Matrix ]\n";
    for (int i = 0; i < x_matrix.size(); i++)
    {
        for (int j = 0; j < x_matrix[i].size(); j++)
        {
            outfile << x_matrix[i][j] << " : " << calculate_x_matrix.at<double>(i, j) << "\n";
        }
    }

    outfile<< "\n[ V Matrix ]\n";
    cv_a_matrix.convertTo(cv_a_matrix, CV_64F);
    cv::Mat cv_v_matrix = cv_a_matrix* calculate_x_matrix - cv_l_matrix;
    for (int i = 0; i < cv_v_matrix.rows; i++)
    {
		outfile << cv_v_matrix.at<double>(i, 0) << "\n";
	}

	outfile << "\n[ 정확도 ]\n";
	cv::Mat cv_q_matrix = cv_v_matrix.t() * cv_w_matrix * cv_v_matrix;
	outfile << sqrt(cv_q_matrix.at<double>(0, 0) / (lines_data.size() + benchmarks_data.size() - x_matrix.size())) << "\n";
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
	point_names = point_names_temp;
	point_number = point_names_temp.size();

	// Print sorted point names
	//for (const auto& str : point_names_temp) {
	//	std::cout << str << std::endl;
	//}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void SetXMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& n_measurement_point, std::vector<std::string>& point_names, std::vector<std::vector<std::string>>& x_matrix)
{
    std::vector<std::vector<std::string>> new_x_matrix(n_measurement_point, std::vector<std::string>(1));
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

void SetLMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& m_observation_point, std::vector<std::vector<double>>& l_matrix)
{
    	std::vector<std::vector<double>> new_l_matrix(m_observation_point, std::vector<double>(1));
        for (int i = 0; i < lines_data.size(); ++i)
        {
		new_l_matrix[i][0] = lines_data[i].elevation_difference;
	}

        for (int i = lines_data.size(); i < m_observation_point; ++i)
        {
		new_l_matrix[i][0] = benchmarks_data[i - lines_data.size()].elevation;
	}

         //for (int i = 0; i < m_observation_point; ++i)
         //{
         //    printf("%f\n", new_l_matrix[i][0]);
         //}


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

void SetATWAMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& n_measurement_point, std::vector<std::vector<int32_t>>& a_matrix, std::vector<std::vector<double>>& w_matrix, std::vector<std::vector<double>>& atwa_matrix)
{
    std::vector<std::vector<double>> new_atwa_matrix(n_measurement_point, std::vector<double>(n_measurement_point));

    for (int i = 0; i < n_measurement_point; ++i)
    {
        for (int j = 0; j < n_measurement_point; ++j)
        {
			new_atwa_matrix[i][j] = 0;
            for (int k = 0; k < lines_data.size() + benchmarks_data.size(); ++k)
            {
				new_atwa_matrix[i][j] += a_matrix[k][i] * w_matrix[k][k] * a_matrix[k][j];
			}
		}
	}
    
	atwa_matrix = new_atwa_matrix;
}

void SetATWLMatrix(std::vector<Line>& lines_data, std::vector<Benchmark>& benchmarks_data, int32_t& n_measurement_point, std::vector<std::vector<int32_t>>& a_matrix, std::vector<std::vector<double>>& w_matrix, std::vector<std::vector<double>>& l_matrix, std::vector<std::vector<double>>& atwl_matrix)
{
    std::vector<std::vector<double>> new_atwl_matrix(n_measurement_point, std::vector<double>(1));

    for (int i = 0; i < n_measurement_point; ++i)
    {
		new_atwl_matrix[i][0] = 0;
        for (int j = 0; j < lines_data.size() + benchmarks_data.size(); ++j)
        {
			new_atwl_matrix[i][0] += a_matrix[j][i] * w_matrix[j][j] * l_matrix[j][0];
		}
	}

	atwl_matrix = new_atwl_matrix;
}

void VectoMatrix(std::vector<std::vector<double>>& matrix, cv::Mat& mat)
{
    mat = cv::Mat(matrix.size(), matrix[0].size(), CV_64F);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
			mat.at<double>(i, j) = matrix[i][j];
		}
	}
}

void VectoMatrix(std::vector<std::vector<int32_t>>& matrix, cv::Mat& mat)
{
    mat = cv::Mat(matrix.size(), matrix[0].size(), CV_32S);
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[0].size(); j++)
        {
            mat.at<int32_t>(i, j) = (matrix[i][j]);
        }
    }
}




