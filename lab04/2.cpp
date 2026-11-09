/**
 * CUSTOM FILE BUFFER READER BY RAIHAN SIR
 */
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdlib>


using namespace std;

void advance(string &prog, int &start, int steps) {
  return;
}

int main(int argc, char *argv[]) {
  FILE *fd = fopen(argv[1], "r");

  if (fd == NULL) {
    cout << "File not found";
    return -1;
  }

  char *buffer = (char*)calloc(1, 1024); // kotobar_alloc, block size
  // char *buffer = (char*)malloc(1024);

  fread(buffer, 1024, 1, fd);
  cout << buffer << '\n';

  free(buffer);
  fclose(fd);
}