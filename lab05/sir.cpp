#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

class Tokenizer {
public:
    Tokenizer(char *fname) {
        ifstream fin(fname);
       
        if (!fin.is_open()) {
            cerr << "File could not be opened\n";
            exit(-1);
        }
       
        fin.seekg(0, ios::end);
        size_t fsize = fin.tellg();
        this->end = fsize;
        fin.seekg(0, ios::beg);
       
        this->content = new char[fsize+1];
        fin.read(this->content, fsize);
       
        this->content[fsize] = '\0';
       
        fin.close();
    }
   
    ~Tokenizer() {
        delete[] this->content;
    }
   
    char peek(int pos = 0) {
        if (pos > this->end) {
            return '\0';
        }
        return this->content[cur + pos];
    }
   
    char advance(int pos = 1) {
        if (pos > this->end) {
            return '\0';
        }
        this->cur += pos;
        return this->content[this->cur - pos];
    }
   
    char skipws() {
        while (this->cur < this->end &&
               isspace(this->content[this->cur])) {
            ++this->cur;
        }
    }
       
       
    string readString() {
        string str = "";
        if (this->content[this->cur] != '"') {
            cerr << "Error: string does not this->cur with '\"'\n";
            return "";
        }
        advance(); // read the first "
       
        while (this->cur < this->end
               && peek() != '"') {
            str += advance();
        }
       
        if (this->cur >= this->end) {
            cerr << "Error: unterminated string\n";
            return "";
        }
       
        advance(); /// read ending "
        return str;
    }

   
    vector<string> tokenize() {
        vector<string> tokens;
       
        string token = "";
        while (this->cur < this->end) {
            skipws();
           
            switch (peek()) {
                case '"':
                    token = readString();
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
                    token += advance();
                    break;
                case '<':
                    if (peek(1) == '=') {
                        token += advance();
                        token += advance();
                    } else if (peek(1) == '<') {
                        token += advance();
                        token += advance();
                        if (peek() == '=') {
                            token += advance();
                        }
                    } else {
                        token += advance();
                    }
                    break;
                case '>':
                    if (peek(1) == '=') {
                        token += advance();
                        token += advance();
                    } else if (peek(1) == '>') {
                        token += advance();
                        if (peek(1) == '=') {
                            token += advance();
                        }
                    } else {
                        token += advance();
                    }
                    break;
                case '=':
                    if (peek(1) == '=') {
                        token += advance();
                        token += advance();
                    } else {
                        token += advance();
                    }
                    break;
                default:
                    if (isalpha(this->content[this->cur])) {
                        while (this->cur < this->end &&
                               isalpha(this->content[this->cur])){
                            token += advance();
                        }
                    } else if (isdigit(this->content[this->cur])) {
                        while (this->cur < this->end &&
                               isdigit(this->content[this->cur])){
                            token += advance();
                        }
                    }
                    break;
            }
            tokens.push_back(token);
            token.clear();
        }
    }
   
private:
    char *content{ nullptr };
    int cur, end;
};

int main(int argc, char *argv[]) {
    Tokenizer tok(argv[1]);
   
    vector<string> tokens = tok.tokenize();
   
    for (int i = 0; i < tokens.size(); ++i) {
        cout << tokens[i] << '\n';
    }
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