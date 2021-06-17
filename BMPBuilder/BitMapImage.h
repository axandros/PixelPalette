#pragma once
#include <vector>
#include <string>

class BitMapImage
{
public:
	// Make a bmp file.  No extensions are added to the file name.
	void WriteToFile(std::string fileToWrite);

	// Add the string Hex Code to the image.  Must be exactly 6 characters long, RGB values.  Returns false if string cannot be prob=perly parsed.
	bool addColor(std::string hexCode);

private:
	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
	};
	std::vector<BitMapImage::Pixel> originalPixels;
	std::vector<BitMapImage::Pixel> hexPixels;

	bool addColor(std::string hexCode, std::vector<BitMapImage::Pixel>& pixelArray);

	void makeFileHeader(std::vector<unsigned char>& bmp);
	void makeDIBHeader(std::vector<unsigned char>& bmp);
	void addPixelArray(std::vector<unsigned char>& bmp);
	bool getHex(std::string input, unsigned char& output);
	unsigned char getHex(std::string input);

	void setFileSize(std::vector<unsigned char>& bmp);
	void setFileWidthAndHeight(std::vector<unsigned char>& bmp);
	void setFileColorsUsed(std::vector<unsigned char>& bmp);
	
	void setColorArraySizeBytes();
	void setImageWidth();
	void setImagePadding();

	int width;
	int padding;
	int colorBytes;

};

