#include <stdio.h>
#include <fstream>
#include <iostream>

constexpr size_t N = 20;
constexpr size_t M = 30;
constexpr size_t P = 40;

double current_concentration[N][M][P];
double next_concentration[N][M][P];

void main() {
	double D = 1;  // коэффициент диффуции
	double t_step = 0.01;  // шаг по времени
	double x_step = 1.0;  // шаг по x
	double y_step = 1.0;  // шаг по y
	double z_step = 1.0;  // шаг по z
	size_t t_steps_number = 100;  // количесство шагов по времени

	std::ifstream fin("input.raw", std::ios_base::binary);
	fin.read((char*)current_concentration, sizeof(double) * N * M * P);
	fin.close();

	for (size_t i = 0; i < N; i++) {
		for (size_t j = 0; j < M; j++){
			next_concentration[i][j][0] = 0.0;
			next_concentration[i][j][P - 1] = 0.0;
		}
	}
	for (size_t k = 0; k < P; k++) {
		for (size_t j = 0; j < M; j++) {
			next_concentration[0][j][k] = 0.0;
			next_concentration[N - 1][j][k] = 0.0;
		}
	}
	for (size_t i = 0; i < N; i++) {
		for (size_t k = 0; k < P; k++) {
			next_concentration[i][0][k] = 0.0;
			next_concentration[i][M - 1][k] = 0.0;
		}
	}

	for (size_t n = 0; n < t_steps_number; n++) {
		for (size_t i = 1; i < (N - 1); i++) {
			for (size_t j = 1; j < (M - 1); j++) {
				for (size_t k = 1; k < (P - 1); k++)
				{
					double x_term = current_concentration[i + 1][j][k] - 2 * current_concentration[i][j][k] + current_concentration[i - 1][j][k];
					double y_term = current_concentration[i][j + 1][k] - 2 * current_concentration[i][j][k] + current_concentration[i][j - 1][k];
					double z_term = current_concentration[i][j][k + 1] - 2 * current_concentration[i][j][k] + current_concentration[i][j][k - 1];
					next_concentration[i][j][k] = (x_term / x_step / x_step + y_term / y_step / y_step + z_term / z_step / z_step) * D * t_step + current_concentration[i][j][k];
				}
			}
		}
		std::swap(current_concentration, next_concentration);
	}

	std::ofstream fout("output.raw", std::ios_base::binary);
	fout.write((char*)current_concentration, sizeof(double) * N * M * P);
	fout.close();
}
