#include <stdint.h>

unsigned add_two(unsigned a, unsigned b)
{
  return a + b;
}

int main()
{
  unsigned a = 1;
  unsigned b = 2;
  return add_two(a, b);
}