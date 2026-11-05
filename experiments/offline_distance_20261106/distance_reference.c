#include <math.h>
#include <stdio.h>
int main(void) {
 volatile float x=29.600000381469727f,y=34.5f,z=23.100000381469727f;
 volatile float xx=x*x, yy=y*y, zz=z*z;
 volatile float xy=yy+xx, squared=xy+zz;
 float result=sqrtf(squared);
 printf("%.9g %a\n",result,(double)result);
 return 0;
}
