#pragma once
#include <vector>
#include <string>

class BitMapImage
{
public:
	// Make a bmp file.  No extensions are added to the file name.
	void WriteToFile(std::string fileToWrite);

	// Add the string Hex Code to the image.  Must be exactly 6 characters long, RGB values.  Returns false if string cannot be properly parsed.
	bool addColor(std::string hexCode);

private:
	struct Pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;

		std::string toString();
	};

	// Original Pixels vector stores the colors that the user has passed to the image.
	std::vector<BitMapImage::Pixel> originalPixels;

	// Hex Pixels vector stores the values that will be written  to the file.  These differ due to padding.
	std::vector<BitMapImage::Pixel> hexPixels;
	
	// Adds the hexCode string to the supplied vector.  Returns false if the string cannot be parsed.
	bool addColor(std::string hexCode, std::vector<BitMapImage::Pixel>& pixelArray);

	// Write the general file header to the supplied vector
	void makeFileHeader(std::vector<unsigned char>& bmp);
	// Write the DIB Header to the supplied vecotr
	void makeDIBHeader(std::vector<unsigned char>& bmp);
	// Generate the array of pixels for the bmp image and add them into the input vector.
	void addPixelArray(std::vector<unsigned char>& bmp, bool debug = false);
	// convert the input string to a char representing the hex value.  Returns false if the string cannot be parsed.
	bool getHex(std::string input, unsigned char& output);

	// Convert the input string to a char representing the hex value.
	unsigned char getHex(std::string input);

	char* makeHex(unsigned char hex);

	// Calculate the size of the file, then write that value to the correct position in the vector.
	void setFileSize(std::vector<unsigned char>& bmp);

	// Calculate and set the width and height of the image in the file metadata.
	void setFileWidthAndHeight(std::vector<unsigned char>& bmp);
	// Calculate and set the number of colors used in the image metadata.
	void setFileColorsUsed(std::vector<unsigned char>& bmp);
	

	void setColorArraySizeBytes();
	// Calculate the width (and height) of our square image, based on the number of pixels in originalPixels.
	void setImageWidth();
	void setImagePadding();

	// The width of the image.
	int width;
	// The amount of padding required. ?Is this in bytes or pixels?
	int padding;
	// The number of... colors? in the image?
	int colorBytes;

};
