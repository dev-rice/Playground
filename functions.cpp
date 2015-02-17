#include <functional>
#include <stdio.h>

void printnum(int a){
    printf("Number: %d\n", a);
}

int main(){
    std::function<void(...)> a;

    return 0;
}
