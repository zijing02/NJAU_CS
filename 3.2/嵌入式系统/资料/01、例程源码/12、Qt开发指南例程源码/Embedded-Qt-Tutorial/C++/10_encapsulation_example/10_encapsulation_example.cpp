#include <iostream>
#include <string>
using namespace std;

class Dog
{
public:
	string name;
	Dog(int i = 0)
	{
		total = i;
	}
	void addFood(int number) {
		total = total + number;
	}
	int getFood() {
		return total;
	}
private:
	int total;
};


int main()
{
	Dog dog;
	dog.name = "旺财";
	dog.addFood(3);
	dog.addFood(2);
	cout<<dog.name<<"总共获得了"<<dog.getFood()<<"份食物"<<endl;
    return 0;
}

