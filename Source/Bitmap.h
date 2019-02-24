////////////////////////////////////////
// Bitmap.h
////////////////////////////////////////

#pragma once
#include <iostream>

////////////////////////////////////////////////////////////////////////////////

class Bitmap {
public:
    Bitmap();
	Bitmap(int x,int y)						{XRes=x; YRes=y; Pixel=new int[XRes*YRes];}
	~Bitmap()								{if (Pixel!=nullptr) delete []Pixel;}

	bool SaveBMP(const char *filename);

	int GetXRes() const						{return XRes;}
	int GetYRes() const						{return YRes;}
	int &GetPixel(int x,int y)				{return Pixel[y*XRes+x];}
	void SetPixel(int x,int y,int pix)		{Pixel[y*XRes+x]=pix;
//        int col = Pixel[y*XRes+x];
//        std::cout << "(r,g,b): (" << int((col>>16)&0xff) << ", " << int((col>>8)&0xff) << ", " << int(col&0xff) << ")" << std::endl;
    }

private:
	int XRes;
	int YRes;
	int *Pixel;
};

////////////////////////////////////////////////////////////////////////////////
