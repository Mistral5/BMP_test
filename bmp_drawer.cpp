#include "bmp_drawer.h"

BMPDrawer::BMPDrawer() : file_header_({ NULL }), info_header_({ NULL }), data_(nullptr), kAlignmentDegree(2), is_open_(false) {};

BMPDrawer::~BMPDrawer()
{
	closeBMP();
};

bool BMPDrawer::openBMP(const std::string& fileName)
{
	if (is_open_) // если другой файл был открыт, но не был закрыт
		closeBMP();

	std::cout << std::endl << "Opening file: " << fileName << std::endl;

	std::ifstream input(fileName, std::ios::binary);

	if (!input.is_open())
	{
		std::cerr << "Unable to open file!" << std::endl;
		return false;
	}

	input.read(reinterpret_cast<char*>(&file_header_), sizeof(file_header_));

	if (file_header_.bfType != 0x4d42 || file_header_.bfReserved1 || file_header_.bfReserved2)
	{
		std::cerr << "Invalid file format!" << std::endl;
		input.close();
		return false;
	}

	input.read(reinterpret_cast<char*>(&info_header_), sizeof(info_header_));

	if (info_header_.biCompression != BI_RGB)
	{
		std::cerr << "Unable to process a compressed file!" << std::endl;
		input.close();
		return false;
	}

	int64_t shift_size = file_header_.bfOffBits - sizeof(info_header_) - sizeof(file_header_);

	if (shift_size < 0)
	{
		std::cerr << "Error in internal file data!" << std::endl;
		input.close();
		return false;
	}

	input.seekg(shift_size, std::ios_base::cur);

	if (info_header_.biBitCount != 24 && info_header_.biBitCount != 32)
	{
		std::cerr << "Unsupported picture bit rate!" << std::endl;
		input.close();
		return false;
	}

	size_t significant_row_size = info_header_.biWidth * (info_header_.biBitCount >> 3);
	size_t round_row_size = ((significant_row_size + (1LL << kAlignmentDegree) - 1) >> kAlignmentDegree) << kAlignmentDegree;

	size_t data_size = info_header_.biHeight * round_row_size;
	data_ = new (std::nothrow) unsigned char[data_size];

	if (!data_)
	{
		std::cerr << "Insufficient memory!" << std::endl;
		input.close();
		return false;
	}

	input.read(reinterpret_cast<char*>(data_), data_size);

	input.close();
	is_open_ = true;
	return true;
};

void BMPDrawer::displayBMP() const
{
	if (!is_open_)
	{
		std::cout << "To start open the image with \"BMPDrawer::openBMP(const std::string&)\"." << std::endl;
		return;
	}

	size_t significant_row_size = info_header_.biWidth * (info_header_.biBitCount >> 3);
	size_t round_row_size = ((significant_row_size + (1LL << kAlignmentDegree) - 1) >> kAlignmentDegree) << kAlignmentDegree;

	for (int64_t i = info_header_.biHeight - 1; i >= 0; --i)
	{
		size_t curr_row = i * round_row_size;
		for (size_t j = 0; j < significant_row_size; j += (info_header_.biBitCount >> 3))
		{
			RGBTRIPLE pixel;
			memcpy(&pixel, &data_[curr_row + j], sizeof(RGBTRIPLE));

			if (pixelsAreEqual(pixel, { 0,0,0 }))
			{
				std::cout << " ";
			}
			else if (pixelsAreEqual(pixel, { 255,255,255 }))
			{
				std::cout << ".";
			}
			else
			{
				std::cerr << "The application works only with monochrome images!" << std::endl;
				return;
			}
		}
		std::cout << std::endl;
	}
};

bool BMPDrawer::closeBMP()
{
	delete[] data_;
	data_ = nullptr;
	is_open_ = false;
	return true;
};

bool BMPDrawer::pixelsAreEqual(const RGBTRIPLE& lhs, const RGBTRIPLE& rhs) const
{
	if (lhs.rgbtRed == rhs.rgbtRed && lhs.rgbtGreen == rhs.rgbtGreen && lhs.rgbtBlue == rhs.rgbtBlue)
		return true;
	return false;
};