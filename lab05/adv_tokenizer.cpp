#include <iostream>
#include <cstring>
#include <string>

using namespace std;


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
    return prog[start+1];
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

int main()
{
    string prog = "{\
					int i; int j; float[100] a; float v; float x;\
while ( true ) {\
do i = i+1; while ( a[i] < v );\
do j = j-1; while ( a[j] > v );\
if ( i >= j ) break;\
x = a[i]; a[i] = a[j]; a[j] = x;\
}\
}" ;

    int start = 0;
    string token = "";
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
        case '=':
            if (peek(prog, start) == '=')
            {
                token += advance(prog, start, 2);
            }
            else
            {
                token += advance(prog, start);
            }
            break;
        default:
            if (isalpha(prog[start]))
            {
                while (start < prog.length() && isalpha(prog[start]))
                {
                    token += advance(prog, start);
                }
            }
            else if (isdigit(prog[start]))
            {
                while (start < prog.length() && isdigit(prog[start]))
                {
                    token += advance(prog, start);
                }
            }
            break;
        }
        cout << "Token: " << token << '\n';
        token.clear();
    }
}