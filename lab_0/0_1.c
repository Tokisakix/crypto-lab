#include <stdio.h>

int main(){
    long a, b, c;
    scanf("%ld%ld%ld", &a, &b, &c);
    printf("%ld\n%ld\n", (a + b) % c, (a * b) % c);
    return 0;
}