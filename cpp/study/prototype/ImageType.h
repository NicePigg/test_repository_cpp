/*prototype 原型模式  创建未来的class
1.使用场景：由于需求的变化，对象经常剧烈的变化，但他们拥有比较稳定一致的接口clone
2.理论依据：虚函数
3.定义：使用原型实例指定创建对象的种类，然后通过拷贝这些原型来创建新的对象
4.步骤：如何创建易变类的实体对象，（采用原型克隆的方法来做，它使得我们可以非常灵活的动态创建“拥有某些稳定接口”的新对象）所需工作仅仅是注册一个新类的对象（即原型），然后在任何需要的地方Clone（深拷贝）。
5.技法：早绑定->晚绑定
6.变化：需创建新的对象
不变：接口

静态数据成员:
1.初始化在类体外进行，而前面不加static，以免与一般静态变量或对象相混淆。
2.初始化时不加该成员的访问权限控制符private，public等。
3.初始化未赋值。默认为NULL；

多态： 
多态可以通过基类的指针或引用访问派生类的对象，
执行派生类中实现的操作(虚函数)

*/

#ifndef __IMAGETYPE__
#define __IMAGETYPE__

#include <iostream>
using namespace std;
enum imageType
{
	LSAT,SOPT
};

class Image
{
public:
	virtual void draw() = 0;
	static Image* findAndClone(imageType);
protected:
	virtual imageType returnType() = 0;
	virtual Image* clone() = 0;
	static void addPrototype(Image* image)
	{
		__prototypes[__nextSlot++] = image;
	}
private:
	static Image* __prototypes[10]; // 静态变量的声明
	static int __nextSlot;
};
Image* Image::__prototypes[];  // 静态变量类外定义
int Image::__nextSlot;

Image* Image::findAndClone(imageType type)
{
	for (int i = 0; i < __nextSlot; i++)
	{
		if (__prototypes[i]->returnType() == type) //找到原型，就返回克隆副本
			return __prototypes[i]->clone();
	}
}

class LandSatImage :public Image
{
public:
	imageType returnType() {
		return LSAT;
	}
	void draw() {
		cout << "LandSatImage::draw" << __id << endl;
	}
	Image *clone() {
		return new LandSatImage(1);
	}
protected:
	LandSatImage(int dummy) {   // 新增的构造函数 这个dummy没有用到，只是为了区别私有构造函数
		__id = __count++;
	}
private:
	static LandSatImage __LandSatImage; //一个静态的自己
	LandSatImage()				// 私有的构造函数
	{
		addPrototype(this);
	}
	int __id;
	static int __count;
};
LandSatImage LandSatImage::__LandSatImage;
int LandSatImage::__count = 1;

class SpotImage :public Image
{
public:
	imageType returnType() {
		return SOPT;
	}
	void draw() {
		cout << "SpotImage::draw " << _id << endl;
	}
	Image* clone() {
		return new SpotImage(1);
	}
protected:
	SpotImage(int dummy) {
		_id = _count++;
	}
private:
	SpotImage() {
		addPrototype(this);
	}
	static SpotImage _spotImage;
	int _id;
	static int _count;
};
SpotImage SpotImage::_spotImage;
int SpotImage::_count = 1;
#endif
