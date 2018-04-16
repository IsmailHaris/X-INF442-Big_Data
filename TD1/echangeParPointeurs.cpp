#include <iostream>
 
void swap(int* a, int* b) // je passe des pointeurs en arg, c est donc la version par pointeurs
{
  int s=*a;
  *a=*b;
  *b=s;
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
 
  swap(&a,&b);
 
  std::cout << a << " " << b << std::endl;
  return 0;
}