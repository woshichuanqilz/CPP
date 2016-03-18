#include <stdio.h>

int main()
{

   unsigned int i,j;

   i=35;
   //为什么下面两个左移操作结果不一样？

   j=1<<i;  // j为8

   j=1<<35; // j为0

   printf("i = %d, j = %d \n", i, j);
   getchar();
   return ;
}
