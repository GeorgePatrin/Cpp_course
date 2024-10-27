#include <stdio.h>
#include <format>
#include <fstream>
#include <iostream>

using namespace std;

struct index_tuple {
public:
	const size_t id1;  // row index
	const size_t id2;  // column index

	index_tuple(size_t i1, size_t i2) : id1(i1), id2(i2) {}
};

class Matrix {
protected:
	size_t M = 0;  // rows number
	size_t N = 0;  // columns number

	void fill_base(size_t M, size_t N) {
		this->M = M;
		this->N = N;
	}

public:
	virtual void read_from_file(string file_path) = 0;
	virtual void print_info() const = 0;

	virtual void insert(index_tuple elem_ids, double value) = 0;
	virtual double pop(index_tuple elem_ids) = 0;
	virtual double get_elem(index_tuple elem_ids) const = 0;

	//virtual Matrix& operator=(const Matrix& other) = 0;
	//virtual Matrix& operator=(Matrix&& other) = 0;
};
