/**
  Moderate Task
  @uthor: ASIF
  ID: 2309005

**/

#include <iostream> 
#include <string> 
#include <vector> 
#include <stack> 
#include <queue> 
#include <sstream> 
#include <cctype>

#define pb push_back

using namespace std;
bool alphanum(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isalnum(c)) return false;
    }
    return true;
}

bool bornomala(string str) {
    for (int i = 0; i < str.size(); i++) {
        if ( (str[i] >= 'a' && str[i] <= 'z') ||
        (str[i] >= 'A' && str[i] <= 'Z') ||
        (str[i] >= '0' && str[i] <= '9')) return true;
        else if (!isalpha(str[i]) || str[i] == '(' || str[i] == ')') {
            return false;
        }
    }
    return true;
}

bool ispunc(char ch) {
    if (ch == '=' || ch == '+' || ch == '-'
    || ch == '/' || ch == '*' ||
    ch == '(' || ch == ')'){
        return true;
    }
    return false;
}

int main()
{
    string str;
    vector<string> v;
    stack<string> st;
    queue<string> tokens;

    while (getline(cin, str)) {
        v.pb(str);
    }

    int i = 0;
    for (auto it: v) {
        i++;
        stringstream ss(it);
        string temp;

        cout << "\"" << it << "\" has ";

        int j = 0;

        while (ss >> temp) {
            if (alphanum(temp)) {
                tokens.push(temp);
            } else {
                string curr = "";
                for (int i = 0; i < temp.size(); i++) {
                    if (ispunc(temp[i])) {
                        if (!curr.empty()) {
                            tokens.push(curr);
                            curr = "";
                        }
                        tokens.push(string(1, temp[i]));
                    }
                    else {
                        curr += temp[i];
                    }
                }
                if (!curr.empty()) {
                    tokens.push(curr);
                }
            }
        }
        cout << tokens.size() << " tokens:";

        while (!tokens.empty()) {
                j++;
                if (tokens.size() > 1) cout << "'" << tokens.front() << "',";
                else cout << " and '" << tokens.front() << "'.";
                tokens.pop();
            }

        cout << '\n';

    }
    return 0;}
