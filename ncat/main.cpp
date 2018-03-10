//#include <stdio.h>
#include <iostream>
#include <windows.h>	//yuanyuan for OutputDebugString()

using namespace std;

#include "Game1.h"

int main(void)
{
	//printf("Hello, world!\n");
	//cout << "Hello" << "world, C++!" << endl;
	OutputDebugString("***HelloDebugString()***\n");

	Game1 *gm = new Game1();

	LL list2(10,6);
	list2.push_back(5);

	LL::iterator ii;
	for(ii=list2.begin();ii!=list2.end();ii++) {
		cout << *ii << endl;
	}

	delete gm;

	return 0;
}
