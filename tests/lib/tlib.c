
#include <stdio.h>

void hello(void);

int main(void)
{
  printf("%s\n", "main routine calling routine in library\n");
  hello();
  return;
};