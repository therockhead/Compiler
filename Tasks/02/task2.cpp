#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

typedef struct invalid_char
{
    int row;
    int col;
    string ch;
} inv;


map<string, string> op = {
    {"+", "PLUS"},
    {"-", "MINUS"},
    {"*", "STAR"},
    {"/", "SLASH"},
    {"%", "PERCENT"},
    {"^", "CARET"},
    {"(", "LPAREN"},
    {")", "RPAREN"},
    {"0", "INT"},
    {"1", "INT"},
    {"2", "INT"},
    {"3", "INT"},
    {"4", "INT"},
    {"5", "INT"},
    {"6", "INT"},
    {"7", "INT"},
    {"8", "INT"},
    {"9", "INT"},
};;


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
                col = start +1;
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
                col=start+1;
                token_name = op[token];
                token += advance(prog, start);
                token_name = op[token];
                break;
            case '<':
                col=start+1;
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
                col=start+1;
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
                col=start+1;
                inv_char.push_back({row, start, "."});
                start++;
                break;
            case '=':
                col=start+1;
                inv_char.push_back({row, start, "="});
                start++;
                break;
            case '$':
                col=start+1;
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
                col=start+1;
                if (isalpha(prog[start]))
                {
                    while(start <prog.length() && isalpha(prog[start])) {
                        token += advance(prog, start);
                    }
                }
                else if (isdigit(prog[start]))
                {
                    int temp = 0;
                    while (start < prog.length() && isdigit(prog[start]))
                    {
                        token += advance(prog, start);
                        token_name = "INT";
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
                        token_name="FLOAT";
                    }

                    if (start < prog.length() && prog[start] == '.') {
                        inv_char.push_back({row, start, "."});
                        start++;
                    }
                }
                break;
            }

            if (token != "") cout << row << ":" << col << " " << token_name << " " << token << '\n';


            token.clear();
            token_name.clear();
        }

    }

    cout << "--- ERRORS ---\n";
    if (inv_char.size() == 0) cout << "(none)\n";
    else {
        for (auto it: inv_char) {
           cout << it.row << ":" << it.col+1 << " INVALID_CHAR" << " [" << it.ch << "] \n";
        }
    }

    cout << "--- end ---\n";
    return 0;
}
