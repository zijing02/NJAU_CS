#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	string name;
	void getWeight(int weight) {
		cout<<name<<"的体重是："<<weight<<"kG"<<endl;
	}

	void getWeight(double weight) {
		cout<<name<<"的体重是："<<weight<<"kG"<<endl;
	}
};

int main()
{
	Dog dog;
	dog.name = "旺财";
	dog.getWeight(10);
	dog.getWeight("00");
    return 0;
}



