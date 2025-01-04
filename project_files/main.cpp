#include "dmml_linalg.h"





int main() {

	////dmml::linalg::Vector<double> V1 = dmml::linalg::Vector<double>(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
	////dmml::linalg::Vector<double> V2 = dmml::linalg::Vector<double>(4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
	//dmml::linalg::Vector<int> V1 = dmml::linalg::Vector<int>( 1, 2, 3, 4, 5, 6 );
	//dmml::linalg::Vector<int> V2 = dmml::linalg::Vector<int>(4, 5, 6, 7, 8, 9);
	////V1.ShowVector();
	////V2.ShowVector();
	//
	//auto V3 = V1.AddVector(V2);
	//V3.ShowVector();

	//auto prod = V1.DotProduct(V2);
	//std::cout << "V1 * V2: " << prod << std::endl;

	//double norm3 = V3.L2Norm();

	//std::cout << "\nV3 Norm: " << norm3 << '\n';
	//
	//dmml::linalg::Vector<double> temp = dmml::linalg::Vector<double>();
	//V3.ConvertUnitVector(temp);
	int i1 = 1;
	int i2 = 2;
	int i3 = 3;
	int i4 = 4;

	dmml::linalg::Matrix<int> m1 = dmml::linalg::Matrix<int>(i1, i2, i3, i4);
	m1.ShowMatrix();

	return 0;

}