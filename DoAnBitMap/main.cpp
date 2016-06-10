#include "Bitmap.h"

int main(){
	Bitmap a;
	a.readFromFile("hinh.bmp");
	a.printInformation();
	a.adjustBrightness(50);
	
	Bitmap b = a.cutToNewBitmap(100, 100,300,300);
	b.writeToFile("dacat.bmp");

	return 0;
}