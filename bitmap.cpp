//bitmap.cpp
//By Trey Reynolds
#include "stdafx.h"//Windows onleh kk?
#include <fstream>

using namespace std;

//bitmap class by Trey Reynolds
//bitmap(width, height)
class bitmap{
private:
	//Width and height of the image
	int w,h;
	//Channels for red, green, and blue
	float* rchan;
	float* gchan;
	float* bchan;
public:
	//Makes a new bitmap object of width neww and height newh
	bitmap(int neww,int newh);

	//Frees the memory when deleting the bitmap
	~bitmap();

	//Sets the pixel at (x, y) to color (newr, newg, newb)
	bool setpixel(int x,int y,float newr,float newg,float newb);

	//Gets the pixel at (x, y) and sets the reference values to the respective channel color
	bool getpixel(int x,int y,float &retr,float &retg,float &retb);

	//Saves to a file of an arbitrary name
	void save(const char* name);

	//Saves to the file "out.bmp"
	void save();
};

bitmap::bitmap(int neww,int newh){
	w=neww;
	h=newh;
	rchan=new float[neww*newh];
	gchan=new float[neww*newh];
	bchan=new float[neww*newh];
}

bitmap::~bitmap(){
	delete[] rchan;//Free memory
	delete[] gchan;
	delete[] bchan;
}

bool bitmap::setpixel(int x,int y,float newr,float newg,float newb){
	if(0<=x&x<w&0<=y&y<h){//Test to see that the pixel is within the image
		rchan[y*w+x]=newr;//If so, set the pixel color
		gchan[y*w+x]=newg;
		bchan[y*w+x]=newb;
		return true;//Return true that it did set the color
	}else{
		return false;//Otherwise, return false because it did not set a color.
	}
}

bool bitmap::getpixel(int x,int y,float &retr,float &retg,float &retb){
	if(0<=x&x<w&0<=y&y<h){//Test to see that the pixel is within the image
		retr=rchan[y*w+x];
		retg=gchan[y*w+x];
		retb=bchan[y*w+x];
		return true;
	}else{
		retr=0;
		retg=0;
		retb=0;
		return false;
	}
}

/*bitmap
Takes name and bitmap

This is how ms paint writes bitmap files.
I saved about 16 different small bitmap files and figured out what everything meant
by reverse engineering
This assumes base 10 escape sequences. Not base 8.
"BM"									//Bitmap Header
..writeint(54+bytes)					//Total file size.
.."\0\0\0\0\54\0\0\0\40\0\0\0"			//40 defines color definition size
..writeint(w)..writeint(h)				//Width by height
.."\1\0\24\0\0\0\0\0"					//Defines 24 bit color
..writeint(bytes)						//Total pixel byte length
.."\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"	//Space that we don't use
*/
void bitmap::save(const char * name){
	char color[3];							//For colors lelelel
	fstream bmp(name,ios::out|ios::binary);
	int n=1;
	int excess=w%4;							//Should be -3*w%4, but CPU doesn't calculate modulus correctly
											//Fortunately, it simplifies down to w%4    :)
	int bytes=h*(3*w+excess);				//The size of the color data section
	int size=bytes+54;						//The total size of the file
	char* zero="\0\0";						//For buffer use
	bmp.write("BM",2);
	bmp.write((char*)&size,4);
	bmp.write("\0\0\0\0\66\0\0\0\50\0\0\0",12);
	bmp.write((char*)&w,4);
	bmp.write((char*)&h,4);
	bmp.write("\1\0\30\0\0\0\0\0",8);
	bmp.write((char*)&bytes,4);
	bmp.write("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0",16);
	for(int i=0;i<h;++i){
		for(int j=0;j<w;++j){
			//Convert float values from 0 to 1 into bytes from 0 to 255 :)
			float b=255*bchan[i*w+j];
			float g=255*gchan[i*w+j];
			float r=255*rchan[i*w+j];
			color[0]=(b!=b|b<0)?0:b<256?char(b):255;//Make sure that it's within 0 to 255.
			color[1]=(g!=g|g<0)?0:g<256?char(g):255;
			color[2]=(r!=r|r<0)?0:r<256?char(r):255;
			bmp.write(color,3);//write the three bytes to the file.
		}
		bmp.write(zero,excess);
	}
	bmp.close();
}

void bitmap::save(){
	save("out.bmp");
}
