#pragma pack(1)
#include <stdint.h>
#include <string>
using namespace std;



struct Color{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

class RGB{
private:
	Color m_color;
public:
	RGB();
	RGB(const RGB& rgb);
	RGB(unsigned char red, unsigned char green, unsigned char blue);
	RGB& operator=(const RGB& rgb);
	~RGB();

	Color readColor();
	RGB& changeColor(unsigned char red, unsigned char green, unsigned char blue);
	RGB& changeBrightness(int value);
	RGB& changeToGrayScale();
	RGB& changeToInversion();

	void printInformation();
};

void changeToGrayScale(RGB& r);
void changeToInversion(RGB& r);