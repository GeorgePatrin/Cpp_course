#include "CSR3Mat.h"


void indexation_test(string file_path) {
	string input_name = "\\input_CSR3.txt";

	CSR3Mat my_mat;
	my_mat.read_from_file(file_path + input_name);
	//my_mat.print_info();

	FilledMat my_fmat = move(my_mat.to_filled());
	my_fmat.print_info();
	cout << endl << endl;

	try {
		my_mat.pop(index_tuple(1, 0));  // correct pop
		my_mat.insert(index_tuple(4, 4), 200);  // simple insert
		my_mat.insert(index_tuple(1, 3), -200);  // insert with memory allocation
		double a = my_mat.get_elem(index_tuple(0, 0));  // correct getting
		cout << "Getting value by (0, 0): \t" << a << endl;
		my_mat.pop(index_tuple(3, 4));  // correct pop
		my_mat.pop(index_tuple(0, 0));  // correct pop
		my_mat.pop(index_tuple(1, 1));  // correct pop
		my_mat.pop(index_tuple(3, 0));  // pop with memory allocation
		my_mat.pop(index_tuple(5, 5));  // incorrect pop
	}
	catch (const string error_message) {
		cout << endl << error_message << endl;
	}
	//my_mat.print_info();

	FilledMat new_my_fmat = move(my_mat.to_filled());
	new_my_fmat.print_info();

	string output_name = "\\output_CSR3.txt";
	my_mat.write_to_file(file_path + output_name);
}

void operation_test(string file_path) {
	string input1_name = "\\input1_CSR3.txt";
	string input2_name = "\\input2_CSR3.txt";

	CSR3Mat my_mat1;
	my_mat1.read_from_file(file_path + input1_name);
	//my_mat1.print_info();

	CSR3Mat my_mat2;
	my_mat2.read_from_file(file_path + input2_name);
	//my_mat2.print_info();

	FilledMat my_fmat1 = move(my_mat1.to_filled());
	my_fmat1.print_info();

	FilledMat my_fmat2 = move(my_mat2.to_filled());
	my_fmat2.print_info();

	try {
		CSR3Mat my_mat3 = my_mat1 - 2 * my_mat2;
		//my_mat3.print_info();
		cout << endl << endl;
		cout << "Mat1 - 2 * Mat2:" << endl;

		FilledMat my_fmat3 = move(my_mat3.to_filled());
		my_fmat3.print_info();
	}
	catch (const string error_message) {
		cout << endl << error_message << endl;
	}
}

template<typename T>
void print_vector(vector<T> my_vector) {
	cout << "Vector:" << endl;
	for (size_t i = 0; i < my_vector.size(); i++)
		cout << my_vector[i] << '\t';
	cout << endl << endl;
}

void vector_multiplication_test(string file_path) {
	string input_name = "\\input_CSR3.txt";

	CSR3Mat my_mat;
	my_mat.read_from_file(file_path + input_name);
	//my_mat.print_info();

	index_tuple mat_shape = my_mat.get_shape();
	vector<double> my_vec(mat_shape.id2);
	my_vec[0] = 1;
	my_vec[2] = 1;
	my_vec[3] = 1;
	print_vector(my_vec);

	FilledMat my_fmat = move(my_mat.to_filled());
	my_fmat.print_info();

	try {
		vector<double> new_vect = my_mat * my_vec;
		print_vector(new_vect);
	}
	catch (const string error_message) {
		cout << endl << error_message << endl;
	}
}

int main() {
	string file_path = "C:\\Users\\Georgiy\\Desktop\\C++ course\\lectures\\лекция 5";

	//indexation_test(file_path);
	
	//operation_test(file_path);

	//vector_multiplication_test(file_path);


	return 0;
}
