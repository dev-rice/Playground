#include <cstdarg>
#include <stdarg.h>
#include <stdio.h>

int myPrintf(const char* format, ...) {
    va_list vl;
    va_start(vl, format);
    vprintf(format, vl);
    va_end(vl);
    return 0;
}

int main(){
    myPrintf("%d %s %f", 2, "hi", 3.14);
}
