#include <iostream>
#include <fstream>
//#include "src/Functions.cpp"
#include "../9FinalLaba/inc/FunctionsHeader.hpp"
#include "../9FinalLaba/inc/mathutils/matrix.hpp"


int main() {
	BMP bmp_Image(2000, 2000);
	bmp_Image.Read();
	//bmp_Image.Pixels();
	bmp_Image.DarkFilter();
	//bmp_Image.Rotate(acos(-1) / 4);
	bmp_Image.Correct();
	bmp_Image.Write();

	//bmp_Image.Clean();
	//bmp_Image.Rotate(acos(-1)/4);
	return 0;
}


