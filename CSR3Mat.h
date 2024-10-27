#include "FilledMat.h"
#include <vector>

class CSR3Mat : Matrix {
private:
	size_t non_zero_size = 0;
	size_t capacity = 0;
	double* values = nullptr;
	size_t* columns = nullptr;
	size_t* row_index = nullptr;

	index_tuple get_element_position(size_t elem_index) const {
		size_t j = columns[elem_index];
		size_t i = 0;
		while (row_index[i] <= elem_index)
			i++;
		return index_tuple(i - 1, j);
	}

	size_t get_element_idx(index_tuple elem_coords) const {
		size_t default_answer = capacity;  // if element is not exist
		for (size_t idx = row_index[elem_coords.id1]; idx < row_index[elem_coords.id1 + 1]; idx++)
			if (columns[idx] == elem_coords.id2)
				return idx;
		return default_answer;
	}

	size_t get_not_exist_element_idx(index_tuple elem_coords) const {
		size_t row_first = row_index[elem_coords.id1];
		size_t row_last = row_index[elem_coords.id1 + 1];
		if (row_first == row_last)
			return row_first;

		size_t elem_idx = row_first;
		for (; elem_idx < row_last; elem_idx++)
			if (columns[elem_idx] >= elem_coords.id2)
				break;
		return elem_idx;
	}

public:
	CSR3Mat() = default;

	CSR3Mat(size_t M, size_t N, size_t nz_size, size_t cap,
		double* vals, size_t* cols, size_t* rindex) : capacity(cap) {
		swap(values, vals);
		swap(columns, cols);
		swap(row_index, rindex);
		fill_base(M, N);
	}

	void read_from_file(string file_path) {
		ifstream instream(file_path);
		instream >> M >> N >> non_zero_size;

		capacity = non_zero_size;
		values = new double[capacity];
		columns = new size_t[capacity];
		row_index = new size_t[M + 1];
		row_index[0] = 0;
		row_index[M] = non_zero_size;

		size_t previose_row_idx = 0;
		size_t current_row_idx = 0;
		for (size_t elem_idx = 0; elem_idx < non_zero_size; elem_idx++) {
			instream >> current_row_idx >> columns[elem_idx] >> values[elem_idx];
			for (; previose_row_idx < current_row_idx;)
				row_index[++previose_row_idx] = elem_idx;  // no buffer overrun here !!!
		}
		instream.close();
	}

	CSR3Mat(const CSR3Mat& other) {
		fill_base(other.M, other.N);
		non_zero_size = other.non_zero_size;
		capacity = other.capacity;

		values = new double[capacity];
		columns = new size_t[capacity];
		row_index = new size_t[M + 1];
		memcpy(values, other.values, sizeof(double) * non_zero_size);
		memcpy(columns, other.columns, sizeof(size_t) * non_zero_size);
		memcpy(row_index, other.row_index, sizeof(size_t) * (M + 1));
	}

	CSR3Mat(CSR3Mat&& other) noexcept {
		fill_base(other.M, other.N);
		non_zero_size = other.non_zero_size;
		capacity = other.capacity;
		swap(values, other.values);
		swap(columns, other.columns);
		swap(row_index, other.row_index);
	}

	CSR3Mat& operator=(const CSR3Mat& other) {
		fill_base(other.M, other.N);
		non_zero_size = other.non_zero_size;
		capacity = other.capacity;
		delete[] values;
		delete[] columns;
		delete[] row_index;

		values = new double[capacity];
		columns = new size_t[capacity];
		row_index = new size_t[M + 1];
		memcpy(values, other.values, sizeof(double) * non_zero_size);
		memcpy(columns, other.columns, sizeof(size_t) * non_zero_size);
		memcpy(row_index, other.row_index, sizeof(size_t) * (M + 1));
		return *this;
	}

	CSR3Mat& operator=(CSR3Mat&& other) noexcept {
		fill_base(other.M, other.N);
		non_zero_size = other.non_zero_size;
		capacity = other.capacity;
		swap(values, other.values);
		swap(columns, other.columns);
		swap(row_index, other.row_index);
		return *this;
	}

	void print_info() const {
		cout << "rows number: " << M << endl;
		cout << "columns number: " << N << endl;
		cout << "number of non-zero elements: " << non_zero_size << endl;
		cout << "capacity: " << capacity << endl << endl;

		char sep = ' ';
		cout << "values: " << endl;
		for (size_t i = 0; i < non_zero_size; i++) {
			cout << values[i] << sep;
		}
		cout << endl;
		cout << "columns: " << endl;
		for (size_t i = 0; i < non_zero_size; i++) {
			cout << columns[i] << sep;
		}
		cout << endl;
		cout << "row_index: " << endl;
		for (size_t i = 0; i < M + 1; i++) {
			cout << row_index[i] << sep;
		}
		cout << endl;
	}

	void write_to_file(string file_path) {
		ofstream outstream(file_path);
		char separator = ' ';
		outstream << M << separator << N << separator << non_zero_size << endl;

		for (size_t idx = 0; idx < non_zero_size; idx++) {
			index_tuple curr_position = get_element_position(idx);
			outstream << curr_position.id1 << separator << curr_position.id2 << separator;
			outstream << values[idx] << endl;
		}
		outstream.close();
	}

	FilledMat to_filled() {
		size_t filled_size = M * N;
		double* new_data = new double[filled_size] {0.0};

		for (size_t idx = 0; idx < non_zero_size; idx++) {
			index_tuple curr_position = get_element_position(idx);
			new_data[curr_position.id1 * N + curr_position.id2] = values[idx];
		}
		FilledMat full_matrix(M, N, new_data);
		return full_matrix;
	}

	CSR3Mat slice(index_tuple rows_ids, index_tuple columns_ids) const {
		size_t i1 = rows_ids.id1;
		size_t i2 = rows_ids.id2;
		size_t j1 = columns_ids.id1;
		size_t j2 = columns_ids.id2;

		size_t nz_size_counter = 0;
		for (size_t idx = row_index[i1]; idx < row_index[i2]; idx++)
			if (j1 <= columns[idx] && columns[idx] < j2)
				nz_size_counter++;
		
		size_t my_M = i2 - i1;
		size_t my_N = j2 - j1;
		size_t my_capacity = nz_size_counter;
		double* my_vals = new double[my_capacity];
		size_t* my_cols = new size_t[my_capacity];
		size_t* my_rindex = new size_t[my_M + 1];
		my_rindex[0] = 0;
		my_rindex[my_M] = nz_size_counter;
		size_t my_idx = 0;
		for (size_t i = i1; i < i2; i++) {
			for (size_t idx = row_index[i]; idx < row_index[i + 1]; idx++)
				if (j1 <= columns[idx] && columns[idx] < j2) {
					my_vals[my_idx] = values[idx];
					my_cols[my_idx] = columns[idx] - j1;
					my_idx++;
				}
			my_rindex[i - i1 + 1] = my_idx;
		}
		CSR3Mat my_csr3mat = CSR3Mat(my_M, my_N, nz_size_counter, my_capacity, 
			my_vals, my_cols, my_rindex);
		return my_csr3mat;
	}

	void insert(index_tuple elem_coords, double value) {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Insert error: Coordinates out of matrix" };
		size_t elem_idx = get_element_idx(elem_coords);

		if (elem_idx < capacity && value == 0.0)
			pop(elem_coords);
		if (elem_idx < capacity && value != 0.0)
			values[elem_idx] = value;
		if (elem_idx >= capacity && value != 0.0) {
			size_t new_elem_idx = get_not_exist_element_idx(elem_coords);
			for (size_t i = elem_coords.id1 + 1; i < M + 1; i++)
				row_index[i]++;

			size_t after_num = non_zero_size - new_elem_idx;
			if (capacity >= (non_zero_size + 1)) {
				memcpy(values + new_elem_idx + 1, values + new_elem_idx, sizeof(double) * after_num);
				values[new_elem_idx] = value;
				memcpy(columns + new_elem_idx + 1, columns + new_elem_idx, sizeof(size_t) * after_num);
				columns[new_elem_idx] = elem_coords.id2;
			}
			else {
				capacity = min(capacity + 10, N * M);

				double* new_values = new double[capacity];
				memcpy(new_values, values, sizeof(double) * new_elem_idx);
				memcpy(new_values + (new_elem_idx + 1), values + new_elem_idx, sizeof(double) * after_num);
				swap(new_values, values);
				delete[] new_values;
				values[new_elem_idx] = value;

				size_t* new_columns = new size_t[capacity];
				memcpy(new_columns, columns, sizeof(size_t) * new_elem_idx);
				memcpy(new_columns + (new_elem_idx + 1), columns + new_elem_idx, sizeof(size_t) * after_num);
				swap(new_columns, columns);
				delete[] new_columns;
				columns[new_elem_idx] = elem_coords.id2;
			}
			non_zero_size++;
		}
	}

	double pop(index_tuple elem_coords) {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Pop error: Coordinates out of matrix" };
		size_t elem_idx = get_element_idx(elem_coords);

		if (elem_idx >= capacity)
			return 0;
		double poped_val = values[elem_idx];

		for (size_t i = elem_coords.id1 + 1; i < M + 1; i++)
			row_index[i]--;

		size_t after_num = non_zero_size - 1 - elem_idx;
		if (capacity < 2 * (non_zero_size - 1)) {
			memcpy(values + elem_idx, values + elem_idx + 1, sizeof(double) * after_num);
			memcpy(columns + elem_idx, columns + elem_idx + 1, sizeof(size_t) * after_num);
		}
		else {
			capacity = non_zero_size - 1;
			double* new_values = new double[capacity];
			memcpy(new_values, values, sizeof(double) * elem_idx);
			memcpy(new_values + elem_idx, values + elem_idx + 1, sizeof(double) * after_num);
			swap(new_values, values);
			delete[] new_values;

			size_t* new_columns = new size_t[capacity];
			memcpy(new_columns, columns, sizeof(size_t) * elem_idx);
			memcpy(new_columns + elem_idx, columns + elem_idx + 1, sizeof(size_t) * after_num);
			swap(new_columns, columns);
			delete[] new_columns;
		}
		non_zero_size--;
		return poped_val;
	}

	double get_elem(index_tuple elem_coords) const {
		if (elem_coords.id1 >= M || elem_coords.id2 >= N)
			throw string{ "Get error: Coordinates out of matrix" };
		size_t elem_idx = get_element_idx(elem_coords);
		if (elem_idx >= capacity)
			throw string{ "Trying to get element by invalid coordinates" };
		return values[elem_idx];
	}

	CSR3Mat operator*(double value) const {
		CSR3Mat new_mat(*this);
		for (size_t i = 0; i < new_mat.non_zero_size; i++)
			new_mat.values[i] *= value;
		return new_mat;
	}

	CSR3Mat operator-() const {
		CSR3Mat new_mat(*this);
		for (size_t i = 0; i < new_mat.non_zero_size; i++)
			new_mat.values[i] *= -1.0;
		return new_mat;
	}

	bool has_contain(index_tuple elem_coords) const {
		return (capacity > get_element_idx(elem_coords));
	}

	CSR3Mat operator+(const CSR3Mat& other) const {
		if (this->M != other.M || this->N != other.N)
			throw string{ "Summation error: matrix with different shapes" };
		CSR3Mat new_mat(*this);
		for (size_t idx_in_this = 0; idx_in_this < this->non_zero_size; idx_in_this++){
			index_tuple coords = this->get_element_position(idx_in_this);
			if (other.has_contain(coords))
				new_mat.insert(coords, other.get_elem(coords) + this->values[idx_in_this]);
		}
		for (size_t idx_in_other = 0; idx_in_other < other.non_zero_size; idx_in_other++) {
			index_tuple coords = other.get_element_position(idx_in_other);
			if (!this->has_contain(coords))
				new_mat.insert(coords, other.values[idx_in_other]);
		}
		return new_mat;
	}

	CSR3Mat operator-(const CSR3Mat& other) const {
		return this->operator+(-other);
	}

	vector<double> operator*(vector<double> dense_vector) const {
		if (dense_vector.size() != N)
			throw string{ "Multiplication error: Matrix and vector has disbalanced sizes" };

		vector<double> res_vector(M);
		for (size_t row = 0; row < M; row++)
			for (size_t idx = row_index[row]; idx < row_index[row + 1]; idx++)
				res_vector[row] += values[idx] * dense_vector[columns[idx]];
		return res_vector;
	}

	index_tuple get_shape() {
		return index_tuple(M, N);
	}

	~CSR3Mat() {
		delete[] values;
		delete[] columns;
		delete[] row_index;
	}
};

static CSR3Mat operator*(const double value, const CSR3Mat& matrix) {
	return matrix * value;
}
