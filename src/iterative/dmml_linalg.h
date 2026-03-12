#pragma once

#include <vector>
#include <utility>
#include <type_traits>
#include <exception>
#include <algorithm>

#include <cstdarg>
#include <climits>
#include <cassert>
#include <cmath>

#include <iostream>
#include<iomanip>

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
//change/check decltypes of derived vectors/matrices (particularly for QR Decomp)
//copy constructor for vectors and matrices
// 
//constructor for vector/matrix for single value to fill whole entity
//std::common type for deriving types of vector/matrix operations
//update try/catch type and size checks to some sort of function (may be unnecessary)





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
			template <typename U> friend class Vector;
			template <typename V> friend class Matrix;

			//CONSTRUCTORS
			Vector()
				:sizeType_m(0), vec_m{} {
				//std::cout << "Constructor 1" << std::endl;
			}


			Vector(typename std::vector<T>::size_type& size) 
				:sizeType_m(size), vec_m(size) {
				//std::cout << "Enter vector size constructor\n";
				//full size un-init constructor, init with default values?
			}


			template<typename ...Ts>
			Vector(Ts...args)
				: sizeType_m(sizeof...(Ts)), vec_m{ static_cast<T>(args)... } { 
				//std::cout << "Constructor 2" << std::endl;
			}


			template<typename T>
			Vector(std::vector<T> list) //test this
				: sizeType_m(list.size()), vec_m(list) {
				//vec_m.resize(list.size());
				//std::cout << "Constructor 3" << std::endl;
			}
			//maybe add pass by reference? etc.			


			Vector(const dmml::linalg::Vector<T>& v2)
				: sizeType_m(v2.sizeType_m), vec_m(v2.vec_m) {}


			//OPERATORS
			dmml::linalg::Vector<T>& operator=(const dmml::linalg::Vector<T>& copyFrom) {
				std::cout << "Vector operator=\n";
				this->sizeType_m = copyFrom.sizeType_m;
				this->vec_m = copyFrom.vec_m;

				return *this;
			}


			template<typename T2>
			dmml::linalg::Vector<T>& operator=(const dmml::linalg::Vector<T2>& copyFrom) {
				this->sizeType_m = copyFrom.sizeType_m;
				this->vec_m = std::vector<T>(copyFrom.vec_m.begin(), copyFrom.vec_m.end());
				return *this;
			}


			bool operator==(const dmml::linalg::Vector<T>& compTo) const { //make more robust, compare other members?---------------------------

				if (this->vec_m == compTo.vec_m)
					return true;

				return false;
			}


			template<typename T2>
			auto operator-(const dmml::linalg::Vector<T2>& v2) {
				auto retVec(*this);
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					retVec.vec_m[i] -= v2.vec_m[i];
				}
				return retVec;
			}


			template<typename T2>
			auto& operator-=(const dmml::linalg::Vector<T2>& v2) {
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					this->vec_m[i] -= v2.vec_m[i];
				}
				return *this;
			}


			template<typename T2>
			auto operator+(const dmml::linalg::Vector<T2>& v2) {
				auto retVec(*this);
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					retVec.vec_m[i] += v2.vec_m[i];
				}
				return retVec;
			}


			template<typename T2>
			auto& operator+=(const dmml::linalg::Vector<T2>& v2) {
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					this->vec_m[i] += v2.vec_m[i];
				}
				return *this;
			}


			template<typename T2>
			auto operator*(const T2& scalar) {
				std::cout << "Operator*\n";
				auto retVec(*this);
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					retVec.vec_m[i] *= scalar;
				}
				return retVec;
			}


			template<typename T2>
			auto& operator*=(const T2& scalar) {
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					this->vec_m[i] *= scalar;
				}
				return *this;
			}

			
			//GETTERS
			typename std::vector<T>::size_type GetSize() const { return this->sizeType_m; }
			
			typename std::vector<T> GetValues() const { return this->vec_m; }

			T GetValue(uint64_t idx) const { return this->vec_m[idx]; }


			void ShowVector() const {
				//display type as well?
				std::cout << "Size: " << sizeType_m << '\n';
				std::cout << "Type: " << typeid(this->vec_m).name() << '\n';

				typename std::vector<T>::const_iterator iter = this->vec_m.cbegin();
				for (iter; iter != this->vec_m.cend(); iter++) {
					std::cout << *iter << ' ';
				}
				std::cout << "\n\n";
			}



			//SETTERS
			void Append(T val) { this->vec_m.push_back(val); }

			void AssignElement(T val, uint64_t idx) {
				this->vec_m[idx] = val;
			}


			//MEMBER FUNCS
			template<typename T2>
			auto AddVector(const dmml::linalg::Vector<T2>& v2) {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (this->sizeType_m != v2.sizeType_m) {
						throw(this->GetSize());
					}
				}
				catch (typename std::vector<dervType>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->sizeType_m << " and passed vector of length " << v2.sizeType_m << '.' << std::endl;
					
					return dmml::linalg::Vector<dervType>();
				}

				auto retVec = dmml::linalg::Vector<dervType>(this->sizeType_m); //issues with instantiating with sizeType using derived types? does this implicitly instatiate another class (desired)?
				retVec = (*this) + v2;
				
				return retVec;
			}

			
			template <typename T2>
			auto SubtractVector(const dmml::linalg::Vector<T2>& v2) {  

				using dervType = typename std::common_type<T, T2>::type;

				try { //check for 0 length vectors?
					if (!is_Vector<std::decay_t<decltype(v2)>, dmml::linalg::Vector>) { //almost unnecessary?
						throw(typeid(v2).name());
					}

					if (this->sizeType_m != v2.GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Type Error" << std::endl;
					return dmml::linalg::Vector<dervType>((dervType)1);
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->GetSize() << " and passed vector of length " << v2.GetSize() << '.' << std::endl;
					
					return dmml::linalg::Vector<dervType>((dervType)1);
				}
	
				auto retVec = dmml::linalg::Vector<dervType>(this->sizeType_m); //could store first value instead of recomputing, checking indexes tho
				retVec = (*this) - v2;

				return retVec;
			}


			template <typename T2>
			auto VecMult(const T2& scalar) {
				
				using dervType = typename std::common_type<T, T2>::type;
				auto retVec = dmml::linalg::Vector<dervType>(this->sizeType_m);
				retVec = (*this) * scalar;

				return retVec;
			}


			template <typename T2> 
			double DotProduct(const dmml::linalg::Vector<T2>& v2) { 	
								
				try {	
					if (!is_Vector<std::decay_t<decltype(v2)>, dmml::linalg::Vector>) { //almost unnecessary?
						throw(typeid(v2).name());
					}
					
					if (this->sizeType_m != v2.GetSize()) {
						throw(this->GetSize());
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Type Error" << std::endl;
					return std::numeric_limits<double>::min();
				}
				catch (const typename std::vector<T>::size_type&) {
					std::cout << "Vectors are of differing dimensions. Invoked vector of length " <<
						this->sizeType_m << " and passed vector of length " << v2.sizeType_m << '.' << std::endl;
					
					return std::numeric_limits<double>::min();
				}

				
				double dProd = 0.0;
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					dProd += this->vec_m[i] * v2.vec_m[i];
				}

				return dProd;
			}


			template <typename T2>
			auto OuterProduct(const dmml::linalg::Vector<T2>& v2) {

				using dervType = typename std::common_type<T, T2>::type;
				
				if (this->sizeType_m != v2.sizeType_m) {
					//throw size mismatch error
				}
				
				auto retMat = dmml::linalg::Matrix<dervType>(this->sizeType_m, v2.sizeType_m);
				
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					for (std::size_t j = 0; j != v2.sizeType_m; ++j) {
						retMat(i, j) = this->vec_m[i] * v2.vec_m[j];
					}
				}

				return retMat;

			}


			double L2Norm() const {
				try {
					if (this->GetSize() == 0) {
						throw(this->sizeType_m);
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of length 0" << std::endl;
					return 0;
				}

				double norm = 0.0;
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					norm += this->vec_m[i] * this->vec_m[i];
				}

				return std::sqrt(norm);
			}


			template<typename T2>
			auto HadamardProduct(const dmml::linalg::Vector<T2>& v2) {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (!is_Vector<std::decay_t<decltype(v2)>, dmml::linalg::Vector>) { //almost unnecessary?
						throw(typeid(v2).name());
					}

					if (this->sizeType_m != v2.GetSize()) {
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

				auto retVec = dmml::linalg::Vector<dervType>(this->sizeType_m);	
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					retVec.vec_m[i] = this->vec_m[i] * v2.vec_m[i];
				}

				return retVec;
			}


			dmml::linalg::Vector<double>& ConvertUnitVector() {

				try {
					if (this->GetSize() == 0) {
						throw(this->sizeType_m);
					}
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return *this;
				}

				double vecNorm = this->L2Norm();

				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					this->vec_m[i] = (static_cast<double>(this->vec_m[i]) / vecNorm);
				}

				return *this;
			}


			dmml::linalg::Vector<double>& ConvertUnitVector(dmml::linalg::Vector<double>& unit) {
				try {
					if (this->GetSize() == 0) {
						throw(this->sizeType_m);
					} 
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Vector of size 0." << std::endl;
					return dmml::linalg::Vector<double>();
				}

				double vecNorm = this->L2Norm();
				auto unit = dmml::linalg::Vector<double>(this->sizeType_m);
				//RESIZE VECTOR? 
				for (std::size_t i = 0; i != this->sizeType_m; ++i) {
					unit.vec_m[i] = (static_cast<double>(this->vec_m[i]) / vecNorm);
				}

				return unit;
			}
	
			
			/*dmml::linalg::Vector<double> ConvertUnitVector_() {

				return dmml::linalg::Vector<double>)();
			}*/





		private:
	
			//MEMBER VALUES
			//using valType_m = typename std::vector<T>::value_type;
			typename std::vector<T>::size_type sizeType_m = 0;
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
			template <typename U> friend class Matrix;
			template <typename V> friend class Vector;

			//CONSTRUCTORS - need overloads for lvalue/rvalue args and vector list constructor
			//in constructors check if T matches Ts
			Matrix() {}

			
			Matrix(std::size_t rows, std::size_t cols)
				:rowSize_m(rows), colSize_m(cols), numEle_m(rows * cols), matrix_m(std::vector<T>(rows * cols, 0)) { }


			template<typename ...Ts>
			Matrix(std::size_t rows, std::size_t cols, Ts...args)
				: rowSize_m(rows), colSize_m(cols), numEle_m(rows* cols), matrix_m{ static_cast<T>(args)... } { //for numEle_m, better to do mult or sizeof(args...)?
			}


			Matrix(const dmml::linalg::Matrix<T>& m2)
				: rowSize_m(m2.rowSize_m), colSize_m(m2.colSize_m), numEle_m(m2.numEle_m), matrix_m(m2.matrix_m) { }


			//OPERATORS
			dmml::linalg::Matrix<T>& operator=(const dmml::linalg::Matrix<T>& copyFrom) {
				
				this->colSize_m = copyFrom.colSize_m;
				this->rowSize_m = copyFrom.rowSize_m;
				this->numEle_m = copyFrom.numEle_m;
				this->matrix_m = copyFrom.matrix_m;
			
				return *this;
			}


			template<typename T2> 
			dmml::linalg::Matrix<T>& operator=(const dmml::linalg::Matrix<T2>& copyFrom) {
				this->colSize_m = copyFrom.colSize_m;
				this->rowSize_m = copyFrom.rowSize_m;
				this->numEle_m = copyFrom.numEle_m;
				this->matrix_m = std::vector<T>(copyFrom.matrix_m.begin(), copyFrom.matrix_m.end());
				return *this;
			}


			bool operator==(const dmml::linalg::Matrix<T>& compTo) const { //make more robust, compare other members?---------------------------

				if (this->matrix_m == compTo.matrix_m)
					return true;
				
				return false;
			}


			template<typename T2>
			auto operator-(const dmml::linalg::Matrix<T2>& m2) {
				//need matrix size check, could use std::transform?---------------------------
				auto retMat(*this);
				for (std::size_t i = 0; i != this->numEle_m; ++i) {
					retMat.matrix_m[i] -= m2.matrix_m[i];
				}
				return retMat;
			}


			template<typename T2>
			auto& operator-=(const dmml::linalg::Matrix<T2>& m2) {
				//need matrix size check
				for (std::size_t i = 0; i != this->numEle_m; ++i) {
					this->matrix_m[i] -= m2.matrix_m[i];
				}
				return *this;
			}


			template<typename T2>
			auto operator+(const dmml::linalg::Matrix<T2>& m2) {
				//need matrix size check
				auto retMat(*this);
				for (std::size_t i = 0; i != this->numEle_m; ++i) {
					retMat.matrix_m[i] += m2.matrix_m[i];
				}
				return retMat;
			}


			template<typename T2>
			auto& operator+=(const dmml::linalg::Matrix<T2>& m2) {
				//need matrix size check
				for (std::size_t i = 0; i != this->numEle_m; ++i) {
					this->matrix_m[i] += m2.matrix_m[i];
				}
				return *this;
			}


			inline std::size_t GetLinearIndex(const std::size_t row, const std::size_t col) const {
				return this->colSize_m * row + col;
			}

			inline auto& operator()(const std::size_t row, const std::size_t col) const {
				return this->matrix_m[GetLinearIndex(row, col)];
			}

			inline auto& operator()(const std::size_t row, const std::size_t col) {
				return this->matrix_m[GetLinearIndex(row, col)];
			}


			//GETTERS
			typename std::vector<T> GetValues() const { return this->matrix_m; }

			std::size_t GetRowSize() const { return this->rowSize_m; }

			std::size_t GetColSize() const { return this->colSize_m; }

			std::size_t GetNumElements() const { return this->numEle_m; }


			void ShowMatrix() const {
				//print with better formatting?------------------------------
				std::cout << "Size: " << this->rowSize_m << 'x' << this->colSize_m << '\n';
				std::cout << "Type: " << typeid(this->matrix_m).name() << '\n';

				for (std::size_t r = 0; r < this->rowSize_m; ++r) {
					for (std::size_t c = 0; c < this->colSize_m; ++c) {
						std::cout << std::fixed << std::setprecision(4) << (*this)(r, c) << "  ";
					}
					std::cout << '\n';
				}
				std::cout << '\n';
			}


			//SETTERS
			void AssignElement(std::size_t row, std::size_t col, T val) {
				(*this)(row, col) = val;
			}


			void AddElement(T val, uint64_t row, uint64_t col) {
				(*this)(row, col) += val;
			}


			void SubtractElement(T val, uint64_t row, uint64_t col) {
				(*this)(row, col) -= val;
			}


			//MEMBERS
			void Transpose() {

				auto tpose = dmml::linalg::Matrix<T>(this->colSize_m, this->rowSize_m);
				
				for (std::size_t r = 0; r < this->rowSize_m; ++r) {
					for (std::size_t c = 0; c < this->colSize_m; ++c) {
						tpose(c, r) = (*this)(r, c);
					}
				}
				
				this->rowSize_m ^= this->colSize_m;
				this->colSize_m ^= this->rowSize_m;
				this->rowSize_m ^= this->colSize_m;
				
				*this = tpose;
			}


			template<typename T2>
			auto MatMul(const dmml::linalg::Matrix<T2>& m2) {

				using dervType = typename std::common_type<T, T2>::type;

				try {
					if (!is_Matrix<std::decay_t<decltype(m2)>, dmml::linalg::Matrix>) {
						throw(typeid(m2).name());
					}

					if (this->colSize_m != m2.rowSize_m) {
						throw(this->colSize_m); //change to get col size method
					}
				}
				catch (const std::bad_typeid&) {
					std::cout << "Improper type passed." << std::endl;
				}
				catch (typename std::vector<T>::size_type) {
					std::cout << "Matrices of differing sizes." << std::endl;
					return dmml::linalg::Matrix<dervType>(1, 1, static_cast<dervType>(1));
				}
				 
				auto retMat = dmml::linalg::Matrix<dervType>(this->colSize_m, m2.rowSize_m);

				std::size_t BLOCK_SIZE = 64 / sizeof(T); //make dependent on hardware/size of matrix?

				if (this->rowSize_m < BLOCK_SIZE || this->colSize_m < BLOCK_SIZE) { //for now if smaller than cache line, make block size 1
					BLOCK_SIZE = 1;
				}

				for (std::size_t ib = 0; ib < this->rowSize_m; ib += BLOCK_SIZE) {
					for (std::size_t kb = 0; kb < this->colSize_m; kb += BLOCK_SIZE) {
						for (std::size_t jb = 0; jb < m2.colSize_m; jb += BLOCK_SIZE) {

							const T* blockA = &(*this)(ib, kb);
							const T* blockB = &(m2(kb, jb));
							T* posC = &(retMat(ib, jb));

							for (std::size_t i = 0; i < BLOCK_SIZE; ++i, posC += m2.colSize_m, blockA += this->colSize_m) { //convert to own func
								const T* b = blockB;
								for (std::size_t k = 0; k < BLOCK_SIZE; ++k, b += m2.colSize_m) {
									for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
										posC[j] += blockA[k] * b[j];
									}
								}
							}

						}
					}
				}

				
				/*for (std::size_t m1R = 0; m1R < this->rowSize_m; m1R++) {
					for (std::size_t m2R = 0; m2R < m2->colSize_m; m2R++) {
						for (std::size_t m1C = 0; m1C < this->colSize_m; m1C++) {
							retMat(m1R, m1C) += (*this)(m1R, m2R) * (*m2)(m2R, m1C); 
						}
					}
				}*/
				return retMat;
			}
			

			void MatMul_(const int64_t&& scalar) { //need matmul for vecotr/matrix multiplocations as well


			}


			//when chaining this with a constructor, is this constructing a nerw matrix, and destructing/replacing the matrix made by the class constructor?? INVESTIGATE
			//make special constructor for identity matrix? way to make this faster?
			//should return reference?
			dmml::linalg::Matrix<T> IdentityMatrix(const uint64_t&& size) { //change type on identity matrix to be more flexible? template?

				//auto idMatrix = dmml::linalg::Matrix<T>(size, size);
				for (std::size_t i = 0; i < this->colSize_m; ++i) {
					(*this)(i, i) = 1;
				}

				return *this;
			}

			
			//create a matrix with certain size with an entry down the diagonal, almost identical to ID matrix
			dmml::linalg::Matrix<T> DiagMatrix(const T& val, const uint64_t& rows, const uint64_t& cols = rows) {

				//auto diagMat = dmml::linalg::Matrix<T>(rows, cols);
				for (std::size_t i = 0; i < this->colSize_m; ++i) {
					(*this)(i, i) = val;
				}

				return *this;
			}


			double Determinant() {

				if (this->rowSize_m == this->colSize_m) {
					return this->SquareDeterminant();
				}
				else {
					std::cout << "No viable option for finding determinant of this matrix at this time.\n";
				}
				return 0.0;
			}


			double SquareDeterminant() {
				
				if (this->rowSize_m == 2) {
					return (double)(*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
				}
				else if (this->rowSize_m == 3) {
					return (double)(((*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2)) +
						((*this)(0, 1) * (*this)(1, 2) * (*this)(2, 0)) +
						((*this)(0, 2) * (*this)(1, 0) * (*this)(2, 1)) -
						((*this)(0, 2) * (*this)(1, 1) * (*this)(2, 0)) -
						((*this)(0, 1) * (*this)(1, 0) * (*this)(2, 2)) -
						((*this)(0, 0) * (*this)(1, 2) * (*this)(2, 1)));
						
				}
				return 0.0;
			}


			dmml::linalg::Matrix<T> ConvertUpperHessenberg() {

			}


			//figure out return type deductions?
			//bunch of stuff to rework/optimize here (esp for fewer function calls, make dedicated gramm-schmitt func?
			//create a (lin alg) projection function?
			std::pair<dmml::linalg::Matrix<T>, dmml::linalg::Matrix<T>> QRDecomposition() {

				auto Q = dmml::linalg::Matrix<double>(this->rowSize_m, this->colSize_m);
				auto R = dmml::linalg::Matrix<double>(this->colSize_m, this->colSize_m);
				
				auto tempCol = dmml::linalg::Vector<double>(this->rowSize_m);
				for (std::size_t cIter = 0; cIter < this->colSize_m; cIter++) {
					//COL WISE CONSTRUCTOR?-------------------------------
					for (std::size_t rIter = 0; rIter != this->rowSize_m; rIter++) {
						tempCol.AssignElement((*this)(rIter, cIter), rIter);
					}

					auto qPrev = dmml::linalg::Vector<double>(this->rowSize_m);
					for (std::size_t qRem = 0; qRem < cIter; ++qRem) { //for (uint64_t qRem = cIter - 1; qRem >= 0; qRem--) {
						
						for (std::size_t q = 0; q < this->rowSize_m; q++) {
							//aPerp.AssignElement((*this)(q, cIter), q);
							qPrev.AssignElement(Q(q, qRem), q);
						}
						
						//calculate dot products here, store in R matrix
						auto upperR = tempCol.DotProduct(qPrev);
						R(qRem, cIter) = upperR;
						qPrev *= upperR;
						tempCol -= qPrev;

						//printf("Dot Prod, C-col %d, Q-col %d: %f\n", (int)cIter, (int)qRem, upperR);
					}

					//normalize, put in Q matrix (col wise)
					//L2 norm of un-normalized vec goes on diag of R
					R(cIter, cIter) = tempCol.L2Norm();
					tempCol.ConvertUnitVector();
					for (std::size_t qIter = 0; qIter < this->colSize_m; qIter++) {
						Q(qIter, cIter) = tempCol.vec_m[qIter];
					}
				}
				
				return std::pair<dmml::linalg::Matrix<double>, dmml::linalg::Matrix<double> >(Q, R);
			}


			//needs upper hesenberg transformation
			//include deflation? checking for when certain values arent converging, check eigenvalues of submatrix/matrices, delfate, continue
			dmml::linalg::Matrix<double> QRAlgorithm(const double&& epsilon) {

				//what is getting constructed/copied here? should DiagMatrix return a reference?
				auto shift = dmml::linalg::Matrix<double>(this->rowSize_m, this->colSize_m).DiagMatrix((*this)(this->rowSize_m - 1, this->colSize_m - 1), this->rowSize_m, this->colSize_m); 
				
				//THIS REQUIRES TYPE CONVERSION FOR MATRICES, REWRITE MATRIX_M TO SINGLE VECTOR FOR EASIER CONVERSION
				//using dervType = typename std::common_type<T, double>::type;
				//dmml::linalg::Matrix<double> nextA(this->rowSize_m, this->colSize_m);
				//nextA = *this - shift; 

				dmml::linalg::Matrix<double> nextA(*this);
				nextA -= shift;
				//nextA.ShowMatrix();
				auto QR = std::make_pair(dmml::linalg::Matrix<double>(this->rowSize_m, this->colSize_m), dmml::linalg::Matrix<double>(this->rowSize_m, this->colSize_m));
				

				while (std::abs(nextA(this->rowSize_m - 1, this->colSize_m - 2)) > epsilon) { //heuristic check, extend to all lower half values, or different single value?
					QR = nextA.QRDecomposition();
					//QR.first.ShowMatrix();
					//QR.second.ShowMatrix();
					nextA = QR.second.MatMul(QR.first);
					//nextA.ShowMatrix();
				}
				//nextA.ShowMatrix();
				nextA += shift;
				return nextA;
			}



		
			
			void SingularValueDecomposition(const dmml::linalg::Matrix<T>* const matrix) const {
				
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
			

				
		private:

			//MEMBER VALUES
			//typename std::vector<std::vector<T>>::size_type rowSize_m {};
			//typename std::vector<T>::size_type colSize_m {};
			//std::vector<std::vector<T>> matrix_m {}; //convert to single array
			
			std::size_t rowSize_m = 0;
			std::size_t colSize_m = 0;
			std::size_t numEle_m = 0;
			std::vector<T> matrix_m {};

			//need destructor, copy constructor, and copy assignment operator


			//SETTERS
			





			//PRIVATE MEMBER FUNCS

		};
	}
}
