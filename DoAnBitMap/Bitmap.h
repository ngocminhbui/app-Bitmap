#pragma pack(1)
#include <stdint.h>
#include <string>
#include "RGB.h"
using namespace std;
	

struct BmpSignature{
	unsigned char data[2];
};

struct BmpHeader{
	BmpSignature signature;
	uint32_t fileSize;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t dataOffset;
};

struct BmpDib{
	uint32_t dibSize;
	int32_t	 imageWidth;
	int32_t  imageHeight;
	uint16_t colorPlaneCount;
	uint16_t pixelSize;
	uint32_t compressMethod;
	uint32_t bitmapByteCount;
	int32_t  horizontalResolution;
	int32_t  verticalResolution;
	uint32_t colorCount;
	uint32_t importantColorCount;
};

struct ColorTable{
	RGB	 *colors;
	uint32_t length;
};

struct PixelArray{
	RGB	 **pixels;
	uint32_t rowCount;
	uint32_t columnCount;
};


class Bitmap{
private:

	BmpHeader m_header;
	BmpDib m_dib;
	ColorTable m_colorTable;
	PixelArray m_pixelArray;

	bool isBMP();
	void readBmpHeader(ifstream &inStream);
	void readBmpDib(ifstream &inStream);
	void readBmpPixelArray(ifstream &inStream);
	void writeBmpHeader(ofstream &outStream);
	void writeBmpDib(ofstream &outStream);
	void writeBmpPixelArray(ofstream &outStream);
public:
	Bitmap();
	Bitmap(const Bitmap& p);
	Bitmap(string filename);
	Bitmap& operator=(const Bitmap& p);
	~Bitmap();

	void readFromFile(string filename);
	void writeToFile(string filename);
	void printInformation();
	void drawToScreen();

	void printAPixelInformation(int row, int column);
	void changeAPixel(int row, int column, unsigned char red, unsigned char green, unsigned char blue);
	void adjustBrightness(int value);

	void doToAllPixels(void (*function_pointer)(RGB&));

	Bitmap& cutToNewBitmap(int x0, int y0, int x1, int y1);
	

};


