//#pragma once

#include <vector>
#include <mutex>
#include <cmath>
#include <cstdarg>
#include <utility>
#include <algorithm>
#include <iostream>
#include <climits>
#include <exception>

#include <cassert>

//adding exception class?


//callable type caster
template <typename From, typename To>
struct static_caster {
	To operator()(From i) { return static_cast<To>(i); }
};


//vector type check
template<typename T, template<typename> typename V>
bool is_Vector = std::false_type{};

template<template<typename> typename V, typename U>
bool is_Vector<V<U>, V> = std::true_type{};


//matrix type check
template<typename T, template<typename> typename M>
bool is_Matrix = std::false_type{};

template<template<typename> typename M, typename U>
bool is_Matrix<M<U>, M> = std::true_type{};


//--------------------MAKE GETTER MEMBER FUNCTIONS TO MAKE TEMPLATED CLASSES WORK ACROSS TYPES-----------------------
//change/check decltypes of derived vectors/matrices
//constructor for vector/matrix for single value to fill whole entity
//std::common type for deriving types of vector/matrix operations
//change vector <T>PushBack to private only?
//update try/catch type and size checks to some sort of function





//std::vector static allocator
//template<typename T, const size_t MAX_SIZE>
//class VectorStackCalloc {
//
//public:
//	typedef T				  value_type;
//	typedef value_type*       pointer;
//	typedef const value_type* const_pointer;
//	typedef value_type&		  reference;
//	typedef const value_type& const_reference;
//	typedef std::size_t		  size_type;
//	typedef std::ptrdiff_t    difference_type;
//
//
//	/*enum {
//		NUMBER_OF_BUFFERS
//	};*/
//
//
//	VectorCalloc() 
//		: buffer_id(0) {}
//
//
//	template<typename U>
//	struct rebind {
//		typedef VectorCalloc<U, MAX_SIZE> other;
//	};
//
//
//	
//
//
//private:
//
//	int buffer_id;
//
//};










namespace dmml {
	namespace linalg {		

		template <typename T>
		class Vector {
		public:


			//CONSTRUCTORS
			Vector()
				:sizeType_m(0), vec_m{} {
				//std::cout << "Constructor 1" << std::endl;
			}
	

			template<typename ...Ts>
			Vector(Ts...args)
				: sizeType_m(sizeof...(Ts)), vec_m{ args... } {
				//std::cout << "Constructor 2" << std::endl;
			}


			template<typename T>
			Vector(std::vector<T> list) //test this
				: sizeType_m(list.size()), vec_m(list) {
				//std::cout << "Constructor 3" << std::endl;
			}
			//maybe add pass by reference? etc.			




			
			//GETTERS
			typename std::vector<T>::size_type GetSize() const { return this->sizeType_m; }
			
			std::vector<T> GetValues() const { return this->vec_m; }


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
			template<typename T2>
			auto AddVector(const dmml::linalg::Vector<T2>* const v2) const {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (this->sizeType_m != v2->GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (typename std::vector<dervType>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetSize() << " and passed vector of length " << v2->GetSize() << '.' << std::endl;
					
					return dmml::linalg::Vector<dervType>();
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				auto v2Vec = v2->GetValues();
				auto iTwo = v2Vec.cbegin();	
				auto retVec = dmml::linalg::Vector<dervType>();

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.PushBack(*iOne + *iTwo);
					iTwo++;
				}
				retVec.UpdateVectorSize();
				return retVec;
			}

			
			template <typename T2>
			auto SubtractVector(const dmml::linalg::Vector<T2>* const v2) const {  

				using dervType = typename std::common_type<T, T2>::type;

				try { //check for 0 length vectors?
					if (!is_Vector<std::decay_t<decltype(*v2)>, dmml::linalg::Vector>) { //almost uneccessary?
						throw(typeid(v2).name());
					}

					if (this->sizeType_m != v2->GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Type Error" << std::endl;
					return dmml::linalg::Vector<dervType>((dervType)1);
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetSize() << " and passed vector of length " << v2->GetSize() << '.' << std::endl;
					
					return dmml::linalg::Vector<dervType>((dervType)1);
				}
	
				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				auto v2Vec = v2->GetValues();
				auto iTwo = v2Vec.cbegin();
				auto retVec = dmml::linalg::Vector<dervType>(); //could store first value instead of recomputing, checking indexes tho

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.PushBack(*iOne - *iTwo);
					iTwo++;
				}
				retVec.UpdateVectorSize();
				return retVec;
			}


			template <typename V2> 
			double DotProduct(const V2* const v2) const { 	
								
				try {	
					if (!is_Vector<std::decay_t<decltype(*v2)>, dmml::linalg::Vector>) { //almost uneccessary?
						throw(typeid(v2).name());
					}
					
					if (this->sizeType_m != v2->GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Type Error" << std::endl;
					return std::numeric_limits<double>::min();
				}
				catch (const typename std::vector<T>::size_type&) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetSize() << " and passed vector of length " << v2->GetSize() << '.' << std::endl;
					
					return std::numeric_limits<double>::min();
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				auto v2Vals = v2->GetValues();
				auto iTwo = v2Vals.cbegin();

				double dProd = 0.0;
				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					dProd += *iOne * *iTwo;
					iTwo++;
				}
				return dProd;
			}


			double L2Norm() const {
				try {
					if (this->GetSize() == 0) {
						throw(this->GetSize());
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


			template<typename T2>
			auto HadamardProduct(const dmml::linalg::Vector<T2>*const v2) const {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (!is_Vector<std::decay_t<decltype(*v2)>, dmml::linalg::Vector>) { //almost uneccessary?
						throw(typeid(v2).name());
					}

					if (this->sizeType_m != v2->GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Type Error" << std::endl;
					return dmml::linalg::Vector<dervType>((dervType)1);
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors of differing sizes" << std::endl;
					return dmml::linalg::Vector<dervType>((dervType)1);
				}

				typename std::vector<T>::const_iterator iOne = this->vec_m.cbegin();
				auto v2Vals = v2->GetValues();
				auto iTwo = v2Vals.cbegin();
				auto retVec = dmml::linalg::Vector<dervType>();	

				for (iOne; iOne != this->vec_m.cend(); iOne++) {
					retVec.PushBack(*iOne * *iTwo);
					iTwo++;
				}
				retVec.UpdateVectorSize();
				return retVec;
			}


			dmml::linalg::Vector<double> ConvertUnitVector() const  {
				try {
					if (this->GetSize() == 0) {
						throw(this->GetSize());
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return dmml::linalg::Vector<double>();
				}

				double vecNorm = this->L2Norm();
				auto retVec = dmml::linalg::Vector<double>();
				
				for (auto iter = this->vec_m.cbegin(); iter != this->vec_m.cend(); iter++) {
					retVec.PushBack(static_cast<double>(*iter) / vecNorm);
				}
				retVec.UpdateVectorSize();
				return retVec;
			}


			dmml::linalg::Vector<double> ConvertUnitVector(dmml::linalg::Vector<double>& unit) const { //change overload/const
				try {
					if (this->GetSize() == 0) {
						throw(this->GetSize());
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
			using valType_m = typename std::vector<T>::value_type;
			typename std::vector<T>::size_type sizeType_m { 0 };
			std::vector<T> vec_m {};


			//SETTERS
			void _UpdateVectorSize() { this->sizeType_m = this->vec_m.size(); }


			//COMPARISONS
			/*bool _IsSameSize(const dmml::linalg::Vector<T>& v2) const {
				if (this->sizeType_m == v2.GetSize()) {
					return true;
				}
				else {
					return false;
				}
			}*/	


			//OPERATORS

		};










		template <typename T>
		class Matrix {
		public:

			//CONSTRUCTORS - need overloads for lvalue/rvalue args and vector list constructor
			//in constructors check if T matches Ts
			Matrix() {}

			
			Matrix(uint64_t rows, uint64_t cols)
				:rowSize_m(static_cast<typename std::vector<T>::size_type>(rows)), colSize_m(static_cast<typename std::vector<T>::size_type>(cols)),
				  matrix_m(std::vector<std::vector<T>>(rows, std::vector<T>(cols))) {}
			

			template <typename...Ts>
			Matrix(uint64_t rows, uint64_t cols, Ts...args) 
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
			 typename std::vector<std::vector<T>> GetValues() const { return this->matrix_m; }

			 typename std::vector<std::vector<T>>::size_type GetRowSize() const { return this->rowSize_m; }

			 typename std::vector<T>::size_type GetColSize() const { return this->colSize_m; }


			void ShowMatrix() const { //CONVERT TO ITERATORS?
				//std::cout << '[';
				for (typename std::vector<T>::size_type r = 0; r < this->rowSize_m; r++) {
					for (typename std::vector<T>::size_type c = 0; c < this->colSize_m; c++) {
						std::cout << ' ' << matrix_m[r][c] << ' ';
					}
					std::cout << '\n';
				}

				std::cout << std::endl;
			}





			//SETTERS
			void AssignElement(uint64_t row, uint64_t col, T val) {
				this->matrix_m[row][col] = val;
			}





			//MEMBERS
			void Transpose() {
				
				auto retMat = std::vector<std::vector<T>>(this->colSize_m, std::vector<T>(this->rowSize_m));

				for (uint64_t oldRow = 0; oldRow < this->rowSize_m; oldRow++) {
					auto tempRow = this->matrix_m[oldRow];
					
					for (uint64_t newCol = 0; newCol < this->rowSize_m; newCol++) {
						retMat[newCol][oldRow] = tempRow[newCol];
					}
				}
				this->matrix_m = retMat;
				_UpdateMatrixSize();
			}


			template<typename T2>
			auto MatMul(const dmml::linalg::Matrix<T2>* const m2) const {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (!is_Matrix<std::decay_t<decltype(*m2)>, dmml::linalg::Matrix>) {
						throw(typeid(*m2).name());
					}

					if (this->colSize_m != m2->GetRowSize()) {
						throw(this->colSize_m); //change to get col size method
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Improper type passed." << std::endl;
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Matrices of differing sizes." << std::endl;
					return dmml::linalg::Matrix<dervType>(1, 1, (dervType)1);
				}
				 
				auto retMat = dmml::linalg::Matrix<dervType>(this->colSize_m, m2->GetRowSize());
				auto m2Matrix = m2->GetValues();
				dervType temp = 0;

				for (uint64_t m1R = 0; m1R < this->rowSize_m; m1R++) {
					for (uint64_t m2C = 0; m2C < m2->GetColSize(); m2C++) {
						uint64_t m2R = 0;
						temp = 0;
						for (uint64_t m1C = 0; m1C < this->colSize_m; m1C++) {
							temp += this->matrix_m[m1R][m1C] * m2Matrix[m2R][m2C];
							m2R++;
						}
						m2R = 0;
						retMat.AssignElement(m1R, m2C, temp);
					}
				}
				return retMat;
			}
			

			void MatMul_(const int64_t&& scalar) {


			}


			dmml::linalg::Matrix<T> IdentityMatrix(const uint64_t&& size) const { //change type on identity matrix to be more flexible? template?

				auto idMatrix = dmml::linalg::Matrix<T>(size, size);
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


			void EigenDecomposition(const dmml::linalg::Matrix<T>* const matrix) const {

			}

			
			void SingleValueDecomposition(const dmml::linalg::Matrix<T>* const matrix) const {
				
				//U - left singular vectors
				//V - right singular vectors
				//D - singular values
				
				//auto U =  

			}


			dmml::linalg::Matrix<T> MoorePenrosePseudoInverse() const {

				SingleValueDecomposition(this);
				//single value decomposition
				//eigen decomp
				//inverse and transpose D+
				//multiply for pseudoinv


			}


			auto Determinant() {
			
				if (this->rowSize_m == this->colSize_m) {
					_SquareDeterminant(this);
				}
				else {
					_EigenDeterminant(this);
				}
			
			}





			//OPERATORS
			//dmml::linalg::Matrix<T> operator= (const Matrix<T>& m) 

				
		private:

			//MEMBER VALUES
			typename std::vector<std::vector<T>>::size_type rowSize_m {};
			typename std::vector<T>::size_type colSize_m {};
			std::vector<std::vector<T>> matrix_m {};

			//need destructor, copy constructor, and copy assignment operator


			//SETTERS
			void _UpdateMatrixSize() {
				this->colSize_m = this->matrix_m.size();
				this->rowSize_m = this->matrix_m[0].size();
			}





			//PRIVATE MEMBER FUNCS

		};
	}
}
