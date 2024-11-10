#include <stdio.h>

int main(){
           float l, b;
           printf("Input the length of your rectangle: ");
           scanf("%f", &l);
           printf("\nInput the breadth of your rectangle: ");
           scanf("%f", &b);
           printf("\n\nThe perimeter is: %f", 2*(l+b));
           printf("\nThe area is: %f", l*b);
           
           return 0;
           }
