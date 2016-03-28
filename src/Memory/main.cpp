//#include <vld.h>

#include <iostream>

void handlder()
{
  std::cerr << " Error " << std::endl;
}
int main()
{
	std::set_new_handler(handlder);
	int *p = new  int[100000000 ];
	std::cin.get();
	//delete p;
}