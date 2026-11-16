#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;


enum TokenType {
    INT,
    FLOAT,
    STRING,
    LPAREN,
    RPAREN,
    LSQRUARE,
    RSQUARE,
    LCURLY,
    RCURLY,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    SEMICOLON,
    LT,
    LTLT,
    GT,
    GTGT,
    EQ,
    EQEQ,
    LTEQ,
    LSHIFT,
    RSHIFT,
    GTEQ,
    ID,
    UNKNOWN,
};


struct Token {
    TokenType type;
    string _token;
    int line, col;
};


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
        cout << this->content << '\n';
       
        fin.close();
    }
   
    ~Tokenizer() {
        delete[] this->content;
    }
       
    bool is_eof() {
        return this->cur >= this->end;
    }
   
    char peek(int pos = 0) {
        if (this->cur+pos >= this->end) {
            return '\0';
        }
        return this->content[this->cur + pos];
    }
   
    string advance(int pos = 1) {
        if (!is_eof()) {
            return "\0";
        }
        int mv = min(pos, this->end-this->cur);
        this->cur += mv;
        this->col += mv;
        return string(this->content).substr(this->cur-pos, mv);
    }
   
    void skipws() {
        while (!is_eof() &&
               isspace(peek())) {
            if (peek() == '\n') {
                ++this->line;
                this->col = 1;
            }
            advance();
        }
    }
       
       
    string read_string() {
        if (peek() != '"') {
            cerr << "Error: string does not start with '\"'\n";
            return "";
        }
       
        advance(); // read the first "
        string str = "";
        while (!is_eof() && peek() != '"') {
            str += advance();
        }
       
        if (is_eof()) {
            cerr << "Error: unterminated string\n";
            return "";
        }
       
        advance(); /// read ending "
        return str;
    }
       
    string read_number() {
        return "";
    }
   
    string read_identifier() {
        return "";
    }
   
    vector<Token> tokenize() {
        cout << this->content << '\n';
        vector<Token> tokens;
       
        string token = "";
        while (!is_eof() || peek() != '\0') {
            skipws();
            switch (peek()) {
                case '"':
                    token = read_string();
                    tokens.push_back(
                        Token{
                            STRING,
                            token,
                            this->line,
                            this->col
                        }
                    );
                    break;
                case '{':
                    token += advance();
                    tokens.push_back(
                        Token{
                            LCURLY,
                            token,
                            this->line,
                            this->col
                        }
                    );
                    break;
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
                    token += advance();
                    if (peek() == '=') {
                        token += advance();
                        tokens.push_back(
                            Token{
                                LTEQ,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    } else if (peek() == '<') {
                        token += advance();
                        tokens.push_back(
                            Token{
                                LSHIFT,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    } else {
                        tokens.push_back(
                            Token{
                                LT,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    }
                    break;
                case '>':
                    token += advance();
                    if (peek() == '=') {
                        token += advance();
                        tokens.push_back(
                            Token{
                                GTEQ,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    } else if (peek() == '>') {
                        token += advance();
                        tokens.push_back(
                            Token{
                                RSHIFT,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    }
                    else {
                        tokens.push_back(
                            Token{
                                GT,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    }
                    break;
                case '=':
                    token += advance();
                    if (peek() == '=') {
                        token += advance();
                        tokens.push_back(
                            Token{
                                EQEQ,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    }
                    else {
                        tokens.push_back(
                            Token{
                                EQ,
                                token,
                                this->line,
                                this->col
                            }
                        );
                    }
                    break;
                default:
                    if (isalpha(peek())) {
                        while (!is_eof() &&
                               isalpha(peek())){
                            token += advance();
                        }
                    } else if (isdigit(peek())) {
                        while (!is_eof() &&
                               isdigit(peek())){
                            token += advance();
                        }
                    } else {
                        //cout << "Error: unknown token '" << advance() << "'\n";
                        advance();
                            tokens.push_back(Token{
                            UNKNOWN,
                            token,
                            this->line,
                            this->col
                        });
                    }
                    break;
            }
           
            if (!token.empty()) {
                
            }
            token.clear();
        }
    }
   
private:
    char *content = NULL;
    int cur = 0;
    int end = 0;
    int line = 1, col = 1;
};

int main(int argc, char *argv[]) {
    Tokenizer tok(argv[1]);
   
    /*
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