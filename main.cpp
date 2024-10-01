﻿// main.cpp: определяет точку входа для приложения.
//

#include <iostream> 

#include "BMPDrawer.h"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	if (argc == 1) // можно подряд передать несколько картинок
	{
		cout << "Specify the path of the picture to display." << endl;
		return 0;
	}

	BMPDrawer bmpDrawer;

	for (size_t i = 1; i < argc; ++i)
	{
		cout << endl << "Opening file: " << argv[i] << endl;
		if (bmpDrawer.openBMP(argv[i]))
		{
			bmpDrawer.displayBMP();
			bmpDrawer.closeBMP();
		}
	}

	return 0;
}
