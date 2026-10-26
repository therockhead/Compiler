#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

typedef struct invalid_char
{
    int row;
    int col;
    string ch;
    string name;
} inv;

map<string, string> op = {
    {"::", "SCOPE"},
    {":", "COLON"},
    {"->", "ARROW"},
    {".", "DOT"},
    {"..", "RANGE"},
    {"...", "ELLIPSIS"}
};

void skipws(string &prog, int &start)
{
    while (start < prog.length() && isspace(prog[start]))
    {
        ++start;
    }
}

string advance(string &prog, int &start, int count = 1)
{
    int pos = start;
    start += count;
    return prog.substr(pos, count);
}

char peek(string &prog, int &start)
{
    return prog[start + 1];
}

string readString(string &prog, int &start)
{
    string str = "";
    if (prog[start] != '"')
    {
        cerr << "Error: string does not start with '\"'\n";
        return "";
    }
    advance(prog, start); // read the first "

    while (start < prog.length() && prog[start] != '"')
    {
        str += advance(prog, start);
    }

    if (start >= prog.length())
    {
        cerr << "Error: unterminated string\n";
        return "";
    }

    advance(prog, start); /// read ending "
    return str;
}

char readChar(string &prog, int &start)
{
    char ch = prog[start + 1];
    advance(prog, start, 3); 
    return ch;
}
int main(int argc, char *argv[])
{
    fstream inf(argv[1], ios::in);
    if (!inf.is_open())
    {
        return 0;
    }

    string prog;
    string token_name;

    int start = 0;
    string token = "";

    vector<inv> inv_char;

    int row = 0;
    int col = 1;
    while (getline(inf, prog))
    {
        token_name = "";
        start = 0;
        col = 1;

        row++;
        while (start < prog.length())
        {
            skipws(prog, start);

            switch (prog[start])
            {
            case '"':
                col = start + 1;
                token = readString(prog, start);
                token_name="STRING";
                break;
            case '\'':
                col = start + 1;
                if (peek(prog, start) == 'n') {
                    token += advance(prog, start, 2);
                }
                else {
                    token = string(1,readChar(prog, start));
                }
                token_name = "CHAR";
                break;
            case '@':
                token+=advance(prog, start);
                while (start < prog.length() && prog[start]!='@' 
                        && !isspace(prog[start])) {
                    token+= advance(prog, start);
                }
                token_name = "ANNOT";
                break;
            case ':':
                col = start + 1;
                if (peek(prog, start) == ':') {
                     token += advance(prog, start);
                }
                token += advance(prog, start);
                token_name = op[token];
                break;
            case '.':
                col = start + 1;
                if (peek(prog, start) == prog[start])
                {
                    if (prog[start+2] ==  prog[start]) {
                        token += advance(prog, start, 3);
                    }
                    else {
                        token += advance(prog, start, 2);
                    }
                }
                else
                {
                    token += advance(prog, start);
                }
                token_name = op[token];
                break;
            case '-':
                col = start + 1;
                if (peek(prog, start) == '>')
                {
                    token += advance(prog, start, 2);
                    token_name = op[token];
                }
                else
                {
                    inv_char.push_back({row, start, string(1, prog[start]), "INVALID_CHAR"});
                    start++;
                }
                break;
            default:
                col = start + 1;
                if (isalpha(prog[start]) || prog[start] == '0')
                {
                    while (start < prog.length() && (isalpha(prog[start]) || (prog[start] >= '0' && prog[start] <= '9')))
                    {
                        token += advance(prog, start);
                        
                    }
                    if (token[0] == '0' && token[1] == 'x' && token.length() >= 3) {
                        token_name = "HEX"; 
                    }
                    if (token[0] == '0' && token[1] == 'b' && token.length() >= 3) {
                        token_name = "BIN"; 
                    }
                }
                else
                {
                    // # theke start kore shesh porjonto oi line e shb comment
                    if (prog[start] == '#') { 
                        if (prog[start+1]!='[') {
                            while (start < prog.length()) {
                                start++;
                            }
                        }
                        
                    }
                    while (start < prog.length() && !isspace(prog[start]) && !isalpha(prog[start]))
                    {
                        token += advance(prog, start);
                    }
                    inv_char.push_back({row, start, token, "INVALID_CHAR"});
                    token.clear();
                }
                break;
            }

            if (token != "")
                cout << row << ":" << col << " " << token_name << " " << token << '\n';

            token.clear();
            token_name.clear();
        }
    }

    cout << "--- ERRORS ---\n";
    if (inv_char.size() == 0)
        cout << "(none)\n";
    else
    {
        for (auto it : inv_char)
        {
            cout << it.row << ":" << it.col + 1 << " " << it.name << " [" << it.ch << "] \n";
        }
    }

    cout << "--- end ---\n";
    return 0;
}
