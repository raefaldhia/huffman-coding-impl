#ifndef UTIL_H
#define UTIL_H

int max(int first, int second);
void clearstdin();

#ifdef __linux__
// there's no such function on linux
void getch();
#endif

#endif // UTIL_H