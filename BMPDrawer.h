#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>


class BMPDrawer
{
public:
	BMPDrawer();
	~BMPDrawer();

	bool openBMP(const std::string&);
	void displayBMP() const;
	bool closeBMP();

private:
	bool pixelsAreEqual(const RGBTRIPLE&, const RGBTRIPLE&) const;

	BITMAPFILEHEADER file_header_;
	BITMAPINFOHEADER info_header_;
	unsigned char* data_; // можно было бы использовать RGBTRIPLE и RGBQUAD
	const size_t alignment_degree_; // выравнивание строк пикселей в файле как степень двойки
	bool is_open_; // флаг, прочитан ли сейчас какой-либо файл
};
