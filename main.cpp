#include <stdio.h>
#include <format>
#include <cmath>
#include <vector>
#include <fstream>


using namespace std;

// file processing functions
void write_file(string path, vector<float>& u) {
	ofstream fout(path, ios_base::binary);
	fout.write((char*)u.data(), sizeof(float) * u.size());
	fout.close();
}

void read_file(string path, vector<float>& c) {
	ifstream fin(path, ios_base::binary);
	fin.read((char*)c.data(), sizeof(float) * c.size());
	fin.close();
}

// structure of problem parameters
struct parameters {
public:
	size_t Nx, Ny, Nt;  // grid sizes
	float hx, hy, tau;  // grid steps
	vector<float> given_c;  // speed of sound distribution
	float f, xs, ys;  // source parameters
	size_t n_save;  // frequency of the frame saving

	parameters(string general_file_path, string c_file_path) {
		ifstream instream(general_file_path);
		instream >> this->Nx;
		instream >> this->Ny;
		instream >> this->Nt;
		instream >> this->hx;
		instream >> this->hy;
		instream >> this->tau;
		instream >> this->f;
		instream >> this->f;
		instream >> this->xs;
		instream >> this->ys;
		instream >> this->n_save;
		instream.close();

		this->given_c = vector<float>(this->Nx * this->Ny);
		read_file(c_file_path, this->given_c);
	}

	~parameters() {}
};


constexpr float PI = 3.14159265358979323846;

float ricker_wavelet(float t, float main_freq) {
	float arg = pow(PI * main_freq * (t - 2.0 / main_freq), 2);
	return (1 - 2 * arg) * exp(-arg);
}

void make_iteration(parameters wave_params, vector<float>& next_u,
	const vector<float>& curr_u, const vector<float>& prev_u, size_t Nt) {

	//search for the nearest node to the source of the disturbance
	size_t source_i = static_cast<size_t>(round(wave_params.ys / wave_params.hy));
	size_t source_j = static_cast<size_t>(round(wave_params.xs / wave_params.hx));

	for (size_t i = 0; i < wave_params.Ny; ++i) {
		for (size_t j = 0; j < wave_params.Nx; ++j) {
			size_t idx = i * wave_params.Nx + j;

			// Neumann conditions
			float x_diff_prev = (j == 0) ? 
				0.0 : (curr_u[idx] - curr_u[idx - 1]) / wave_params.hx;
			float x_diff_next = (j == wave_params.Nx - 1) ? 
				0.0 : (curr_u[idx + 1] - curr_u[idx]) / wave_params.hx;
			float y_diff_prev = (i == 0) ? 
				0.0 : (curr_u[idx] - curr_u[idx - wave_params.Nx]) / wave_params.hy;
			float y_diff_next = (i == wave_params.Ny - 1) ? 
				0.0 : (curr_u[idx + wave_params.Nx] - curr_u[idx]) / wave_params.hy;
			
			// Laplace operator approximation by first-order derivatives
			float laplace = (y_diff_next - y_diff_prev) / wave_params.hy +
				(x_diff_next - x_diff_prev) / wave_params.hx;

			// difference scheme
			float scheme_approximation = 2 * curr_u[idx] - prev_u[idx] +
				pow(wave_params.tau * wave_params.given_c[idx], 2) * laplace;
			if (i == source_i && j == source_j) {
				scheme_approximation += pow(wave_params.tau, 2) * 
					ricker_wavelet(Nt * wave_params.tau, wave_params.f) /
					(wave_params.hx * wave_params.hy);
			}
			next_u[idx] = scheme_approximation;
		}
	}
}


void main() {
	string file_path = "C:\\Users\\Georgiy\\Desktop\\C++ course\\lectures\\лекция 2\\домашка_1";
	string input_name = "\\input.txt";
	string c_name = "\\c.raw";
	parameters wave_params = parameters(file_path + input_name, file_path + c_name);

	vector<float> prev_u(wave_params.Nx * wave_params.Ny, 0.0);
	vector<float> curr_u(wave_params.Nx * wave_params.Ny, 0.0);
	vector<float> next_u(wave_params.Nx * wave_params.Ny, 0.0);

	// solution iterations
	for (size_t i = 0; i < wave_params.Nt; i++) {
		make_iteration(wave_params, next_u, curr_u, prev_u, i);
		swap(next_u, curr_u);
		swap(next_u, prev_u);

		// output in file
		if (i % wave_params.n_save == 0) {
			string output_name = format("\\output_{}.raw", i);
			write_file(file_path + output_name, curr_u);
		}
	}

}
