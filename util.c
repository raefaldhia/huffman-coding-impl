#include "util.h"

#include <stdio.h>

int max(int first, int second) {
    return (first > second) ? (first) : (second);
}

void clearstdin() {
    /* taken from https://stackoverflow.com/a/7898516  */
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

#ifdef __linux__
void getch() {}
#endif