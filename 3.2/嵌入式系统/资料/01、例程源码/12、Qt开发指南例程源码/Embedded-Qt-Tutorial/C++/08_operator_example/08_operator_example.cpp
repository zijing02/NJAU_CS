#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	int weight;
	Dog operator+(const Dog &d) {
		Dog dog;
		dog.weight = this->weight + d.weight;
		return dog;
	}

};

int main()
{
	Dog dog1;
	Dog dog2;
	Dog dog3;

	dog1.weight = 10;
	dog2.weight = 20;
	dog3 = dog1 + dog2;
	cout<<"第三只狗的体重是："<<dog3.weight<<endl;
	return 0;
}



