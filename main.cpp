# include <stdio.h>
# include <iostream>
#include <fstream>

class DynamicArray3D {
private:
	int* ptr_level3;  // указатель третьего уровня глубины (указывает на массив значений типа double)
	int** ptr_level2;  // указатель второго уровня глубины на массив указателей double*
	int*** ptr_level1;  // указатель первого уровня глубины на массив указателей double**
	size_t size_1, size_2, size_3;  // размеры трёхмерного массива
public:
	// функция возвращает указатель, по которому можно итерироваться
	int*** get_ptr() {
		return ptr_level1;
	}

	// функция по ссылкам передает разеры массива
	void get_shape(size_t& size_1_new, size_t& size_2_new, size_t& size_3_new) {
		size_1_new, size_2_new, size_3_new = size_1, size_2, size_3;
	}

	// функция заполняет массив нулями
	void make_zeros() {
		size_t full_size = size_1 * size_2 * size_3;
		for (size_t i = 0; i < full_size; i++){
			ptr_level3[i] = 0;
		}
	}

	DynamicArray3D(size_t size_1_new, size_t size_2_new, size_t size_3_new) {
		size_1 = size_1_new;
		size_2 = size_2_new;
		size_3 = size_3_new;

		ptr_level3 = new int[size_1 * size_2 * size_3];
		ptr_level2 = new int*[size_1 * size_2];
		ptr_level1 = new int**[size_1];

		for (size_t i = 0; i < size_1 * size_2; i++){
			ptr_level2[i] = &ptr_level3[i * size_3];
		}
		for (size_t i = 0; i < size_1; i++){
			ptr_level1[i] = &ptr_level2[i * size_2];
		}
	}

	~DynamicArray3D() {
		delete[] ptr_level3;
		delete[] ptr_level2;
		delete[] ptr_level1;
	}
};

// функция выводит значения трёхмерного массива в файл по указанному пути, указателю и размерам
// вынесена из класса для наглдной демонстрации работы обращения по мультииндексу
void print_array_3D(size_t size_1, size_t size_2, size_t size_3, 
	int*** array_ptr, std::string full_path) {
	std::ofstream outstream(full_path);
	std::string separator = " ";
	for (size_t i = 0; i < size_1; i++) {
		for (size_t j = 0; j < size_2; j++) {
			for (size_t k = 0; k < size_3; k++) {
				outstream << array_ptr[i][j][k] << separator;
			}
			outstream << std::endl;
		}
		outstream << std::endl << std::endl;
	}
	outstream.close();
}

void main() {
	std::string file_path = "C:\\Users\\Georgiy\\Desktop\\C++ course\\lectures\\лекция 2\\домашка_2";
	std::string input_name = "\\input.txt";
	std::string output_name = "\\output.txt";
	size_t size_1, size_2, size_3;

	std::ifstream instream(file_path + input_name);
	instream >> size_1;
	instream >> size_2;
	instream >> size_3;
	instream.close();

	DynamicArray3D arr3d = DynamicArray3D(size_1, size_2, size_3);
	arr3d.make_zeros();
	int*** array_ptr = arr3d.get_ptr();

	print_array_3D(size_1, size_2, size_3, array_ptr, file_path + output_name);
}
