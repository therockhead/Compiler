#include <iostream>
#include <string>
#include <fstream>


using namespace std;

void advance(string &prog, int &start, int steps=1) {
  start+=steps;
  return;
}

char peek(string &prog, int &start) {
  return prog[start + 1];
}

int main(int argc, char *argv[]) {
  cout << argc << '\n';
  for (int i = 0; i < argc; i++) {
    cout << argv[i] << '\n';
  }
  
  /*
    PLUS     =: '+'
MINUS    =: '-'
STAR     =: '*'
SLASH    =: '/'
PERCENT  =: '%'
CARET    =: '^'
LPAREN   =: '('
RPAREN   =: ')'
  
  */
  fstream inf(argv[1]);
  string prog;
  while (getline(inf, prog)) {
    // cout << prog << '\n';
    int start = 0;
    switch(prog[start]) {
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case '^':
      case '(':
      case ')':
        advance(prog, start);

    }
  }


}