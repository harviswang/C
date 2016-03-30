/*
 * 测试c++中虚函数/纯虚函数的用法
 */
#include <iostream>
//namespace VirtualFunction {
class Shape {
public:
	Shape(int id, int area);
	virtual ~Shape(); /* 虚函数 */
	//virtual ~Shape() = 0; /* 纯虚函数 */
	//~Shape(){ } /* 空的析构函数 */
	
	virtual void setId(int id) = 0; /* 纯虚函数 */
	virtual int getId() = 0;
	virtual void setArea(int area) = 0;
	virtual int getArea() = 0;
protected:
	int id;
	int area;
};

class Square : public Shape {
public:
	Square(int id, int area) : Shape(id, area){ }
	virtual ~Square();
	
	void setId(int id);
	int getId();
	void setArea(int area);
	int getArea();
};

Shape::Shape(int id, int area)
{
	this->id = id;
	this->area = area;
}

Shape::~Shape()
{
	std::cout << __func__ << ":" << __LINE__ << std::endl;
}

void Square::setId(int id)
{
	this->id = id;
}

int Square::getId()
{
	return this->id;
}

void Square::setArea(int area)
{
	this->area = area;
}

int Square::getArea()
{
	return this->area;
}

/*
 * Square对象在结束生命周期时候,会先调用~Square(),后调用Square的父类的析构函数~Shape()
 */
Square::~Square()
{
	std::cout << __func__ << ":" << __LINE__ << std::endl;
}

int main()
{
	//Shape sh(10, 15); /* 抽象类不能实例化 */
	Square sq(10, 20);
	using namespace std;
	
	sq.setId(88);
	sq.setArea(99);
	std::cout << sq.getId() << "\n";
	std::cout << sq.getArea() << "\n";
	return 0;
}