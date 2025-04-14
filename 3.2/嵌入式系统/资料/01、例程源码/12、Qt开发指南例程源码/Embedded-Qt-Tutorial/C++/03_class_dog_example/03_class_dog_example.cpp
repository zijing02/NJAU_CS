#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	string name;
	int age;

	void run() {
		cout<<"小狗的名字是:"<<name<<","<<"年龄是"<<age<<endl;
	}
};

int main()
{
	Dog dog1;

	dog1.name = "旺财";
	dog1.age = 2;
	dog1.run();

	Dog *dog2 = new Dog();

	if (NULL == dog2) {
		return 0;
	}
	dog2->name = "富贵";
	dog2->age = 1;
	dog2->run();


	delete dog2;
	dog2 = NULL;
    return 0;
}

