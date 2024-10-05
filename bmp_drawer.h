#ifndef BMP_DRAWER_H_
#define BMP_DRAWER_H_

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
	unsigned char* data_;			// ����� ���� �� ������������ RGBTRIPLE � RGBQUAD
	const size_t kAlignmentDegree;	// ������������ ����� �������� � ����� ��� ������� ������
	bool is_open_;					// ����, �������� �� ������ �����-���� ����
};

#endif  // BMP_DRAWER_H_