#pragma once

#include <string>

namespace spa {
    enum TokenType {
        TOKEN_NAME,
        TOKEN_INTEGER,
        TOKEN_OPEN_BRACE,
        TOKEN_CLOSE_BRACE,
        TOKEN_OPEN_BRACKET,
        TOKEN_CLOSE_BRACKET,
        TOKEN_SEMICOLON,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_DIVIDE,
        TOKEN_MULTIPLY,
        TOKEN_MODULO,
        TOKEN_EQUAL,
        TOKEN_COND_EQ,
        TOKEN_COND_NEQ,
        TOKEN_COND_LT,
        TOKEN_COND_LTE,
        TOKEN_COND_GT,
        TOKEN_COND_GTE,
        TOKEN_BOOL_NOT,
        TOKEN_BOOL_AND,
        TOKEN_BOOL_OR,
        TOKEN_COMMA,
        TOKEN_UNDERSCORE,
        TOKEN_INVERTED_COMMA
    };

    class Token;

    bool operator==(const Token& t1, const Token& t2);
    bool operator!=(const Token& t1, const Token& t2);

    class Token {
    private:
        const TokenType type;
        const std::string value;
    public:
        Token(TokenType type, std::string value);
        Token(TokenType type, char c);
        const TokenType& getType();
        const std::string& getValue();

        friend bool operator==(const Token& t1, const Token& t2);
        friend bool operator!=(const Token& t1, const Token& t2);
    };
}
