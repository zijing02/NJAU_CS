#include <iostream>
#include <string>
using namespace std;

/*动物类，抽象出下面两种属性，
 *颜色和体重，是每种动物都具有的属性
 */
class Animal
{
public:
	/* 颜色成员变量 */
	string color;
	/* 体重成员变量 */
	int weight;
};

/*让狗类继承这个动物类，并在狗类里写自己的属性。
 *狗类拥有自己的属性name，age，run()方法，同时也继承了
 *动物类的color和weight的属性
 */
class Dog : public Animal
{
public:
	string name;
	int age;
	void run();
};

int main()
{
	Dog dog;
	dog.name = "旺财";
	dog.age = 2;
	dog.color = "黑色";
	dog.weight = 120;
	cout<<"狗的名字叫："<<dog.name<<endl;
	cout<<"狗的年龄是："<<dog.age<<endl;
	cout<<"狗的毛发颜色是："<<dog.color<<endl;
	cout<<"狗的体重是："<<dog.weight<<endl;
    return 0;
}

