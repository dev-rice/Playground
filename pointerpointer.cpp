#include <stdio.h>
#include <random>
#include <vector>

unsigned long size = 50000000;

struct Container {
    unsigned char* data;
};

void random_data(Container& conatainer){
    for (unsigned long i = 0; i < size; ++i){
        unsigned char a = conatainer.data[rand()%size];
    }
}

int main(){
    struct Container container;
    container.data = new unsigned char[size];
    random_data(container);
    return 0;
}
