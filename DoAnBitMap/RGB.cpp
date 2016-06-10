#include "RGB.h"
#include <fstream>
#include <iostream>
#define swap(x,y) {int (temp)=(x); (x)=(y);(y)=(temp);}

RGB::RGB(const RGB& rgb){
	this->m_color.blue = rgb.m_color.blue;
	this->m_color.green = rgb.m_color.green;
	this->m_color.red = rgb.m_color.red;
}

RGB& RGB::operator = (const RGB& rgb){
	this->m_color.blue = rgb.m_color.blue;
	this->m_color.green = rgb.m_color.green;
	this->m_color.red = rgb.m_color.red;
	return *this;
}

RGB::RGB(unsigned char red, unsigned char green, unsigned char blue){
	this->m_color.blue = blue;
	this->m_color.red = red;
	this->m_color.green = green;
}

RGB::RGB(){

}

RGB::~RGB(){

}

Color RGB::readColor(){
	return this->m_color;
}

RGB& RGB::changeColor(unsigned char red, unsigned char green, unsigned char blue){
	this->m_color.red = red;
	this->m_color.green = green;
	this->m_color.blue = blue;

	return *this;
}

RGB& RGB::changeBrightness(int value){
	int newR = this->m_color.red + value;
	int newG = this->m_color.green + value;
	int newB = this->m_color.blue + value;
	if (newR > 255) newR = 255;
	if (newR < 0)	newR = 0;
	if (newG > 255) newG = 255;
	if (newG < 0)	newG = 0;
	if (newB > 255) newB = 255;
	if (newB < 0)	newB = 0;

	this->m_color.red = newR;
	this->m_color.green = newG;
	this->m_color.blue = newB;
	return *this;
}

void RGB::printInformation(){
	cout << "R:" << (int)this->m_color.red << " G:" << (int) this->m_color.green << " B:" << (int)this->m_color.blue << endl;
}

RGB& RGB::changeToGrayScale(){
	int n = .299*this->m_color.red + .587*this->m_color.green + .114*this->m_color.blue;
	this->changeColor(n, n, n);
	return *this;
}

RGB& RGB::changeToInversion(){
	this->changeColor(255 - this->m_color.red, 255 - this->m_color.green, 255 - this->m_color.blue);
	return *this;
}


void changeToGrayScale(RGB& r){
	Color a = r.readColor();
	int n = .299*a.red + .587*a.green + .114*a.blue;
	r.changeColor(n, n, n);
}

void changeToInversion(RGB& r){
	Color a = r.readColor();
	r. changeColor(255 - a.red, 255 - a.green, 255 - a.blue);
}
