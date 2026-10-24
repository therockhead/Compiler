/**
  Easy Task
  @uthor: ASIF
  ID: 2309005
**/

#include <iostream>
#include <string>

using namespace std;

int main() {
    string str;
    int cnt = 0;

    while (cin >> str) {
        if (str == "^Z" || str == "^X") break;
        
        cnt++;
        cout << "Token No " << cnt << " : " << str << '\n';
    } return 0;}

