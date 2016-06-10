#include "Bitmap.h"
#include "Windows.h"
void Bitmap::drawToScreen(){
	if (!this->isBMP())
		return;
	HWND console = GetConsoleWindow();
	HDC hdc = GetDC(console);

	for (int i = 0; i < this->m_dib.imageHeight; i++)
		for (int j = 0; j < this->m_dib.imageWidth; j++){
			Color a = this->m_pixelArray.pixels[i][j].readColor();
			SetPixel(hdc, 500+ j, i, RGB(a.red, a.green, a.blue));
		}
	ReleaseDC(console, hdc);
}