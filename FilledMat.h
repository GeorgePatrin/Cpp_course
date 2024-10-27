#include "Matrix.h"


class FilledMat : Matrix {
private:
	double* data = nullptr;

public:
	FilledMat() = default;

	FilledMat(size_t M, size_t N, double* new_data) {
		swap(data, new_data);
		fill_base(M, N);
	}

	void read_from_file(string file_path) {
		ifstream instream(file_path);
		instream >> M >> N;
		size_t full_size = M * N;
		data = new double[full_size];
		size_t nz_counter = 0;
		for (size_t i = 0; i < full_size; i++) {
			instream >> data[i];
			if (data[i] != 0) {
				nz_counter++;
			}
		}
		instream.close();
	}

	FilledMat(const FilledMat& other) {
		delete[] data;
		fill_base(other.M, other.N);
		size_t full_size = M * N;
		data = new double[full_size];
		memcpy(data, other.data, sizeof(double) * full_size);
	}

	FilledMat(FilledMat&& other) noexcept {
		fill_base(other.M, other.N);
		swap(data, other.data);
	}

	FilledMat& operator=(const FilledMat& other) {
		delete[] data;
		fill_base(other.M, other.N);
		size_t full_size = M * N;
		data = new double[full_size];
		memcpy(data, other.data, sizeof(double) * full_size);
		return *this;
	}

	FilledMat& operator=(FilledMat&& other) noexcept {
		fill_base(other.M, other.N);
		swap(data, other.data);
		return *this;
	}

	void print_info() const {
		cout << "rows number: " << M << endl;
		cout << "columns number: " << N << endl;
		cout << "data (reshaped to 2D form): " << endl;

		char separator = '\t';
		for (size_t i = 0; i < M; i++) {
			for (size_t j = 0; j < N; j++) {
				cout << data[i * N + j] << separator;
			}
			cout << endl;
		}
		cout << endl;
	}

	FilledMat slice(index_tuple rows_ids, index_tuple columns_ids) const {
		size_t i1 = rows_ids.id1;
		size_t i2 = rows_ids.id2;
		size_t j1 = columns_ids.id1;
		size_t j2 = columns_ids.id2;

		size_t my_M = i2 - i1;
		size_t my_N = j2 - j1;
		double* new_data = new double[my_M * my_N];
		for (size_t idx = 0; idx < M; idx++)
			memcpy(new_data + idx * N, data + N * (i1 + idx) + j1, sizeof(double) * my_N);

		FilledMat my_fmat = FilledMat(my_M, my_N, new_data);
		return my_fmat;
	}

	void insert(index_tuple elem_coords, double value) {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Insert error: Coordinates out of matrix" };
		data[elem_coords.id1 * N + elem_coords.id2] = value;
	}

	double pop(index_tuple elem_coords) {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Pop error: Coordinates out of matrix" };
		double res_val = data[elem_coords.id1 * N + elem_coords.id2];
		data[elem_coords.id1 * N + elem_coords.id2] = 0.0;
		return res_val;
	}

	double get_elem(index_tuple elem_coords) const {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Get error: Coordinates out of matrix" };
		return data[elem_coords.id1 * N + elem_coords.id2];
	}

	~FilledMat() {
		delete[] data;
	}
};