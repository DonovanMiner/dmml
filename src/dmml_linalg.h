//#pragma once

#include <vector>
#include <cmath>
#include <cstdarg>
#include <utility>
#include <algorithm>
#include <iostream>
#include <climits>
#include <exception>

#include <cassert>

//adding exception class?


template <typename From, typename To>
struct static_caster {
	To operator()(From i) { return static_cast<To>(i); }
};


template<typename T, template<typename> typename U>
bool is_Vector = std::false_type{};

template<template<typename> typename U, typename V>
bool is_Vector<U<V>, U> = std::true_type{};


namespace dmml {
	namespace linalg {		

		template <typename T>
		class Vector {
		public:

			//CONSTRUCTORS
			Vector() 
				:sizeType_m(0), vec_m() {}

			template<typename ...Ts>
			Vector(Ts...args)
				:sizeType_m(sizeof...(Ts)), vec_m{args...} {}
			

			Vector(std::vector<T> list) //test this
				:sizeType_m(list.size()), vec_m(list) {} 
			//maybe add pass by reference? etc.			




			
			//GETTERS
			typename std::vector<T>::size_type GetVectorSize() const { return this->sizeType_m; }


			std::vector<T> GetVector() const { return this->vec_m; }


			void ShowVector() const {
				//display type as well?
				std::cout << "Size: " << sizeType_m << "\nValues: ";

				typename std::vector<T>::const_iterator iter = this->vec_m.cbegin();
				for (iter; iter != this->vec_m.cend(); iter++) {
					std::cout << *iter << ' ';
				}
				std::cout << std::endl;
			};





			//SETTERS
			void UpdateVectorSize() { this->_UpdateVectorSize(); }


			void PushBack(T val) { this->vec_m.push_back(val); }




			//member functions
			dmml::linalg::Vector<T> AddVector(const dmml::linalg::Vector<T>& v2) const {
				try {
					if (this->_IsSameSize(v2)) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetVectorSize() << " and passed vector of length " << v2.GetVectorSize() << '.' << std::endl;
					
					return dmml::linalg::Vector<T>();
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				typename std::vector<T>::const_iterator iTwo = v2.vec_m.cbegin();	
				auto retVec = dmml::linalg::Vector<decltype(*iOne + *iTwo)>(); //could store first value instead of recomputing

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.vec_m.push_back(*iOne + *iTwo);
					iTwo++;
				}
				retVec.UpdateVectorSize();
				return retVec;
			}


			dmml::linalg::Vector<T> SubtractVector(const dmml::linalg::Vector<T>& v2) const {  
				try { //check for 0 length vectors?
					if (this->_IsSameSize(v2)) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetVectorSize() << " and passed vector of length " << v2.GetVectorSize() << '.' << std::endl;
					
					return dmml::linalg::Vector<T>();
				}
	
				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				typename std::vector<T>::const_iterator iTwo = v2.vec_m.cbegin();
				auto retVec = dmml::linalg::Vector<decltype(*iOne + *iTwo)>(); //could store first value instead of recomputing, checking indexes tho

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.vec_m.push_back(*iOne - *iTwo);
					iTwo++;
				}
				retVec.UpdateVectorSize();
				return retVec;
			}

			template <typename T2> 
			double DotProduct(const T2& v2) const { 	
								
				try {	
					if (!is_Vector<std::decay_t<const decltype(v2)&>, dmml::linalg::Vector>) { //almost uneccessary?
						throw(typeid(v2).name());
					}
					
					if (this->_IsSameSize(v2)) {
						throw(this->GetVectorSize());
					}
				}
				catch (const std::bad_typeid& typeErr) {
					typeErr.what();
				}
				catch (const typename std::vector<T>::size_type&) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetVectorSize() << " and passed vector of length " << v2.GetVectorSize() << '.' << std::endl;
					
					return std::numeric_limits<double>::min();
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				auto iTwo = v2.vec_m.cbegin();
				double dProd = 0.0;
				
				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					dProd += *iOne * *iTwo;
					iTwo++;
				}
				return dProd;
			}


			double L2Norm() const {
				try {
					if (this->GetVectorSize() == 0) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of length 0" << std::endl;
					return 0;
				}

				double norm = 0.0;
				for (auto iter = this->vec_m.cbegin(); iter != this->vec_m.cend(); iter++) {
					norm += *iter * *iter;
				}
				return std::sqrt(norm);
			}


			dmml::linalg::Vector<T> HadamardProduct(const dmml::linalg::Vector<T>& v2) const {
				try {
					if (this->_IsSameSize(v2)) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors of differing sizes" << std::endl;
					return dmml::linalg::Vector<T>();
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				typename std::vector<T>::const_iterator iTwo = v2.vec_m.cbegin();
				auto retVec = dmml::linalg::Vector<decltype(this->vec_m[0] * v2.vec_m[0])>();

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.vec_m.push_back(*iOne * *iTwo);
					iTwo++;
				}
				return retVec;
			}


			dmml::linalg::Vector<double> ConvertUnitVector() const  {
				try {
					if (this->GetVectorSize() == 0) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return dmml::linalg::Vector<double>();
				}

				double vecNorm = this->L2Norm();
				dmml::linalg::Vector<double> retVec = dmml::linalg::Vector<double>();
				
				for (auto iter = this->vec_m.cbegin(); iter != this->vec_m.cend(); iter++) {
					retVec.PushBack(static_cast<double>(*iter) / vecNorm);
				}
				retVec.UpdateVectorSize();
				return retVec;
			}


			dmml::linalg::Vector<double> ConvertUnitVector(dmml::linalg::Vector<double>& unit) const {
				try {
					if (this->GetVectorSize() == 0) {
						throw(this->GetVectorSize());
					} 
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return dmml::linalg::Vector<double>();
				}

				double vecNorm = this->L2Norm();
				for (auto iter = this->vec_m.cbegin(); iter != this->vec_m.cend(); iter++) {
					unit.PushBack(static_cast<double>(*iter) / vecNorm);
				}
				unit.UpdateVectorSize();
				return unit;
			}
	
			
			/*dmml::linalg::Vector<double> ConvertUnitVector_() {

				return dmml::linalg::Vector<double>)();
			}*/




			//add operators
			//copy constructor




		private:
	
			//MEMBER VALUES
			typename std::vector<T>::size_type sizeType_m { 0 };
			std::vector<T> vec_m {};


			//SETTERS
			void _UpdateVectorSize() { this->sizeType_m = this->vec_m.size(); }


			//COMPARISONS
			bool _IsSameSize(const dmml::linalg::Vector<T>& v2) const {
				if (this->sizeType_m != v2.sizeType_m) {
					return true;
				}
				else {
					return false;
				}
			}	


			//OPERATORS

		};




		template <typename T>
		class Matrix {
		public:

			//CONSTRUCTORS - need overloads for lvalue/rvalue args and vector list constructor
			//in constructors check if T matches Ts
			Matrix() {}

			
			Matrix(unsigned int rows, unsigned int cols)
				:rowSize_m(static_cast<typename std::vector<T>::size_type>(rows)), colSize_m(static_cast<typename std::vector<T>::size_type>(cols)),
				  matrix_m(std::vector<std::vector<T>>(rows, std::vector<T>(cols))) {}
			

			template <typename ...Ts>
			Matrix(unsigned int rows, unsigned int cols, Ts...args) 
				:rowSize_m(static_cast<typename std::vector<T>::size_type>(rows)), colSize_m(static_cast<typename std::vector<T>::size_type>(cols)) {
				
				//check T vs Ts types
				std::vector<T> initList = { { args... } }; //pack expansion but without copying the data?
				/*for (auto iter : initList) {
					std::cout << iter << ' ';
				}*/

				//init matrix_m size
				matrix_m = std::vector<std::vector<T>>(rows, std::vector<T>(cols));

				typename std::vector<T>::const_iterator initIter = initList.cbegin();
				for (typename std::vector<std::vector<T>>::iterator rIter = matrix_m.begin(); rIter != matrix_m.end(); rIter++) {
					for (typename std::vector<T>::iterator cIter = rIter->begin(); cIter != rIter->end(); cIter++) {
						*cIter = *initIter++;
					}
				}

			}





			//GETTERS
			void ShowMatrix() const { //CONVERT TO ITERATORS?
				//std::cout << '[';
				for (typename std::vector<T>::size_type r = 0; r < this->rowSize_m; r++) {
					for (typename std::vector<T>::size_type c = 0; c < this->colSize_m; c++) {
						std::cout << matrix_m[r][c] << ' ';
					}
					std::cout << '\n';
				}

				std::cout << std::endl;
			}





			//SETTERS





			//MEMBERS
			//dmml::linalg::Matrix<T> MatMul(const Matrix& m2) {

			//	//try {
			//	//	if (this->colSize_m != m2->rowSize_m) {

			//	//	}
			//	//}
			//	//catch (/*const std::exception&*/) {

			//	//}

			//	auto retMat = dmml::linalg::Matrix<T>(this->colSize_m, m2.rowSize_m);
			//	unsigned int m1R = 0;
			//	unsigned int m1C = 0;
			//	unsigned int m2R = 0;
			//	unsigned int m2C = 0;
			//	decltype(this->matrix_m[m1R][m1C] * m2.matrix_m[m2R][m2C]) temp = 0;
			//	std::cout << "Sizes: " << this->rowSize_m << ' ' << this->colSize_m << ' ' << m2.rowSize_m << ' ' << m2.colSize_m << std::endl;
			//	for (m1R; m1R < this->rowSize_m; m1R++) {
			//		
			//		for (m1C; m1C < this->colSize_m; m1C++) {	 
			//			std::cout << "Row " << m1R << " Col: " << m1C << '\n';
			//			temp = 0;
			//			for (m2C; m2C < m2.colSize_m; m2C++) {

			//				for (m2R; m2C < m2.rowSize_m; m2C++) {
			//					std::cout << "m2Row " << m2R << " m2Col: " << m2C << '\n';
			//					temp += this->matrix_m[m1R][m1C] * m2.matrix_m[m2R][m2C];
			//				}
			//			}
			//		}
			//		retMat.matrix_m[m1R][m2C] = temp;
			//	}
			//	return retMat;
			//}


			dmml::linalg::Matrix<double> IdentityMatrix(const unsigned int&& size) {

				auto idMatrix = dmml::linalg::Matrix<double>(size, size);
				for (auto rIter = idMatrix.matrix_m.begin(); rIter != idMatrix.matrix_m.end(); rIter++) {
					for (auto cIter = rIter->begin(); cIter != rIter->end(); cIter++) {
						if (std::distance(idMatrix.matrix_m.begin(), rIter) == std::distance(rIter->begin(), cIter)) {
							*cIter = 1;
						}
						else {
							*cIter = 0;
						}
					}
				}
				return idMatrix;
			}




		private:

			//MEMBER VALUES
			typename std::vector<std::vector<T>>::size_type rowSize_m {};
			typename std::vector<T>::size_type colSize_m {};
			std::vector<std::vector<T>> matrix_m {};


		};
	}
}
