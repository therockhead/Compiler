#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

class Tokenizer
{
public:
  Tokenizer(char *fname)
  {
    ifstream fin(fname);

    if (!fin.is_open())
    {
      cerr << "File could not be opened\n";
      exit(-1);
    }

    fin.seekg(0, ios::end);
    size_t fsize = fin.tellg();
    this->end = fsize;
    fin.seekg(0, ios::beg);

    this->content = new char[fsize + 1];
    fin.read(this->content, fsize);

    this->content[fsize] = '\0';

    fin.close();
  }

  ~Tokenizer()
  {
    delete[] this->content;
  }

  void skipws(int &start)
  {
    while (start < this->content.length() && isspace(this->content[start]))
    {
      ++start;
    }
  }

  char peek(int pos = 1)
  {
    if (pos > this->end)
    {
      return '\0';
    }
    return this->content[cur + pos];
  }

  char advance(int pos = 1)
  {
    if (pos > this->end)
    {
      return '\0';
    }
    this->cur += 1;
    return this->content[this->cur - 1];
  }
  char *readString(int &start)
{
    string str = "";
    if (this->content[start] != '"')
    {
        cerr << "Error: string does not start with '\"'\n";
        return "";
    }
    advance(start); // read the first "

    while (this->cur < this->end && this->content[start] != '"')
    {
        str += advance(start);
    }

    if (start >= this->end)
    {
        cerr << "Error: unterminated string\n";
        return "";
    }

    advance(start); /// read ending "
    return str;
}

  vector<string> tokenize()
  {
    int start = 0;
    string token = "";
    while (this->cur < this->end)
    {
      skipws(start);

      switch (this->content[start])
      {
      case '"':
        token = readString(this->content, start);
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
    }
  }
  private:
    char *content{nullptr};
    int cur, end;
  };

  int main(int argc, char *argv[])
  {
    Tokenizer tok(argv[1]);

    vector<string> tokens = tok.tokenize();

    /*
    FILE *fd = fopen(argv[1], "r");

    if (fd == NULL) {
        cout << "File not found!\n";
        return -1;
    }

    char *buffer = (char *)calloc(1, 1024);

    fread(buffer, 1024, 1, fd);
    cout << buffer << '\n';
    free(buffer);
    fclose(fd);
    */
  }