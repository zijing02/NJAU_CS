#include <iostream>
using namespace std;

namespace A
{
	int x = 1;
	void fun() {
		cout<<"A namespace"<<endl;
	}
}
using namespace A;
int main()
{
	fun();
	A::x = 3;
	cout<<A::x<<endl;
	A::fun();
	return 0;
}

