// ImageType.cpp: 定义应用程序的入口点。
//

#include "ImageType.h"

using namespace std;

int main()
{
	const int nums_images = 8;
	imageType input[nums_images] =
	{
		LSAT, LSAT, LSAT, LSAT, LSAT, LSAT, LSAT, LSAT
	};
	Image* img[nums_images];
	for (int i = 0; i < nums_images; i++)
	{
		img[i] = Image::findAndClone(input[i]);
	}
	for (int i = 0; i < nums_images; i++)
	{
		img[i]->draw();
	}
	for (int i = 0; i < nums_images; i++)
	{
		delete img[i];
	}
	return 0;
}
