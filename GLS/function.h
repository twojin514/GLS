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
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <locale>
#include <iomanip>

struct Point
{
	std::string id;
	double old_x;
	double old_y;
	double new_X;
	double new_Y;
	double old_Sx;
	double old_Sy;
	double new_SX;
	double new_SY;
};

void ReadInformationFile(std::string FileName, std::string& title, std::string& business_information, std::string& transformation_model, std::string& name, std::string& input_data, std::string& output_data);
void ReadPointFile(std::string FileName, std::vector<Point>& points_data);
void SortInputData(std::vector<Point>& point_data, std::int32_t& point_num, std::vector<std::string>& point_names);
void SetControlData(std::vector<Point>& point_data, std::vector<Point>& control_point_data);
void SetMeasuredData(std::vector<Point>& point_data, std::vector<Point>& measured_point_data);
void PrintInformation(std::ofstream& outfile, std::string& title, std::string& business_information, std::string& transformation_model, std::string& name, std::vector<Point>& points_data, std::vector<Point>& control_data, std::vector<Point>& measured_data);
void PrintIteration(std::ofstream& outfile, int32_t iteration, std::vector<Point>& control_data, std::vector<double>& coefficients, cv::Mat& x_matrix, cv::Mat& v_matrix, cv::Mat& So_square);
void PrintFinal(std::ofstream& outfile, std::int32_t iteration, std::vector<double>& coefficients, std::vector<Point>& control_data, cv::Mat& x_matrix, cv::Mat& v_matrix, cv::Mat& sigma_xx_matrix, cv::Mat& sigma_ll_matrix, std::vector<Point>& measure_data);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalculateInitial(double& a, double& b, double& c, double& d, std::vector<Point>& control_data);
void SetWMatrix(std::vector<double>& initial_coefficients, std::vector<Point>& points, cv::Mat& w_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix, cv::Mat& q_matrix);
void SetJMatirx(std::vector<Point>& points, cv::Mat& j_martix);
void SetKMatrix(std::vector<double>& initial_coefficients, std::vector<Point>& points, cv::Mat& k_matrix);
void SetXMatrix(cv::Mat& x_matrix, cv::Mat& k_matrix, cv::Mat& j_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix);
void SetVMatrix(cv::Mat& ve_matrix, cv::Mat& v_matrix, std::vector<Point> points, cv::Mat& w_matrix, cv::Mat& we_matrix, cv::Mat& b_matrix, cv::Mat& k_matrix, cv::Mat& j_matrix, cv::Mat& x_matrix, cv::Mat& q_matrix);
void CalculateSoSquare(cv::Mat& s_o, std::vector<Point> points, cv::Mat& ve_matrix, cv::Mat& we_matrix);
void CalculateSigmaXX(cv::Mat& sigma_xx, cv::Mat& s_o, cv::Mat& j_matrix, cv::Mat& we_matrix);
void CalculateSigmaLL(cv::Mat& sigma_ll, cv::Mat& sigma_xx, std::vector<Point> points, cv::Mat& j_matrix);
void UpdateCoefficients(std::vector<double>& initial_coefficients, cv::Mat& x_matrix, std::vector<Point>& control_data, cv::Mat& v_matrix);
