#include <iostream>

using namespace std;

/* 定义一个动物类 */
class Animal
{
public:
    virtual void run() = 0;
};

/* 定义一个狗类，并继承动物类 */
class Dog : public Animal
{
public:
    void run() {
        cout<<"Dog的run()方法"<<endl;
    }

};

/* 定义一个猫类，并继承动物类 */
class Cat : public Animal
{
public:
    void run() {
        cout<<"Cat的run()方法"<<endl;
    }

};

int main()
{
    /* 实例化dog对象 */
    Dog dog;

    /* 实例化cat对象 */
    Cat cat;

	/*	dog调用run()方法 */
	dog.run();

	/* cat调用run()方法 */
	cat.run();

    return 0;
}

