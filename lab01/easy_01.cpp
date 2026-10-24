#include <bits/stdc++.h>
using namespace std;

#define pb push_back

int main() {
    vector<string> ins;
    string str;
    while (cin >> str) {
        ins.pb(str);
    }
    for (auto it: ins) {
        cout << it << '\n';
    }

}