#include <iostream>
#include <fstream>
#include "BitMapImage.h"

using namespace std;

void writeTestFile();

int main() {
    BitMapImage test = BitMapImage();

    test.addColor("ff0000"); test.addColor("00ff00"); test.addColor("0000ff");
    test.addColor("ffff00"); test.addColor("ff00ff"); test.addColor("00ffff");
    test.addColor("ffff00"); test.addColor("ff00ff"); test.addColor("00ffff");
    test.WriteToFile("test5.bmp");
     return 0;
}

void writeTestFile() {
    unsigned char array[70] = { 
        0x42, 0x4D, 0x46, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 
        0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00,   0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    std::ofstream file;
    file.open("test.bmp", std::ios_base::binary);
    if (file.is_open()) {
        for (int i = 0; i < sizeof(array) / sizeof(array[0]); ++i) {
            cout << array + i * sizeof(array[0]) << ' ';
            file.write((char*)(array + i * sizeof(array[0])), sizeof(array[0]));
        }
        file.close();
    }
}