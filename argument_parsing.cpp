// Using example from
// http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char **argv) {
    int argument;

    // opterr = 0;
    while ((argument = getopt(argc, argv, "w:fi")) != -1){
        printf("Read command line option:\n");
        printf("  argument = %c\n", argument);
        printf("  optopt   = %c\n", optopt);
        printf("  optarg   = %s\n\n", optarg);

    }
}
