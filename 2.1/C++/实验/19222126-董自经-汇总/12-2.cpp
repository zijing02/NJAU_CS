12-2-1
#include <iostream>
#include <string>
using namespace std;
class A {
	int i;
protected:
	char c;
public:
	string s;
	A(int tempi, char tempc, string temps) : i(tempi) { c = tempc; s = temps; }
	void show() { cout << "A::i=" << i << " A::c=" << c << " A::s=" << s << endl; }
};
class B : public A {
public:
	B(int tempi, char tempc, string temps) :A(tempi, tempc, temps) {}
};
class C : public A {
public:
	C(int tempi, char tempc, string temps) : A(tempi, tempc, temps) {}
};
class D : public B, public C {
public:
	D(int tempi1, char tempc1, string temps1, int tempi2, char tempc2, string temps2) : B(tempi1, tempc1, temps1), C(tempi2, tempc2, temps2) {}
};
void main() {
	D d(98, 'b', "bs", 99, 'c', "cs");
	//d.show();
	d.B::show();
	d.C::show();
	cout << "使用作用域规则解决路径二义性时，派生类对象所占空间大小为：" << sizeof(d) << endl;
}

12-2-2
#include <iostream>
#include <string>
using namespace std;
class A {
	int i;
protected:
	char c;
public:
	string s;
	A(int tempi, char tempc, string temps) : i(tempi) { c = tempc; s = temps; }
	void show() { cout << "A::i=" << i << " A::c=" << c << " A::s=" << s << endl; }
};
class B :virtual public A {
public:
	B(int tempi, char tempc, string temps) :A(tempi, tempc, temps) {}
};
class C :virtual public A {
public:
	C(int tempi, char tempc, string temps) :A(tempi, tempc, temps) {}
};
class D :public B, public C {
public:
	D(int tempi1, char tempc1, string temps1, int tempi2, char tempc2, string temps2) : A(tempi1, tempc1, temps1), B(tempi1, tempc1, temps1), C(tempi2, tempc2, temps2) {}
};
void main() {
	D d(98, 'b', "bs", 99, 'c', "cs");
	d.B::show();
	d.C::show();
	d.show();
	cout << "使用虚继承方式解决路径二义性时，派生类对象所占空间大小为：" << sizeof(d) << endl;
}
12-3
#include <iostream>
using namespace std;
class Figure {
	int sidenum;		//边数
public:
	Figure() { cout << "Figure." << endl; }
	Figure(int temp) :sidenum(temp) { cout << "The figure has " << sidenum << " sides." << endl; }
};
class Quadrangle1 : virtual public Figure {				//对角线垂直
public:
	Quadrangle1() { cout << "Quadrangle1." << endl; }
	Quadrangle1(int t1) :Figure(t1) { cout << "This is a quarilateral with two diagonal lines perpendicular to each other" << endl; }
};
class Parallelogram : virtual public Figure {			//平行四边形
public:
	Parallelogram() { cout << "Parallelogram." << endl; }
	Parallelogram(int t1) :Figure(t1) { cout << "This is a Parallelorgram" << endl; }
};
class Quadrangle2 : virtual public Figure {				//四个直角
public:
	Quadrangle2() { cout << "Quadrangle2." << endl; }
	Quadrangle2(int t1) :Figure(t1) { cout << "This is a quarilateral with two four 90 degree interior angles" << endl; }
};

class Rhombus : virtual public Quadrangle1, virtual public Parallelogram {		//菱形
protected:
	int sidelength;
public:
	Rhombus() { cout << "Rhombus." << endl; }
	Rhombus(int t1, int t2) :Quadrangle1(t1), Parallelogram(t1), sidelength(t2) { cout << "This is a rhombus." << endl; }
};
class Rectangle : virtual public Quadrangle2, virtual public Parallelogram {	//矩形
protected:
	int angle;
public:
	Rectangle() { cout << "Rectangle." << endl; }
	Rectangle(int t1, int t3) :Quadrangle2(t1), Parallelogram(t1), angle(t3) { cout << "This is a rectangle" << endl; }
};

class Square : public Rhombus, public Rectangle {				//正方形
public:
	Square() { cout << "Square." << endl; }
	Square(int t1, int t2, int t3) :Figure(t1),Quadrangle1(t1),Parallelogram(t1),Quadrangle2(t1), Rhombus(t1, t2), Rectangle(t1, t3) {
		cout << "The figure is a square!" << endl << "All angles of the square are " << angle << " degrees." <<
			endl << "The sidelength of the square is " << sidelength << "." << endl << "The area of the square is " << sidelength * sidelength << endl;
	}
};
void main() { Square s(4, 10, 90); }//边数，边长，角度
