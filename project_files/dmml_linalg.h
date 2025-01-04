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

			
			void UpdateVectorSize() { this->SetVectorSize_(); }





			//member functions
			dmml::linalg::Vector<T> AddVector(const dmml::linalg::Vector<T>& v2) const {
				try {
					if (this->IsSameSize_(v2)) {
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
				temp.SetVectorSize_();
				return temp;
			}


			dmml::linalg::Vector<T> SubtractVector(const dmml::linalg::Vector<T>& v2) const {  
				try { //check for 0 length vectors?
					if (this->IsSameSize_(v2)) {
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
				temp.SetVectorSize_();
				return temp;
			}


			double DotProduct(const dmml::linalg::Vector<T>& v2) const { //may need to make double
				try {
					if (this->IsSameSize_(v2)) {
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
					if (this->IsSameSize_(v2)) {
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
			void SetVectorSize_() { this->sizeType_m = this->vec_m.size(); }


			//COMPARISONS
			bool IsSameSize_(const dmml::linalg::Vector<T>& v2) const {
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

			//CONSTRUCTORS
			template<typename...Ts>
			Matrix(Ts ...args)
				//	:rowSize_m(rows), colSize_m(cols) {
			{
				unsigned int rows = 2;
				unsigned int cols = 2;

				((std::cout << args << std::endl), ...);

				//matrix constructor, pack expansion with these to put them in correct
				//row/col order? do with iterators?

				//std::vector<std::common_type_t<Ts>> tempVec = { args... }; //expand/assign without copying?
				//matrix_m = std::vector<std::vector<Ts>>(rows);
				
				/*auto tempIter = temp.cbegin();
				for (unsigned int r = 0; r < rows; r++) {
					for (unsigned int c = 0; c < cols; c++) {
						matrix_m[r][c] = *tempIter;
						tempIter++;
					}
				}*/
				
			}
			

			/*Matix(unsigned int rows, unsigned int cols)
				:rowSize_m(rows), colSize_m(cols) {}*/



			void ShowMatrix() const {
				for (typename std::vector<T>::size_type r = 0; r < this->rowSize_m; r++) {
					for (typename std::vector<T>::size_type c = 0; c < this->colSize_m; c++) {
						std::cout << matrix_m[r][c];
					}
					std::cout << '\n';
				}
				std::cout << std::endl;
			}


		private:

			//MEMBER VALUES
			typename std::vector<T>::size_type rowSize_m {};
			typename std::vector<T>::size_type colSize_m {};
			std::vector<std::vector<T>> matrix_m {};


		};
	}
}
