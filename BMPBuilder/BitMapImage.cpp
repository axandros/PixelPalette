#include "BitMapImage.h"
#include <vector>
#include <string>
#include <fstream>

#include <iostream>

using namespace std;

void BitMapImage::WriteToFile(std::string fileToWrite)
{
	vector<unsigned char> fileBytes;

	addPixelArray(fileBytes, true);
	makeFileHeader(fileBytes);
	makeDIBHeader(fileBytes);
	setFileSize(fileBytes);

	cout << "BMP Size: " << dec << fileBytes.size() << endl;

	std::ofstream file;
	file.open(fileToWrite.c_str(), std::ios_base::binary);
	
	if (file.is_open()) {
		//cout << dec << "Hex: " << 51 << "Dec: " << 0x51 ;
		file.write((char*)&fileBytes[0], fileBytes.size() * sizeof(char));
		for (int i = 0; i < fileBytes.size(); i++) {
			file.write((char*)&fileBytes[i], sizeof(char));
			file.write((char*)&fileBytes[i], sizeof(char));
			//cout << dec << i << ": " << hex << (int)fileBytes[i] << endl;
		}
		//cout << endl;
		file.close();
	}
}

bool BitMapImage::addColor(std::string hexCode) {
	return addColor(hexCode, originalPixels);
}

bool BitMapImage::addColor(std::string hexCode, std::vector<BitMapImage::Pixel>& pixelArray)
{
	//cout << "Adding Color: " << hexCode << endl;
	bool ret = false;

	//cout << "hexCode Size: " << hexCode.size() << endl;

	if (hexCode.size() == 6) {
		Pixel pix = Pixel();
		unsigned char red = 0x00;
		unsigned char green = 0x00;
		unsigned char blue = 0x00;
		if (getHex(hexCode.substr(0, 2), red) && getHex(hexCode.substr(2, 2), green) && getHex(hexCode.substr(4, 2), blue)) {
			pix.r = red;
			pix.g = green;
			pix.b = blue;

			pixelArray.push_back(pix);
			ret = true;
		}
	}
	return ret;
}

void BitMapImage::makeFileHeader(vector<unsigned char>& bmp)
{
	// BM - indicates bitmap file.
	bmp[0] = 0x42;
	bmp[1] = 0x4D;

	// File size
	setFileSize(bmp);

	// Reserved areas
	bmp[6] = 0x00; bmp[7] = 0x00; bmp[8] = 0x00; bmp[9] = 0x00;

	// Offset where image starts
	bmp[10] = 0x36; bmp[11] = 0x00; bmp[12] = 0x00; bmp[13] = 0x00;

	return;
}

void BitMapImage::makeDIBHeader(std::vector<unsigned char>& bmp)
{
	// Size of the header
	bmp[14] = 0x28; bmp[15] = 0x00; bmp[16] = 0x00; bmp[17] = 0x00;

	// Horizontal Width of the image && Vertical Height of the image
	setFileWidthAndHeight(bmp);
	
	// Number of Planes - 0?
	bmp[26] = 0x01; bmp[27] = 0x00;
	// Bits per pixel - consult documentation.
	bmp[28] = 0x18; bmp[29] = 0x00;

	// Compression. 0-None
	bmp[30] = 0x00; bmp[31] = 0x00; bmp[32] = 0x00; bmp[33] = 0x00;
	// Size of image (Compressed)
	bmp[34] = 0x00; bmp[35] = 0x00; bmp[36] = 0x00; bmp[37] = 0x00;

	// Horizontal Resolution
	bmp[38] = 0x00; bmp[39] = 0x00; bmp[40] = 0x00; bmp[41] = 0x00;
	// Vertical Resolution
	bmp[42] = 0x00; bmp[43] = 0x00; bmp[44] = 0x00; bmp[45] = 0x00;

	// Number of used colors
	setFileColorsUsed(bmp);
	// Number of important colors
	bmp[50] = 0x00; bmp[51] = 0x00; bmp[52] = 0x00; bmp[53] = 0x00;
}

void BitMapImage::addPixelArray(std::vector<unsigned char>& bmp, bool debug)
{
	// BGR  - Its reversed from normal R-G-B
	
	setColorArraySizeBytes();

	int offset = 54;
	int sizeOfArray = offset+colorBytes;
	bmp.resize(sizeOfArray);
	
	int bytesPerRow = width * 3 + padding;

	std::ofstream debugFile;
	if (debug) {
		cout << "opening debug file." << endl;
		debugFile.open("PixelArrayDebug.txt", ofstream::app );
		ofstream::iostate curState = debugFile.rdstate();
		cout << "DebugState: " << ((curState==ofstream::goodbit)?"good":"bad") << endl;
		cout << "File Opened: " << (debugFile.is_open() ? "open" : "closed") << endl;
	}

	cout << "ColorBytes: " << colorBytes << endl;
	cout << "BytesPerRow: " << bytesPerRow << endl;
	cout << "Width: " << width << endl;
	
	for (int y = 0; y < width; y++) {
		cout << "Row: " << y << endl;
		int rowOffset = offset + y * bytesPerRow;
		for (int x = 0; x < width; x++) {
			cout << "Column : " << x << endl;
			
			int pixel = width * y + x;
			int bytePos = rowOffset + x * 3;
			Pixel tempPix = hexPixels[pixel];
			cout << "Pixel " << flush << hex << (int)tempPix.r 
				<< hex << (int)tempPix.g
				<< hex << (int)tempPix.b << endl;
			bmp[bytePos + 0] = tempPix.b;
			bmp[bytePos + 1] = tempPix.g;
			bmp[bytePos + 2] = tempPix.r;
			if (debugFile.is_open()) {
				cout << "Debug file open." << endl;
				const char* i = new char('a');
				debugFile.write(i/*(const char*)tempPix.b*/, sizeof(char));
				debugFile.write(i/*(const char*)tempPix.g*/, sizeof(char));
				debugFile.write(i/*(const char*)tempPix.r*/, sizeof(char));
			}
			
		}
		for (int p = 0; p < padding; p++) {
			bmp[rowOffset + width * 3 + p] = 0x00;
			if (debugFile.is_open()) { debugFile.write("00", sizeof(char)); }
		}
	}

	// Fill the spare spaces.

	// Line 1
	//bmp[54] = 0x00; bmp[55] = 0xff; bmp[56] = 0x00;
	//bmp[57] = 0x00; bmp[58] = 0xff; bmp[59] = 0x00;
	//bmp[54] = getHex("00"); bmp[55] = getHex("ff"); bmp[56] = getHex("00");
	//bmp[57] = getHex("ff"); bmp[58] = getHex("ff"); bmp[59] = getHex("ff");
	
	// Padding for line one to make the number of pixels divisible by 4
	//bmp[60] = 0x00; bmp[61] = 0x00;

	// Line 2
	//bmp[62] = 0x00; bmp[63] = 0x00; bmp[64] = 0xff;
	//bmp[65] = 0x00; bmp[66] = 0x00; bmp[67] = 0x00;
	//bmp[62] = getHex("aa"); bmp[63] = getHex("00"); bmp[64] = getHex("ff");
	//bmp[65] = getHex("00"); bmp[66] = getHex("FF"); bmp[67] = getHex("ff");
	
	// Padding for line one to make the number of pixels divisible by 4
	//bmp[68] = 0x00; bmp[69] = 0x00;

	if (debugFile.is_open()) { 
		debugFile.close();
		cout << "closing debug file." << endl;
	}
}

unsigned char BitMapImage::getHex(string input) {
	unsigned char ret = 0x00;
	if(!getHex(input, ret)) {
		ret = 0x00;
	}
	return ret;
}

void BitMapImage::setFileSize(vector<unsigned char>& bmp)
{
	int totalSizeBytes = colorBytes + 54;
	unsigned char byte1, byte2, byte3, byte4 = 0x00;

	int byte4Size = 256*256*256;
	int byte4Value = totalSizeBytes % byte4Size;
	int temp = float(totalSizeBytes) / float(byte4Size);
	byte4 = unsigned char(byte4Value);

	int byte3Size = 256 * 256;
	int byte3Value = temp % byte3Size;
	temp = float(temp) / float(byte3Size);
	byte3 = unsigned char(byte3Value);

	int byte2Size = 256;
	int byte2Value = temp % byte2Size;
	temp = float(temp) / float(byte2Size);
	byte2 = unsigned char(byte2Value);
	byte1 = unsigned char(temp);
	/*
	cout << "Hex Size: " << std::hex << (int)byte1
		<< " | " << hex << (int)byte2 
		<< " | " << hex << (int)byte3 
		<< " | " << hex << (int)byte4 << endl;
		*/

	bmp[2] = byte4; bmp[3] = byte3; bmp[4] = byte2; bmp[5] = byte1;

}

void BitMapImage::setFileWidthAndHeight(std::vector<unsigned char>& bmp)
{
	unsigned char byte1, byte2, byte3, byte4 = 0x00;

	int byte4Size = 256 * 256 * 256;
	int byte4Value = width % byte4Size;
	int temp = float(width) / float(byte4Size);
	byte4 = unsigned char(byte4Value);

	int byte3Size = 256 * 256;
	int byte3Value = temp % byte3Size;
	temp = float(temp) / float(byte3Size);
	byte3 = unsigned char(byte3Value);

	int byte2Size = 256;
	int byte2Value = temp % byte2Size;
	temp = float(temp) / float(byte2Size);
	byte2 = unsigned char(byte2Value);
	byte1 = unsigned char(temp);

	// Horizontal Width of the image
	bmp[18] = byte4; bmp[19] = byte3; bmp[20] = byte2; bmp[21] = byte1;
	// Vertical Height of the image
	bmp[22] = byte4; bmp[23] = byte3; bmp[24] = byte2; bmp[25] = byte1;
}

void BitMapImage::setFileColorsUsed(std::vector<unsigned char>& bmp)
{
	int colorsUsed = originalPixels.size()-1;
	if (colorsUsed < 0) { colorsUsed = 0; }
	unsigned char byte1, byte2, byte3, byte4 = 0x00;

	int byte4Size = 256 * 256 * 256;
	int byte4Value = colorsUsed % byte4Size;
	int temp = float(colorsUsed) / float(byte4Size);
	byte4 = unsigned char(byte4Value);

	int byte3Size = 256 * 256;
	int byte3Value = temp % byte3Size;
	temp = float(temp) / float(byte3Size);
	byte3 = unsigned char(byte3Value);

	int byte2Size = 256;
	int byte2Value = temp % byte2Size;
	temp = float(temp) / float(byte2Size);
	byte2 = unsigned char(byte2Value);
	byte1 = unsigned char(temp);

	bmp[46] = byte4; bmp[47] = byte3; bmp[48] = byte2; bmp[49] = byte1;
}

void BitMapImage::setColorArraySizeBytes()
{
	hexPixels.clear();
	cout << "setColorArraySizeBytes: Start Populating hexPixels" << endl;
	for (int i = 0; i < originalPixels.size(); i++) {
		cout << "setColorArraySizeBytes: Adding " << originalPixels[i].toString() << " to hexPixels" << endl;
		hexPixels.push_back(originalPixels[i]);
	}

	setImageWidth();
	setImagePadding();
	int spare = width * width - hexPixels.size();
	
	
	for (int i = 0; i < spare; i++) {
		addColor("ffffff", hexPixels);
	}
	colorBytes = hexPixels.size() * 3 + width * padding;
}

void BitMapImage::setImageWidth()
{
	
	cout << "setImageWidth| Hex Pixels Size: " << hexPixels.size() << endl;
	double root = sqrt(hexPixels.size());
	cout << "setImageWidth| Root: " << root << endl;
	float whole, fractional;
	fractional = std::modf(root, &whole);
	cout << "setImageWidth| fractional: " << fractional << endl;
	cout << "setImageWidth| whole: " << whole << endl;
	width = whole;
	if (fractional != 0) { // If it isn't a root
		width++;
	}
}

void BitMapImage::setImagePadding()
{
	// How much padding do we need for each line?
	padding = (width * 3) % 4;
}

bool BitMapImage::getHex(string input, unsigned char& output)
{
	//cout << "Get Hex: " << input;// << endl;
	//cout << "Hex Size: " << input.size() << endl;
	bool ret = false;
	output = 0x00;
	if (input.size() == 2) {
		bool legal = true;
		unsigned char val = 0x00;
		switch (input[0])
		{
		case '0': break;
		case '1': val += 0x10; break;
		case '2': val += 0x20; break;
		case '3': val += 0x30; break;
		case '4': val += 0x40; break;
		case '5': val += 0x50; break;
		case '6': val += 0x60; break;
		case '7': val += 0x70; break;
		case '8': val += 0x80; break;
		case '9': val += 0x90; break;
		case 'a': val += 0xa0; break;
		case 'b': val += 0xb0; break;
		case 'c': val += 0xc0; break;
		case 'd': val += 0xd0; break;
		case 'e': val += 0xe0; break;
		case 'f': val += 0xf0; break;
		case 'A': val += 0xa0; break;
		case 'B': val += 0xb0; break;
		case 'C': val += 0xc0; break;
		case 'D': val += 0xd0; break;
		case 'E': val += 0xe0; break;
		case 'F': val += 0xf0; break;
		default: legal = false;
			cout << "input 0 is not valid" << endl;
			break;
		}
		switch (input[1])
		{
		case '0': break;
		case '1': val += 0x01; break;
		case '2': val += 0x02; break;
		case '3': val += 0x03; break;
		case '4': val += 0x04; break;
		case '5': val += 0x05; break;
		case '6': val += 0x06; break;
		case '7': val += 0x07; break;
		case '8': val += 0x08; break;
		case '9': val += 0x09; break;
		case 'a': val += 0x0a; break;
		case 'b': val += 0x0b; break;
		case 'c': val += 0x0c; break;
		case 'd': val += 0x0d; break;
		case 'e': val += 0x0e; break;
		case 'f': val += 0x0f; break;
		case 'A': val += 0x0a; break;
		case 'B': val += 0x0b; break;
		case 'C': val += 0x0c; break;
		case 'D': val += 0x0d; break;
		case 'E': val += 0x0e; break;
		case 'F': val += 0x0f; break;
		default: legal = false;
			cout << "input 1 is not valid" << endl;
			break;
		}
		if (legal) {
			output = val;
			ret = true;
		}
		else {
			//cout << "Val" << val << "is not legal" << endl;
		}
	}
	//cout << " | " << hex << (int)output << endl;
	return ret;
}

string BitMapImage::Pixel::toString()
{
	string ret = "(" + to_string(r);
	ret += ", " + to_string(g);
	ret += ", " + to_string(b);
	ret += ")";
	return ret;
}
