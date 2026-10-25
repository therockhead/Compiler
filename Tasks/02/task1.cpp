#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

typedef struct invalid_char
{
    int row;
    int col;
    string ch;
} inv;

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

int main(int argc, char *argv[])
{
    fstream inf(argv[1], ios::in);
    if (!inf.is_open())
    {
        return 0;
    }

    string prog;

    int start = 0;
    string token = "";

    vector<inv> inv_char;

    int row = 0;
    while (getline(inf, prog))
    {
        start = 0;
        row++;
        while (start < prog.length())
        {
            skipws(prog, start);

            switch (prog[start])
            {
            case '"':
                token = readString(prog, start);
                break;
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case ';':
            case '+':
            case '-':
            case '%':
            case '/':
            case '^':
            case '*':
                token += advance(prog, start);
                break;
            case '<':
                if (peek(prog, start) == '=')
                {
                    token += advance(prog, start, 2);
                }
                else if (peek(prog, start) == '<')
                {
                    token += advance(prog, start);
                    token += advance(prog, start);
                    if (peek(prog, start) == '=')
                    {
                        token += advance(prog, start);
                    }
                }
                else
                {
                    token += advance(prog, start);
                }
                break;
            case '>':
                if (peek(prog, start) == '=')
                {
                    token += advance(prog, start);
                    token += advance(prog, start);
                }
                else if (peek(prog, start) == '>')
                {
                    token += advance(prog, start);
                    if (peek(prog, start) == '=')
                    {
                        token += advance(prog, start);
                    }
                }
                else
                {
                    token += advance(prog, start);
                }
                break;
            case '.':
                inv_char.push_back({row, start, "."});
                start++;
                break;
            case '=':
                inv_char.push_back({row, start, "="});
                start++;
                break;
            case '$':
                if (peek(prog, start) == '$') {
                    inv_char.push_back({row, start, "$$"});
                    start++;
                }
                else {
                    inv_char.push_back({row, start, "$"});
                }
                start++;
                break;
            default:
                if (isalpha(prog[start]))
                {
                    char curr = prog[start];

                    inv_char.push_back({row, start, {curr}});
                    ++start;


                }
                else if (isdigit(prog[start]))
                {
                    int temp = 0;
                    while (start < prog.length() && isdigit(prog[start]))
                    {
                        token += advance(prog, start);
                    }
                    if (start < prog.length() && prog[start] == '.')
                    {
                        if (start + 1 < prog.length() && isdigit(peek(prog, start)))
                        {
                            token += advance(prog, start); // min 1
                            while (start < prog.length() && isdigit(prog[start]))
                            {
                                token+=advance(prog, start);
                            }
                        }
                    }

                    if (start < prog.length() && prog[start] == '.') {
                        inv_char.push_back({row, start, "."});
                        start++;
                    }
                }
                break;
            }

            if (token != "") cout << row << ":" << start << " " << token << '\n';


            token.clear();
        }

    }

    cout << "--- ERRORS ---\n";
    for (auto it: inv_char) {
        cout << it.row << ":" << it.col+1 << " INVALID_CHAR" << " [" << it.ch << "] \n";
    }

    cout << "--- end ---\n";
    return 0;
}
