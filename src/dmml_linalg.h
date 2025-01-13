#pragma once

#include <vector>
#include <cmath>
#include <cstdarg>
#include <utility>
#include <algorithm>
#include <iostream>


#include <cassert>

//adding exception class? would need for comparing vector lengths

template <typename From, typename To>
struct static_caster {
	To operator()(From i) { return static_cast<To>(i); }
};




namespace dmml {
	namespace linalg {

		template <typename T>
		class Vector {
		public:

			//CONSTRUCTORS
			template<typename ...Ts>
			Vector(Ts...args)
				:sizeType_m(sizeof...(Ts)), vec_m{args...} {}
			

			Vector(std::vector<T> list) //test this
				:sizeType_m(list.size()), vec_m(list) {} 
			//maybe add pass by reference? etc.			




			
			//GETTERS
			typename std::vector<T>::size_type GetVectorSize() const { return this->sizeType_m; }


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
			template<typename T>
			void PushBack(T num) {
				this->vec_m.push_back(num);
			}

			
			void UpdateVectorSize() { this->_UpdateVectorSize(); }





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
				auto temp = dmml::linalg::Vector<decltype(*iOne + *iTwo)>(); //could store first value instead of recomputing

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					temp.vec_m.push_back(*iOne + *iTwo);
					iTwo++;
				}
				temp.UpdateVectorSize();
				return temp;
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
				auto temp = dmml::linalg::Vector<decltype(*iOne + *iTwo)>(); //could store first value instead of recomputing, checking indexes tho

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					temp.vec_m.push_back(*iOne - *iTwo);
					iTwo++;
				}
				temp.UpdateVectorSize();
				return temp;
			}


			double DotProduct(const dmml::linalg::Vector<T>& v2) const { //may need to make double
				try {
					if (this->_IsSameSize(v2)) {
						throw(this->GetVectorSize());
					}
					//may need to include exception for type checks
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetVectorSize() << " and passed vector of length " << v2.GetVectorSize() << '.' << std::endl;
					
					return INT_MIN;
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				typename std::vector<T>::const_iterator iTwo = v2.vec_m.cbegin();
				double dProd = 0.0; //check this for types
				
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
				auto temp = dmml::linalg::Vector<decltype(this->vec_m[0] * v2.vec_m[0])>();

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					temp.vec_m.push_back(*iOne * *iTwo);
					iTwo++;
				}
				return temp;
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


			dmml::linalg::Vector<double> ConvertUnitVector() const {
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
				dmml::linalg::Vector<double> temp = dmml::linalg::Vector<double>();
				for (auto iter = this->vec_m.cbegin(); iter != this->vec_m.cend(); iter++) {
					temp.PushBack(static_cast<double>(*iter) / vecNorm);
				}
				temp.UpdateVectorSize();
				return temp;
			}

			
			/*//take in called object, convert to double object and vector, return that, how to deal with destructor?
			void SelfConvertUnitVector() {
				try {
					if (this->GetVectorSize() == 0) {
						throw(this->GetVectorSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return;
				}

				double vecNorm = this->L2Norm();
				
				//std::vector<double> tempAssign;
				//std::transform(this->vec_m.begin(), this->vec_m.end(), std::back_inserter(tempAssign), static_caster<decltype(this->vec_m), std::vector<double>>());
				//this = dmml::linalg::Vector<double>(tempAssign);
				return;
			}*/





			//add operators





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
				for (typename std::vector<T>::size_type r = 0; r < this->rowSize_m; r++) {
					for (typename std::vector<T>::size_type c = 0; c < this->colSize_m; c++) {
						std::cout << matrix_m[r][c];
					}
					std::cout << '\n';
				}
				std::cout << std::endl;
			}





			//SETTERS





			//MEMBERS
			dmml::linalg::Matrix<T> MatMul(const Matrix& m2) {

				try {
					if (this->colSize_m != m2->rowSize_m) {

					}
				}
				catch (/*const std::exception&*/) {

				}

				auto temp = dmml::linalg::Matrix<T>(this->colSize_m, m2.rowSize_m);
				typename std::vector<std::vector<T>> tempRIter = temp.begin();
				typename std::vector<T> tempCIter = tempRIter->begin();

				for (typename std::vector<std::vector<T>>::iterator m1RIter = this->matrix_m.begin(); m1RIter != this->matrix_m.end(); m1RIter++) {
					typename std::vector<std::vector<T>>::iterator m1CIter = m1RIter->begin();

					for (typename std::vector<std::vector<T>> m2RIter = m2.matrix_m.begin(); m2RIter != m2.matrix_m.end()) {
							
					}
				}
				return temp;
			}





		private:

			//MEMBER VALUES
			typename std::vector<T>::size_type rowSize_m {};
			typename std::vector<T>::size_type colSize_m {};
			std::vector<std::vector<T>> matrix_m {};


		};
	}
}
