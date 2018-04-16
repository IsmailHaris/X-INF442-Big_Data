#include <iostream>
 
void swap(int& a, int& b) // je dis : "ne copie pas a et b mais utilise celles qui sont dans main (la ou elles sont appelles)
{
  int s=a;
  a=b;
  b=s;
  /* Version stylee 
  *a=*a+*b;
  *b=*a-*b;
  *a=*a-*b;
  */
}
 
int main(int argc, char **argv)
{
  int a = 0;
  int b = 0;
 
  std::cin >> a;
  std::cin >> b;
 
  swap(a,b);
 
  std::cout << a << " " << b << std::endl;
  return 0;
}