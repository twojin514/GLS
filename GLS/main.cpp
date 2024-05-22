#include "function.h"

int main(int argc, char* argv[])
{
	/*********************************************************************
	 0.  PATH 설정 (입력 및 출력 자료)
	**********************************************************************/

	std::string input_file_dir = "..\\Workorder\\";
	std::string input_information = input_file_dir + "workorder.txt";

	std::string title, business_information, transformation_model, name, input_data, output_data;

	/*********************************************************************
	 1.  입력 자료 읽기(작업지시서)
	**********************************************************************/
	ReadInformationFile(input_information, title, business_information, transformation_model, name, input_data, output_data); // 입력 자료 읽기

	std::vector<Point> points;
	ReadPointFile(input_data, points);

	std::vector<Point> control_points;
	std::vector<Point> measured_points;

	std::ofstream outfile(output_data);  // 출력 자료 생성

	outfile << std::fixed << std::setprecision(5);
	std::cout << std::fixed << std::setprecision(5);

	/*********************************************************************
	1.1 측점 분류
	*********************************************************************/
	// 우선 기준점로만 최소제곱 수행
	SetControlData(points, control_points);
	SetMeasuredData(points, measured_points);

	/*********************************************************************
	1.2 입력자료 확인
	*********************************************************************/
	PrintInformation(outfile, title, business_information, transformation_model, name, points, control_points, measured_points);

	/*********************************************************************
	1.3  초기 변환계수 설정
	**********************************************************************/
	double a0 = 0;
	double b0 = 0;
	double c0 = 0;
	double d0 = 0;

	outfile << "\n초기 변환계수 설정( Auto : 1 || Input : 2 )\n";
	std::cout << "\n초기 변환계수 설정( Auto : 1 || Input : 2 )\n";
	int32_t input_setting;
	std::cin >> input_setting;
	if (input_setting == 1) {
		a0 = 0;
		b0 = 0;
		c0 = 0;
		d0 = 0;

		CalculateInitial(a0, b0, c0, d0, control_points);
	}
	else {
		double a0, b0, c0, d0;
		std::cout << "a0 : ";
		std::cin >> a0;
		std::cout << "b0 : ";
		std::cin >> b0;
		std::cout << "c0 : ";
		std::cin >> c0;
		std::cout << "d0 : ";
		std::cin >> d0;
	}

	// 초기 변환계수 저장
	std::vector<double> initial_coefficients = { a0, b0, c0, d0 };
	std::vector<double> update_coefficients = { a0, b0, c0, d0 };

	outfile << "\n ************************************* 초기 변환계수 설정 **************************************\n";
	outfile << " a0 : " << a0 << "\n" << " b0 : " << b0 << "\n" << " c0 : " << c0 << "\n" << " d0 : " << d0 << "\n";

	std::cout << "\n ************************************* 초기 변환계수 설정 **************************************\n";
	std::cout << " a0 : " << a0 << "\n" << " b0 : " << b0 << "\n" << " c0 : " << c0 << "\n" << " d0 : " << d0 << "\n";

	/*********************************************************************
	2. 일반최소제곱법 수행
	*********************************************************************/

	/*********************************************************************
	2.1 반복계산 (①	최대반복계산수)
	*********************************************************************/
	cv::Mat we_matrix, w_matrix, b_matrix, q_matrix, j_matrix, k_matrix, x_matrix, ve_matrix, v_matrix, sigma_xx, sigma_ll, So_square, So_list;
	So_list = cv::Mat::zeros(100, 1, CV_64F);
	int iteration;
	for (iteration = 0; iteration < 100; ++iteration) {

		// Matrix 설정
		SetWMatrix(initial_coefficients, control_points, w_matrix, we_matrix, b_matrix, q_matrix); // 동등무게 설정
		SetJMatirx(control_points, j_matrix); // 미지수 증분의 계수행렬 설정
		SetKMatrix(initial_coefficients, control_points, k_matrix); // 관측방정식 F에 초기값을 대입하였을 때의 F 값
		SetXMatrix(x_matrix, k_matrix, j_matrix, we_matrix, b_matrix); // 미지수 증분 행렬
		SetVMatrix(ve_matrix, v_matrix, control_points, w_matrix , we_matrix, b_matrix, k_matrix, j_matrix, x_matrix, q_matrix); // 잔차행렬

		// 분산 계산
		CalculateSoSquare(So_square, control_points, ve_matrix, we_matrix); 		So_list.at<double>(iteration, 0) = sqrt(So_square.at<double>(0, 0)); // 단위무게 표준편차 
		CalculateSigmaXX(sigma_xx, So_square, j_matrix, we_matrix); // 최학값의 분산-공분산 행렬
		CalculateSigmaLL(sigma_ll, sigma_xx, control_points, j_matrix); // 조정된 관측값의 분산-공분산 행렬
		PrintIteration(outfile, iteration, control_points, initial_coefficients, x_matrix, v_matrix, So_square);

		// 반복 중단 조건
		// 가장 큰 조정값이 어느 정도 미만으로 작아지면 종료 (정상종료)
		double max_diff = 0.0;
		for (int i = 0; i < 4; ++i) {
			double diff = abs(x_matrix.at<double>(i, 0));

			if(diff > max_diff) max_diff = diff;
			}

		if (max_diff < 0.000001) {
			outfile << "\n************************************* 3. 조정 종료 **************************************\n";
			outfile << "조정 종료 : ② 가장 큰 조정값이 어느 정도 미만으로 작아지면 종료 (정상종료)\n";
			std::cout << "\n************************************* 3. 조정 종료 ****************************************\n";
			std::cout << "조정 종료 : ② 가장 큰 조정값이 어느 정도 미만으로 작아지면 종료 (정상종료)\n";
			UpdateCoefficients(initial_coefficients, x_matrix, control_points, v_matrix);

			break;
		}

		// 단위무게 RMSE(So) 변화율 점검 (정상종료)
		else if (iteration > 1 && ((sqrt(So_square.at<double>(0, 0)) - So_list.at<double>(iteration - 1, 0)) / So_list.at<double>(iteration - 1, 0)) <= 0.000001)
		{
			outfile << "\n************************************* 3. 조정 종료 **************************************\n";
			outfile << "조정 종료 : ③ 단위무게 RMSE(So) 변화율 점검 (정상종료)\n";
			std::cout << "\n************************************* 3. 조정 종료 ****************************************\n";
			std::cout << "조정 종료 : ③ 단위무게 RMSE(So) 변화율 점검 (정상종료)\n";
			UpdateCoefficients(initial_coefficients, x_matrix, control_points, v_matrix);

			break;
		}

		// 최대반복계산수 (비정상종료)
		else if (iteration == 100)
		{
			outfile << "\n************************************* 3. 조정 종료 **************************************\n";
			outfile << "조정 종료 : ①	최대반복계산수 (비정상종료)\n";
			std::cout << "\n************************************* 3. 조정 종료 ****************************************\n";
			std::cout << "조정 종료 : ①	최대반복계산수 (비정상종료)\n";
			UpdateCoefficients(initial_coefficients, x_matrix, control_points, v_matrix);

			break;
		}

		// So가 연속적으로 증가 (비정상종료)
		else if (iteration >= 2 && (sqrt(So_square.at<double>(0, 0)) - So_list.at<double>(iteration - 1, 0)) > 0) {
			if (So_list.at<double>(iteration - 1, 0) - So_list.at<double>(iteration - 2, 0) > 0)
			{
				outfile << "\n************************************* 3. 조정 종료 **************************************\n";
				outfile << "조정 종료 : ④	So가 연속적으로 증가 (비정상종료)\n";
				std::cout << "\n************************************* 3. 조정 종료 ****************************************\n";
				std::cout << "조정 종료 : ④	So가 연속적으로 증가 (비정상종료)\n";
				UpdateCoefficients(initial_coefficients, x_matrix, control_points, v_matrix);

				break;
			}
		}

		// 2.3 계수 업데이트
		UpdateCoefficients(initial_coefficients, x_matrix, control_points, v_matrix);

	}

	PrintFinal(outfile, iteration, initial_coefficients, control_points, x_matrix, v_matrix, sigma_xx, sigma_ll, measured_points);


	outfile.close();

	return 0;
}

