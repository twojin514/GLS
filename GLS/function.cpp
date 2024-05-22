#include "function.h"

void ReadInformationFile(std::string FileName, std::string& title, std::string& business_information, std::string& transformation_model, std::string& name, std::string& input_data, std::string& output_data)
{
    std::ifstream file(FileName);
    char buffer[256];

    if (!file.is_open())
    {
		printf("Error : Information File not found\n");
		return; // 파일 열기 실패 시 함수 종료
	}

    else {
        file >> buffer;
        if (strncmp(buffer, "Title",5) != 0) {
			printf("Error : Title not found\n");
			return;
		}
        file >> title;

        file >> buffer;
        if (strncmp(buffer, "Business_Information", 20) != 0) {
            printf("Error : Business Information not found\n");
            return;
        }
        file >> business_information;

        file >> buffer;
        if (strncmp(buffer, "Transformation_Model", 20) != 0) {
            printf("Error : Transformation Model not found\n");
            return;
        }
        file >> transformation_model;

		file >> buffer;
        if (strncmp(buffer, "Name",4) != 0) {
            printf("Error : Name not found\n");
            return;
        }
        file >> name;

        file >> buffer;
        if (strncmp(buffer, "Input_Data", 10) != 0) {
			printf("Error : Input Data not found\n");
			return;
		}
        file >> input_data;

		file >> buffer;
        if (strncmp(buffer, "Output_Data", 11) != 0) {
            printf("Error : Output Data not found\n");
            return;
        }
        file >> output_data;
    }
}

void ReadPointFile(std::string FileName, std::vector<Point>& points_data)
{
    std::ifstream file(FileName);
	std::string point_header;
	std::string point_data;
	std::vector<Point> vetor_points;

    if (!file.is_open())
    {
		printf("Error : Point File not found\n");
		return; // 파일 열기 실패 시 함수 종료
	}

	std::getline(file, point_header); // 첫 줄은 헤더이므로 읽지 않음

    while (std::getline(file, point_data))
    {
		std::istringstream iss(point_data);
		Point point;
		std::string token;
		std::getline(iss, token, ',');
		point.id = token;
		std::getline(iss, token, ',');
		point.old_x = std::stod(token);
		std::getline(iss, token, ',');
		point.old_y = std::stod(token);
        std::getline(iss, token, ',');
        point.old_Sx = token.empty() ? NULL : std::stod(token);
        std::getline(iss, token, ',');
        point.old_Sy = token.empty() ? NULL : std::stod(token);
        std::getline(iss, token, ',');
        point.new_X = token.empty() ? NULL : std::stod(token);
        std::getline(iss, token, ',');
        point.new_Y = token.empty() ? NULL : std::stod(token);
        std::getline(iss, token, ',');
        point.new_SX = token.empty() ? NULL : std::stod(token);
        std::getline(iss, token, ',');
        point.new_SY = token.empty() ? NULL : std::stod(token);

		vetor_points.push_back(point);
	}

	points_data = vetor_points;

}

// 입력된 데이터를 정렬하고 측점의 수를 계산하는 함수
void SortInputData(std::vector<Point>& point_data, std::int32_t& point_num, std::vector<std::string>& point_names)
{
	//std::vector<std::string> point_names_temp;

	//// Collect point names from lines_data and benchmarks_data
	//for (const auto& point : point_data) {
	//	point_names_temp.push_back(point.id);
	//}

 //   std::sort(point_names_temp.begin(), point_names_temp.end()); // 공백, 기호, 문자, 숫자, 영어, 한글 순으로 정렬
 //   point_names_temp.erase(std::unique(point_names_temp.begin(), point_names_temp.end()), point_names_temp.end()); // 중복 제거

	//point_names = point_names_temp;
 //   point_num = point_names_temp.size();

    //point_data의 id를 기준으로 point_data를 정렬한다
    std::sort(point_data.begin(), point_data.end(), [](const Point& a, const Point& b) {
		return a.id < b.id;
	});
    point_num = point_data.size();
    for (int i = 0; i < point_num; ++i)
    {
		point_names.push_back(point_data[i].id);
	}

    //

}

void SetControlData(std::vector<Point>& point_data, std::vector<Point>& control_point_data)
{
    std::vector<Point> control_points;
    for (const auto& point : point_data) {
        if (point.new_X != NULL && point.new_Y != NULL && point.new_SX != NULL && point.new_SY != NULL) {
		control_points.push_back(point);
	    }
    }

	control_point_data = control_points;
}

void SetMeasuredData(std::vector<Point>& point_data, std::vector<Point>& measured_point_data)
{
    std::vector<Point> measured_points;
    for (const auto& point : point_data) {
        if (point.new_X == NULL || point.new_Y == NULL || point.new_SX == NULL || point.new_SY == NULL) {
			measured_points.push_back(point);
		}
	}

	measured_point_data = measured_points;
}

void PrintInformation(std::ofstream& outfile, std::string& title, std::string& business_information, std::string& transformation_model, std::string& name, std::vector<Point>& points_data, std::vector<Point>& control_data, std::vector<Point>& measured_data)
{
    outfile << std::fixed << std::setprecision(5);
    std::cout << std::fixed << std::setprecision(5); // 소수점 4자리까지 출력

    std::int32_t n_measurement_point = 4; // 미지점의 개수
    std::int32_t m_observation_point = 2 * control_data.size(); // 관측 수

    outfile << "**********************************************************************************************\n";
    outfile << "************************************ " << title << " ******************************\n";
    outfile << "**************************** " << business_information << " : " << transformation_model << " *************************\n";
    outfile << "**********************************************************************************************\n";
    outfile << "***************************************************** 작성자 : " << name << " **********************\n";
    std::cout << "**********************************************************************************************\n";
    std::cout << "************************************* " << title << " *********************************\n";
    std::cout << "***************************** " << business_information << " : " << transformation_model << " *****************************\n";
    std::cout << "**********************************************************************************************\n";
    std::cout << "***************************************************** 작성자 : " << name << " ***********************\n";

    time_t timer;
    struct tm curr_timer;
    timer = time(NULL);

    outfile << "************************************************ ";
    std::cout << "***************************************************** ";
    if (localtime_s(&curr_timer, &timer) == 0) {
        outfile << "조정시각 : " << curr_timer.tm_year + 1900 << "년 " << curr_timer.tm_mon + 1 << "월 " << curr_timer.tm_mday << "일" << curr_timer.tm_hour << "시 " << curr_timer.tm_min << "분 " << curr_timer.tm_sec << "초\n";
        std::cout << "조정시각 : " << curr_timer.tm_year + 1900 << "년 " << curr_timer.tm_mon + 1 << "월 " << curr_timer.tm_mday << "일" << curr_timer.tm_hour << "시 " << curr_timer.tm_min << "분 " << curr_timer.tm_sec << "초\n";
    }

    else {
        outfile << "시간 정보를 가져오는 데 실패했습니다.\n";
        std::cout << "시간 정보를 가져오는 데 실패했습니다.\n";
    }

    outfile << "**********************************************************************************************\n\n";
    std::cout << "**********************************************************************************************\n\n";

    outfile << "\n*************************************** 1. 입력 자료 ***************************************\n";
    std::cout << "\n*************************************** 1. 입력 자료 ****************************************\n";
    outfile << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    std::cout << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    for (int i = 0; i < points_data.size(); ++i) {
        if(points_data[i].old_Sx==NULL || points_data[i].old_Sy == NULL || points_data[i].new_X == NULL || points_data[i].new_Y == NULL || points_data[i].new_SX == NULL || points_data[i].new_SY == NULL) {
            outfile << std::setw(5) << points_data[i].id << "\t"
                << std::setw(21) << points_data[i].old_x << "\t"
                << std::setw(21) << points_data[i].old_y << "\n\n";


			std::cout << std::setw(5) << points_data[i].id << "\t"
				<< std::setw(21) << points_data[i].old_x << "\t"
                << std::setw(21) << points_data[i].old_y << "\n\n";

		}

        else {
			outfile << std::setw(5) << points_data[i].id << "\t"
				<< std::setw(21) << points_data[i].old_x << "\t"
				<< std::setw(21) << points_data[i].old_y << "\t"
				<< std::setw(21) << points_data[i].old_Sx << "\t"
				<< std::setw(21) << points_data[i].old_Sy << "\t"
				<< std::setw(21) << points_data[i].new_X << "\t"
				<< std::setw(21) << points_data[i].new_Y << "\t"
				<< std::setw(21) << points_data[i].new_SX << "\t"
				<< std::setw(21) << points_data[i].new_SY << "\n\n";

			std::cout << std::setw(5) << points_data[i].id << "\t"
                << std::setw(21) << points_data[i].old_x << "\t"
                << std::setw(21) << points_data[i].old_y << "\t"
                << std::setw(21) << points_data[i].old_Sx << "\t"
                << std::setw(21) << points_data[i].old_Sy << "\t"
                << std::setw(21) << points_data[i].new_X << "\t"
                << std::setw(21) << points_data[i].new_Y << "\t"
                << std::setw(21) << points_data[i].new_SX << "\t"
                << std::setw(21) << points_data[i].new_SY << "\n\n";
        }
    }

    outfile << "\n************************************* Control Point Data **************************************\n";
    std::cout << "\n************************************* Control Point Data ****************************************\n";
    outfile << " 기준점 : " << control_data.size() << "\n";
    std::cout << " 기준점 : " << control_data.size() << "\n";
    outfile << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    std::cout << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    for (int i = 0; i < control_data.size(); ++i) {
        outfile << std::setw(5) << points_data[i].id << "\t"
            << std::setw(21) << points_data[i].old_x << "\t"
            << std::setw(21) << points_data[i].old_y << "\t"
            << std::setw(21) << points_data[i].old_Sx << "\t"
            << std::setw(21) << points_data[i].old_Sy << "\t"
            << std::setw(21) << points_data[i].new_X << "\t"
            << std::setw(21) << points_data[i].new_Y << "\t"
            << std::setw(21) << points_data[i].new_SX << "\t"
            << std::setw(21) << points_data[i].new_SY << "\n\n";

        std::cout << std::setw(5) << points_data[i].id << "\t"
            << std::setw(21) << points_data[i].old_x << "\t"
            << std::setw(21) << points_data[i].old_y << "\t"
            << std::setw(21) << points_data[i].old_Sx << "\t"
            << std::setw(21) << points_data[i].old_Sy << "\t"
            << std::setw(21) << points_data[i].new_X << "\t"
            << std::setw(21) << points_data[i].new_Y << "\t"
            << std::setw(21) << points_data[i].new_SX << "\t"
            << std::setw(21) << points_data[i].new_SY << "\n\n";
    }

    outfile << "\n************************************* Measurement Point Data **************************************\n";
    std::cout << "\n************************************* Measurement Point Data ****************************************\n";
    outfile << " 측점 : " << measured_data.size() << "\n";
    std::cout << " 측점 : " << measured_data.size() << "\n";
    outfile << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\n\n";



        std::cout << std::setw(5) << "\nPoint_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\n\n";


    for (int i = 0; i < measured_data.size(); ++i) {
        outfile << std::setw(5) << measured_data[i].id << "\t"
            << std::setw(21) << measured_data[i].old_x << "\t"
            << std::setw(21) << measured_data[i].old_y << "\n\n";

        std::cout << std::setw(5) << measured_data[i].id << "\t"
            << std::setw(21) << measured_data[i].old_x << "\t"
            << std::setw(21) << measured_data[i].old_y << "\n\n";
    }
}

void PrintIteration(std::ofstream& outfile, int32_t iteration, std::vector<Point>& control_data, std::vector<double>& coefficients, cv::Mat& x_matrix, cv::Mat& v_matrix, cv::Mat& So_square)
{
    outfile << std::fixed << std::setprecision(5);
    std::cout << std::fixed << std::setprecision(5); // 소수점 5자리까지 출력

     std::vector<char> parameter = { 'a', 'b', 'c', 'd' };
    outfile << "\n************************************* Iteration : " << iteration + 1<<" **************************************\n";
    outfile << "2.1 초기값에 대한 조정량\n\n";
    outfile << std::setw(5) << "Parameter" << "\t|"
        << std::setw(21) << "Initial Value" << "\t|"
        << std::setw(21) << "Adjust Value" << "\t|"
        << std::setw(21) << "Updata Value" << "\n\n";

    for (int i = 0; i < 4; ++i)
    {
        outfile << std::setw(5) << parameter[i] << "\t"
            << std::setw(21) << coefficients[i] << "\t"
            << std::setw(21) << x_matrix.at<double>(i, 0) << "\t"
			<< std::setw(21) << coefficients[i] + x_matrix.at<double>(i, 0) << "\n\n";
	}

    outfile << "2.2 반복계산 기준점 좌표 잔차와 So\n\n";
    outfile << std::setw(5) << "Point_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "V_x" << "\t|"
        << std::setw(21) << "V_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "V_X" << "\t|"
        << std::setw(21) << "V_Y" << "\n\n";

    for (int i = 0; i < control_data.size(); ++i) {
        	outfile << std::setw(5) << control_data[i].id << "\t"
			<< std::setw(21) << control_data[i].old_x << "\t"
			<< std::setw(21) << control_data[i].old_y << "\t"
			<< std::setw(21) << v_matrix.at<double>(2 * i, 0) << "\t"
			<< std::setw(21) << v_matrix.at<double>(2 * i + 1, 0) << "\t"
			<< std::setw(21) << control_data[i].new_X << "\t"
            << std::setw(21) << control_data[i].new_Y << "\t"
            << std::setw(21) << v_matrix.at<double>(2 * i + 2, 0) << "\t"
			<< std::setw(21) << v_matrix.at<double>(2 * i + 3, 0) << "\n\n";
    }

    outfile << "So : " << sqrt(So_square.at<double>(0, 0)) << "\n\n";



    std::cout << "\n************************************* Iteration : " << iteration + 1 << " **************************************\n";
    std::cout << "2.1 초기값에 대한 조정량\n\n";
    std::cout << std::setw(5) << "Parameter" << "\t|"
        << std::setw(21) << "Initial Value" << "\t|"
        << std::setw(21) << "Adjust Value" << "\t|"
        << std::setw(21) << "Updata Value" << "\n\n";

    for (int i = 0; i < 4; ++i)
    {
        std::cout << std::setw(5) << parameter[i] << "\t"
            << std::setw(21) << coefficients[i] << "\t"
            << std::setw(21) << x_matrix.at<double>(i, 0) << "\t"
            << std::setw(21) << coefficients[i] + x_matrix.at<double>(i, 0) << "\n\n";
    }

    std::cout << "2.2 반복계산 기준점 좌표 잔차와 So\n\n";
    std::cout<< std::setw(5) << "Point_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "V_x" << "\t|"
        << std::setw(21) << "V_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "V_X" << "\t|"
        << std::setw(21) << "V_Y" << "\n\n";

    for (int i = 0; i < control_data.size(); ++i) {
		std::cout << std::setw(5) << control_data[i].id << "\t"
            << std::setw(21) << control_data[i].old_x << "\t"
            << std::setw(21) << control_data[i].old_y << "\t"
            << std::setw(21) << v_matrix.at<double>(2 * i, 0) << "\t"
            << std::setw(21) << v_matrix.at<double>(2 * i + 1, 0) << "\t"
            << std::setw(21) << control_data[i].new_X << "\t"
            << std::setw(21) << control_data[i].new_Y << "\t"
            << std::setw(21) << v_matrix.at<double>(2 * i + 2, 0) << "\t"
            << std::setw(21) << v_matrix.at<double>(2 * i + 3, 0) << "\n\n";
	}

    std::cout << "So : " << sqrt(So_square.at<double>(0, 0)) << "\n\n";

}

void PrintFinal(std::ofstream& outfile, std::int32_t iteration , std::vector<double>& coefficients, std::vector<Point>& control_data, cv::Mat& x_matrix, cv::Mat& v_matrix, cv::Mat& sigma_xx_matrix, cv::Mat& sigma_ll_matrix, std::vector<Point>& measure_data)
{
	outfile << std::fixed << std::setprecision(5);
	std::cout << std::fixed << std::setprecision(5); // 소수점 5자리까지 출력



	std::vector<char> parameter = { 'a', 'b', 'c', 'd' };
    outfile<< " Final Iteration : "<< iteration + 1 << "\n\n";
	outfile << "3.1 조정된 계수\n\n";
	outfile << std::setw(5) << "Parameter" << "\t|"
		<< std::setw(21) << "Coefficient" << "\t|"
        << std::setw(21) << "S" << "\n\n";
    for (int i = 0; i < 4; ++i)
    {
        outfile << std::setw(5) << parameter[i] << "\t"
            << std::setw(21) << coefficients[i] << "\t"
            << std::setw(21) << sqrt(sigma_xx_matrix.at<double>(i, i)) << "\n\n";
	}
	outfile << "3.2 조정 후 기준점 좌표\n\n";
	outfile << std::setw(5) << "Point_ID" << "\t|"
		<< std::setw(21) << "x" << "\t|"
		<< std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    for (int i = 0; i < control_data.size(); ++i)
    {
        outfile << std::setw(5) << control_data[i].id << "\t"
            << std::setw(21) << control_data[i].old_x  << "\t"
            << std::setw(21) << control_data[i].old_y  << "\t"
            << std::setw(21) << control_data[i].old_Sx << "\t"
            << std::setw(21) << control_data[i].old_Sy << "\t"
            << std::setw(21) << (control_data[i].old_x ) * coefficients[0] - (control_data[i].old_y) * coefficients[1] + coefficients[2] << "\t"
            << std::setw(21) << (control_data[i].old_x ) * coefficients[1] + (control_data[i].old_y) * coefficients[0] + coefficients[3] << "\t"
            << std::setw(21) << sqrt(sigma_ll_matrix.at<double>(2 * i, 2 * i)) << "\t"
            << std::setw(21) << sqrt(sigma_ll_matrix.at<double>(2 * i + 1, 2 * i + 1)) << "\n\n";
    }
    outfile << "3.3 변환모델\n\n";
    outfile << std::setw(3) << "X = (" << coefficients[0] << ") x" << " - (" << coefficients[1] << ") y" << " + (" << coefficients[2] << ")\n";
    outfile << std::setw(3) << "Y = (" << coefficients[1] << ") x" << " + (" << coefficients[0] << ") y" << " + (" << coefficients[3] << ")\n";

    for (int i = 0; i < measure_data.size(); ++i) {
        measure_data[i].new_X = measure_data[i].old_x * coefficients[0] - measure_data[i].old_y * coefficients[1] + coefficients[2];
        measure_data[i].new_Y = measure_data[i].old_x * coefficients[1] + measure_data[i].old_y * coefficients[0] + coefficients[3];

        measure_data[i].new_SX = sqrt(pow(measure_data[i].old_Sx * coefficients[0], 2) + pow(measure_data[i].old_Sy * coefficients[1], 2));
        measure_data[i].new_SY = sqrt(pow(measure_data[i].old_Sx * coefficients[1], 2) + pow(measure_data[i].old_Sy * coefficients[0], 2));
    }

    outfile << "\n3.4 측점의 변환 좌표\n";
    outfile << std::setw(5) << "Point_ID" << "\t|"
		<< std::setw(21) << "x" << "\t|"
		<< std::setw(21) << "y" << "\t|"
		<< std::setw(21) << "X" << "\t|"
		<< std::setw(21) << "Y" << "\n\n";


    for (int i = 0; i < measure_data.size(); ++i) {
		outfile << std::setw(5) << measure_data[i].id << "\t"
            << std::setw(21) << measure_data[i].old_x << "\t"
            << std::setw(21) << measure_data[i].old_y << "\t"
            << std::setw(21) << measure_data[i].new_X << "\t"
            << std::setw(21) << measure_data[i].new_Y << "\n\n";
	}

    std::cout<< " Final Iteration : " << iteration + 1 << "\n\n";
    std::cout << "3.1 조정된 계수\n\n";
    std::cout << std::setw(5) << "Parameter" << "\t|"
		<< std::setw(21) << "Coefficient" << "\t|"
		<< std::setw(21) << "S_Coefficient" << "\n\n";
    for (int i = 0; i < 4; ++i)
    {
		std::cout << std::setw(5) << parameter[i] << "\t"
			<< std::setw(21) << coefficients[i] << "\t"
			<< std::setw(21) << sqrt(sigma_xx_matrix.at<double>(i, i)) << "\n\n";
	}
    std::cout<< "3.2 조정 후 기준점 좌표\n\n";
    std::cout << std::setw(5) << "Point_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
        << std::setw(21) << "y" << "\t|"
        << std::setw(21) << "S_x" << "\t|"
        << std::setw(21) << "S_y" << "\t|"
        << std::setw(21) << "X" << "\t|"
        << std::setw(21) << "Y" << "\t|"
        << std::setw(21) << "S_X" << "\t|"
        << std::setw(21) << "S_Y" << "\n\n";

    for (int i = 0; i < control_data.size(); ++i)
    {
        		std::cout << std::setw(5) << control_data[i].id << "\t"
			<< std::setw(21) << control_data[i].old_x << "\t"
			<< std::setw(21) << control_data[i].old_y << "\t"
            << std::setw(21) << control_data[i].old_Sx << "\t"
            << std::setw(21) << control_data[i].old_Sy << "\t"
			<< std::setw(21) << control_data[i].old_x * coefficients[0] - control_data[i].old_y * coefficients[1] + coefficients[2] << "\t"
			<< std::setw(21) << control_data[i].old_y * coefficients[1] + control_data[i].old_y * coefficients[0] + coefficients[3] << "\t"
			<< std::setw(21) << sqrt(sigma_ll_matrix.at<double>(2 * i, 2 * i)) << "\t"
			<< std::setw(21) << sqrt(sigma_ll_matrix.at<double>(2 * i + 1, 2 * i + 1)) << "\n\n";
	
    }
    std::cout << "3.3 변환모델\n\n";
    std::cout << std::setw(3) << "X = (" << coefficients[0] << ") x" << " - (" << coefficients[1] << ") y" << " + (" << coefficients[2] << ")\n";
    std::cout << std::setw(3) << "Y = (" << coefficients[1] << ") x" << " + (" << coefficients[0] << ") y" << " + (" << coefficients[3] << ")\n";

    std::cout << "\n3.4 측점의 변환 좌표\n";
    std::cout<< std::setw(5) << "Point_ID" << "\t|"
        << std::setw(21) << "x" << "\t|"
		<< std::setw(21) << "y" << "\t|"
		<< std::setw(21) << "X" << "\t|"
		<< std::setw(21) << "Y" << "\n\n";

    
    for (int i = 0; i < measure_data.size(); ++i) {
		std::cout << std::setw(5) << measure_data[i].id << "\t"
			<< std::setw(21) << measure_data[i].old_x << "\t"
			<< std::setw(21) << measure_data[i].old_y << "\t"
			<< std::setw(21) << measure_data[i].new_X << "\t"
			<< std::setw(21) << measure_data[i].new_Y << "\n\n";
	}


}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CalculateInitial(double& a, double& b, double& c, double& d, std::vector<Point>& control_data) {
    cv::Mat a_matrix = cv::Mat::zeros(2 * control_data.size(), 4, CV_64F);
    cv::Mat l_matrix = cv::Mat::zeros(2 * control_data.size(), 1, CV_64F);
    cv::Mat x_matrix = cv::Mat::zeros(4, 1, CV_64F);

    for (int i = 0; i < control_data.size(); ++i) {
        a_matrix.at<double>(2 * i, 0) = control_data[i].old_x;
        a_matrix.at<double>(2 * i, 1) = -control_data[i].old_y;
        a_matrix.at<double>(2 * i, 2) = 1;
        a_matrix.at<double>(2 * i, 3) = 0;

        a_matrix.at<double>(2 * i + 1, 0) = control_data[i].old_y;
        a_matrix.at<double>(2 * i + 1, 1) = control_data[i].old_x;
        a_matrix.at<double>(2 * i + 1, 2) = 0;
        a_matrix.at<double>(2 * i + 1, 3) = 1;

        l_matrix.at<double>(2 * i, 0) = control_data[i].new_X;
        l_matrix.at<double>(2 * i + 1, 0) = control_data[i].new_Y;
    }

    x_matrix = (a_matrix.t() * a_matrix).inv() * a_matrix.t() * l_matrix;

    a = x_matrix.at<double>(0, 0);
    b = x_matrix.at<double>(1, 0);
    c = x_matrix.at<double>(2, 0);
    d = x_matrix.at<double>(3, 0);

}

void SetWMatrix(std::vector<double>& initial_coefficients, std::vector<Point>& points, cv::Mat& w_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix, cv::Mat& q_matrix)
{
    std::int32_t points_size = points.size();
    cv::Mat cv_we_matrix = cv::Mat::zeros(2* points_size, 2 * points_size, CV_64F);
    cv::Mat cv_w_matrix = cv::Mat::zeros(4 * points_size, 4 * points_size, CV_64F);
	cv::Mat cv_b_matrix = cv::Mat::zeros(2* points_size, 4* points_size, CV_64F);
	cv::Mat cv_q_matrix = cv::Mat::zeros(4* points_size, 4* points_size, CV_64F);

    // B 행렬 설정
    for (int32_t i = 0; i < points_size; ++i) {
        cv_b_matrix.at<double>(2 * i, i * (points_size - 1)) = initial_coefficients[0];
        cv_b_matrix.at<double>(2 * i, i * (points_size - 1) + 1) = -initial_coefficients[1];
        cv_b_matrix.at<double>(2 * i, i * (points_size - 1) + 2) = -1;
        cv_b_matrix.at<double>(2 * i, i * (points_size - 1) + 3) = 0;

        cv_b_matrix.at<double>(2 * i + 1, i * (points_size - 1)) = initial_coefficients[1];
        cv_b_matrix.at<double>(2 * i + 1, i * (points_size - 1) + 1) = initial_coefficients[0];
        cv_b_matrix.at<double>(2 * i + 1, i * (points_size - 1) + 2) = 0;
        cv_b_matrix.at<double>(2 * i + 1, i * (points_size - 1) + 3) = -1;
    }

    // Q 행렬 설정
    for (int32_t i = 0; i < points_size; ++i) {
        cv_q_matrix.at<double>(i * (points_size - 1), i * (points_size - 1)) = points[i].old_Sx * points[i].old_Sx;
        cv_q_matrix.at<double>(i * (points_size - 1) + 1, i * (points_size - 1) + 1) = points[i].old_Sy * points[i].old_Sy;
        cv_q_matrix.at<double>(i * (points_size - 1) + 2, i * (points_size - 1) + 2) = points[i].new_SX * points[i].new_SX;
        cv_q_matrix.at<double>(i * (points_size - 1) + 3, i * (points_size - 1) + 3) = points[i].new_SY * points[i].new_SY;

    }

    // W 행렬 설정
    for (int32_t i = 0; i < points_size; ++i) {
        cv_w_matrix.at<double>(i * (points_size - 1), i * (points_size - 1)) = 1 / (points[i].old_Sx * points[i].old_Sx);
        cv_w_matrix.at<double>(i * (points_size - 1) + 1, i * (points_size - 1) + 1) = 1 / (points[i].old_Sy * points[i].old_Sy);
        cv_w_matrix.at<double>(i * (points_size - 1) + 2, i * (points_size - 1) + 2) = 1 / (points[i].new_SX * points[i].new_SX);
        cv_w_matrix.at<double>(i * (points_size - 1) + 3, i * (points_size - 1) + 3) = 1 / (points[i].new_SY * points[i].new_SY);

	}


    // We 행렬 설정
    cv_we_matrix = (cv_b_matrix * cv_q_matrix * cv_b_matrix.t()).inv();

    w_matrix= cv_w_matrix;
	we_matrix = cv_we_matrix;
	b_matrix = cv_b_matrix;
	q_matrix = cv_q_matrix;
}

void SetJMatirx(std::vector<Point>& points, cv::Mat& j_martix)
{
    std::int32_t points_size = points.size();
	cv::Mat cv_j_matrix = cv::Mat::zeros(2 * points_size, 4, CV_64F);

    // J 행렬 설정
    for (int32_t i = 0; i < points_size; ++i) {
        cv_j_matrix.at<double>(2 * i, 0) = points[i].old_x;
        cv_j_matrix.at<double>(2 * i, 1) = -points[i].old_y;
        cv_j_matrix.at<double>(2 * i, 2) = 1;
        cv_j_matrix.at<double>(2 * i, 3) = 0;

        cv_j_matrix.at<double>(2 * i + 1, 0) = points[i].old_y;
        cv_j_matrix.at<double>(2 * i + 1, 1) = points[i].old_x;
        cv_j_matrix.at<double>(2 * i + 1, 2) = 0;
        cv_j_matrix.at<double>(2 * i + 1, 3) = 1;
	}

	j_martix = cv_j_matrix;
}

void SetKMatrix(std::vector<double>& initial_coefficients, std::vector<Point>& points, cv::Mat& k_matrix)
{
    std::int32_t points_size = points.size();
	cv::Mat cv_k_matrix = cv::Mat::zeros(2 * points_size, 1, CV_64F);

	// K 행렬 설정
    for (int32_t i = 0; i < points_size; ++i) {
        cv_k_matrix.at<double>(2 * i, 0) = points[i].new_X - (initial_coefficients[0] * points[i].old_x - initial_coefficients[1] * points[i].old_y + initial_coefficients[2]);
        cv_k_matrix.at<double>(2 * i + 1, 0) = points[i].new_Y - (initial_coefficients[1] * points[i].old_x + initial_coefficients[0] * points[i].old_y + initial_coefficients[3]);
	}

	k_matrix = cv_k_matrix;
}

void SetXMatrix(cv::Mat& x_matrix, cv::Mat& k_matrix, cv::Mat& j_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix)
{
    cv::Mat cv_x_matrix = cv::Mat::zeros(4, 1, CV_64F);
    cv_x_matrix = (j_matrix.t() * we_matrix * j_matrix).inv() * (j_matrix.t() * we_matrix * k_matrix);

	x_matrix = cv_x_matrix;
}

void SetVMatrix(cv::Mat& ve_matrix, cv::Mat& v_matrix, std::vector<Point> points, cv::Mat& w_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix, cv::Mat& k_matrix, cv::Mat& j_matrix, cv::Mat& x_matrix, cv::Mat& q_matrix)
{
    std::int32_t points_size = points.size();
	cv::Mat cv_ve_matrix = cv::Mat::zeros(2 * points_size, 1, CV_64F);
	cv::Mat cv_v_matrix = cv::Mat::zeros(4 * points_size, 1, CV_64F);

	cv_ve_matrix = j_matrix * x_matrix - k_matrix; // 동등잔차
	cv_v_matrix = w_matrix.inv() * b_matrix.t() * we_matrix * cv_ve_matrix; // 관측잔차

	ve_matrix = cv_ve_matrix;
	v_matrix = cv_v_matrix;
}

void CalculateSoSquare(cv::Mat& So_square, std::vector<Point> points, cv::Mat& ve_matrix, cv::Mat& we_matrix)
{
    std::int32_t points_size = points.size();
    cv::Mat cv_s_o = cv::Mat::zeros(1, 1, CV_64F);
    cv_s_o = (ve_matrix.t() * we_matrix * ve_matrix) / (2 * points_size - 4);

    So_square = cv_s_o;

}

void CalculateSigmaXX(cv::Mat& sigma_xx, cv::Mat& So_square, cv::Mat& j_matrix, cv::Mat& we_matrix)
{
    cv::Mat cv_sigma_xx = cv::Mat::zeros(4, 4, CV_64F);
	cv_sigma_xx = So_square.at<double>(0, 0) * (j_matrix.t() * we_matrix * j_matrix).inv();

	sigma_xx = cv_sigma_xx;
}

void CalculateSigmaLL(cv::Mat& sigma_ll, cv::Mat& sigma_xx, std::vector<Point> points, cv::Mat& j_matrix)
{
    std::int32_t points_size = points.size();
	cv::Mat cv_sigma_ll = cv::Mat::zeros(2 * points_size, 2 * points_size, CV_64F);
	cv_sigma_ll = j_matrix * sigma_xx * j_matrix.t();

	sigma_ll = cv_sigma_ll;
}

void UpdateCoefficients(std::vector<double>& initial_coefficients, cv::Mat& x_matrix, std::vector<Point>& control_data, cv::Mat& v_matrix)
{
    for (int i = 0; i < initial_coefficients.size(); ++i)
    {
        initial_coefficients[i] += x_matrix.at<double>(i, 0);
    }
}
