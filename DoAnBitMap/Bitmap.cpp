#include "Bitmap.h"
#include <fstream>
#include <iostream>
#define swap(x,y) {int (temp)=(x); (x)=(y);(y)=(temp);}


Bitmap::Bitmap(){
	memset(this, 0, sizeof(Bitmap));
}

Bitmap::Bitmap(const Bitmap& p){
	this->m_header = p.m_header;
	
	this->m_dib = p.m_dib;
	
	this->m_colorTable = p.m_colorTable;
	this->m_colorTable.colors = new RGB[this->m_colorTable.length];
	memcpy(this->m_colorTable.colors, p.m_colorTable.colors, this->m_colorTable.length * sizeof(RGB));

	this->m_pixelArray = p.m_pixelArray;
	this->m_pixelArray.pixels = new RGB* [this->m_pixelArray.rowCount];
	for (int i = 0; i < this->m_pixelArray.rowCount; i++){
		this->m_pixelArray.pixels[i] = new RGB[this->m_pixelArray.columnCount];
		memcpy(this->m_pixelArray.pixels[i], p.m_pixelArray.pixels[i], sizeof(RGB)*this->m_pixelArray.columnCount);
	}
}

Bitmap::Bitmap(string filename){
	ifstream inStream(filename, ios::binary);
	this->readBmpHeader(inStream);
	this->readBmpDib(inStream);
	this->readBmpPixelArray(inStream);
	inStream.close();
}

bool Bitmap::isBMP(){
	if (this->m_header.signature.data[0] == 'B' && this->m_header.signature.data[1] == 'M')
		return true;
	return false;
}
Bitmap& Bitmap::operator = (const Bitmap& p){
	this->m_header = p.m_header;

	this->m_dib = p.m_dib;

	this->m_colorTable = p.m_colorTable;
	this->m_colorTable.colors = new RGB[this->m_colorTable.length];
	memcpy(this->m_colorTable.colors, p.m_colorTable.colors, this->m_colorTable.length * sizeof(RGB));

	this->m_pixelArray = p.m_pixelArray;
	this->m_pixelArray.pixels = new RGB*[this->m_pixelArray.rowCount];
	for (int i = 0; i < this->m_pixelArray.rowCount; i++){
		this->m_pixelArray.pixels[i] = new RGB[this->m_pixelArray.columnCount];
		memcpy(this->m_pixelArray.pixels[i], p.m_pixelArray.pixels[i], sizeof(RGB)*this->m_pixelArray.columnCount);
	}

	return *this;
}

Bitmap::~Bitmap(){
	delete[] this->m_colorTable.colors;
	
	for (int i = 0; i < this->m_pixelArray.rowCount; i++)
		delete[] this->m_pixelArray.pixels[i];
	delete[] this->m_pixelArray.pixels;
}

void Bitmap::readBmpHeader(ifstream &inStream){
	if (!inStream.good())
		return;
	inStream.seekg(0);
	inStream.read(reinterpret_cast<char *>(&this->m_header), sizeof(BmpHeader));
}
void Bitmap::readBmpDib(ifstream &inStream){
	if (!inStream.good())
		return;
	inStream.seekg(sizeof(BmpHeader));
	inStream.read(reinterpret_cast<char *>(&this->m_dib), sizeof(this->m_dib));
}

void Bitmap::readBmpPixelArray(ifstream &inStream){
	if (!inStream.good())
		return;
	inStream.seekg(this->m_header.dataOffset);

	this->m_pixelArray.rowCount = this->m_dib.imageHeight;
	this->m_pixelArray.columnCount = this->m_dib.imageWidth;
	this->m_pixelArray.pixels = new RGB *[this->m_pixelArray.rowCount];

	int paddingCount = (4 - (this->m_dib.imageWidth * (this->m_dib.pixelSize / 8) % 4)) % 4;
	char padding[3];
	int nRow = this->m_pixelArray.rowCount;
	int nColumn = this->m_pixelArray.columnCount;

	for (int i = 0; i < nRow; i++){
		this->m_pixelArray.pixels[nRow - 1 - i] = new RGB [nColumn];
		inStream.read(reinterpret_cast<char *> (this->m_pixelArray.pixels[nRow-1-i]), nColumn*sizeof(RGB));
		inStream.read(padding, paddingCount);
	}
}

void Bitmap::readFromFile(string filename){
	ifstream inStream(filename, ios::binary);
	char key[3];
	key[2] = '\0';
	inStream.read(key, 2);
	if (strcmp(key, "BM") != 0){
		cout << "error while trying to read" << endl;
		inStream.close();
		return;
	}

	this->readBmpHeader(inStream);
	this->readBmpDib(inStream);
	this->readBmpPixelArray(inStream);

	inStream.close();
}

void Bitmap::printInformation(){
	if (!isBMP()){
		cout << "not a bmp or damaged bmp" << endl;
		return;
	}
	cout<<"*** BMP Header ***\n";
	cout << "- File Size  : " << this->m_header.fileSize << " byte(s)\n";
	cout << "- Reserved1  :" << this->m_header.reserved1 << "\n";
	cout << "- Reserved2  : " << this->m_header.reserved2 << "\n";
	cout<<"- Data Offset: "<<this->m_header.dataOffset<< "byte(s)\n";

	cout<<"*** BMP Dib ***\n";
	cout << "- DIB Size               : " << this->m_dib.dibSize << " byte(s)\n";
	cout << "- Image Width            : " << this->m_dib.imageWidth << "\n";
	cout << "- Image Height           : " << this->m_dib.imageHeight << "\n";
	cout << "- Number of Color Planes : " << this->m_dib.colorPlaneCount << "\n";
	cout << "- Pixel Size             : " << this->m_dib.pixelSize << " bit(s)\n";
	cout << "- Compress Method        : " << this->m_dib.compressMethod << "\n";
	cout << "- Bitmap Size            : " << this->m_dib.bitmapByteCount << " byte(s)\n";
	cout << "- Horizontal Resolution  : " << this->m_dib.horizontalResolution << "\n";
	cout << "- Vertical Resolution    : " << this->m_dib.verticalResolution << "\n";
	cout << "- Number of Colors       : " << this->m_dib.colorCount << "\n";
	cout << "- Number of Impt Colors  : " << this->m_dib.importantColorCount << "\n";
}

void Bitmap::writeBmpHeader(ofstream &outStream){
	outStream.write(reinterpret_cast<char *> (&this->m_header), sizeof(BmpHeader));
}
void Bitmap::writeBmpDib(ofstream &outStream){
	outStream.write(reinterpret_cast<char*> (&this->m_dib), sizeof(BmpDib));
}
void Bitmap::writeBmpPixelArray(ofstream &outStream) {
	int nRow = this->m_pixelArray.rowCount;
	int nColumn = this->m_pixelArray.columnCount;
	for (int i = 0; i < nRow; i++){
		outStream.write( reinterpret_cast<char *> (this->m_pixelArray.pixels[nRow - 1 - i]), sizeof(RGB)*nColumn);
		char paddingCount = (4 - (this->m_dib.imageWidth * (this->m_dib.pixelSize / 8) % 4)) % 4;

		char *addpadding = new char[paddingCount]();
		outStream.write(addpadding, sizeof(char)*paddingCount);
		delete []addpadding;
	}
}

void Bitmap::writeToFile(string filename){
	if (!isBMP())
		return;
	ofstream outStream(filename, ios::binary);
	if (outStream.good()){
		this->writeBmpHeader(outStream);
		this->writeBmpDib(outStream);
		this->writeBmpPixelArray(outStream);
		
		outStream.close();
	}
	else{
		cout << "error while trying to write" << endl;
	}
}

void Bitmap::printAPixelInformation(int row, int column){
	if (row < 0 || row >= m_pixelArray.rowCount || column<0 || column >=m_pixelArray.columnCount)
		return;
	cout << row  << "x" << column << " ";
	this->m_pixelArray.pixels[this->m_pixelArray.rowCount - 1 - row][column].printInformation();
}

void Bitmap::changeAPixel(int row, int column, unsigned char red, unsigned char green, unsigned char blue){
	if (row < 0 || row >= m_pixelArray.rowCount || column<0 || column >= m_pixelArray.columnCount)
		return;
	this->m_pixelArray.pixels[this->m_pixelArray.rowCount - 1 - row][column].changeColor(red,green,blue);
	
}

void Bitmap::adjustBrightness(int value){
	int nRow = this->m_pixelArray.rowCount;
	int nColumn = this->m_pixelArray.columnCount;

	for (int i = 0; i < nRow; i++)
		for (int j = 0; j < nColumn; j++)
			this->m_pixelArray.pixels[i][j].changeBrightness(value);

}

Bitmap& Bitmap::cutToNewBitmap(int x0, int y0, int x1, int y1){
	Bitmap *newPic = new Bitmap;

	if (x0 < 0 || x0 >= m_pixelArray.rowCount || y0 < 0 || y0 >= m_pixelArray.columnCount)
		return *newPic;
	if (x1 < 0 || x1 >= m_pixelArray.rowCount || y1 <0 || y1 >= m_pixelArray.columnCount)
	return *newPic;
	newPic->m_header = this->m_header;
	newPic->m_dib = this->m_dib;

	if (x0 > x1) swap(x0, x1);
	if (y0 > y1) swap(y0, y1);

	int nRow = x1 - x0 +1;
	int nColumn = y1 - y0 +1;

	int paddingCount = (4 - (nRow * (newPic->m_dib.pixelSize / 8) % 4)) % 4;

	newPic->m_header.fileSize = this->m_header.dataOffset + nRow*nColumn*sizeof(RGB) + paddingCount*nRow;
	newPic->m_dib.imageHeight = newPic->m_pixelArray.rowCount = nRow;
	newPic->m_dib.imageWidth = newPic->m_pixelArray.columnCount = nColumn;
	newPic->m_dib.bitmapByteCount = nRow*nColumn*sizeof(RGB) + paddingCount*nRow;

	x0 = this->m_dib.imageHeight - 1 - x0;
	x1 = this->m_dib.imageHeight - 1 - x1;
	int readingRow = x1;

	newPic->m_pixelArray.pixels = new RGB* [nRow];

	for (int i = 0; i < nRow; i++){
		newPic->m_pixelArray.pixels[i] = new RGB[nColumn];
		memcpy(newPic->m_pixelArray.pixels[i], &this->m_pixelArray.pixels[readingRow][y0], nColumn*sizeof(RGB));
		readingRow++;
	}
	return *newPic;
}



void Bitmap::doToAllPixels(void(*function_pointer)(RGB&)){
	int nRow = this->m_pixelArray.rowCount;
	int nColumn = this->m_pixelArray.columnCount;

	for (int i = 0; i < nRow; i++)
		for (int j = 0; j < nColumn; j++)
			function_pointer(this->m_pixelArray.pixels[i][j]);
}
