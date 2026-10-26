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

set<string> keywords = {
    "if", "else", "while", "for", "return", "int", "float"};

map<string, string> op = {
    {"=", "ASSIGN"},
    {"==", "EQ"},
    {"!=", "NEQ"},
    {"<=", "LE"},
    {">=", "GE"},
    {"<", "LT"},
    {">", "GT"},
    {"!", "NOT"},
    {"&&", "AND"},
    {"||", "OR"},
    {";", "SEMI"},
    {",", "COMMA"},
    {"{", "LBRACE"},
    {"}", "RBRACE"},
    {"[", "LBRACKET"},
    {"]", "RBRACKET"},
};


string str(int ch)
{
    return "" + ch;
}
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
                col = start + 1;
                token = readString(prog, start);
                break;
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case ';':
            case ',':
                col = start + 1;
                token += advance(prog, start);
                token_name = op[token];
                break;
            case '<':
            case '>':
            case '!':
            case '=':
                col = start + 1;
                if (peek(prog, start) == '=')
                {
                    token += advance(prog, start, 2);
                }
                else
                {
                    token += advance(prog, start);
                }
                token_name = op[token];
                break;
            case '&':
            case '|':
                col = start + 1;
                if (peek(prog, start) == prog[start])
                {
                    token += advance(prog, start, 2);
                }
                else
                {
                    inv_char.push_back({row, start, string(1, prog[start]), "INCOMPLETE_OPERATOR"});
                    start++;
                }
                token_name = op[token];
                break;
            default:
                col = start + 1;
                if (isalpha(prog[start]) || prog[start] == '_')
                {
                    while (start < prog.length() && (isalpha(prog[start]) || prog[start] == '_' || (prog[start] >= '0' && prog[start] <= '9')))
                    {
                        token += advance(prog, start);
                        token_name = "IDENT";
                    }
                    if (keywords.find(token) != keywords.end())
                    {
                        token_name = "KEYWORD";
                    }
                }
                else
                {
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
