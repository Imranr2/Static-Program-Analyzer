#include "DesignExtractor.h"
#include "SP.h"
#include "Stream.h"
#include "SpValidator.h"
#include "Tokenizer.h"
#include "Token.h"

#include <iostream>
#include <sstream>
#include <string>

#include "SpParser.h"

spa::SP::SP(std::string source, PKBManager& pkbManager) :
    sourceCode(source), pkbManager(pkbManager) {}

void spa::SP::processSource() {
    Stream<Token> convertedTokens = convertToken();
    SpValidator validator(convertedTokens);
     std::cout << "TESTING VALIDATOR" << std::endl;
     for (int64_t i = 0; i < convertedTokens.remaining(); i++) {
        std::cout << "Type: " << convertedTokens[i].getType() <<
            ", Value: " << convertedTokens[i].getValue() << std::endl;
    }
     std::cout << "END OF TESTING" << std::endl;
    validator.validateGrammar();
    SpParser parser = SpParser(convertedTokens);
    std::vector<ProcedureStatement> procedureList = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(pkbManager, procedureList);
    designExtractor.extractRelationship();
}

spa::Stream<spa::Token> spa::SP::convertToken() {
    std::stringstream ss(sourceCode);
    Tokenizer tokenizer;
    Stream<Token> tokens = tokenizer.tokenize(ss);
    for (int64_t i = 0; i < tokens.remaining(); i++) {
        Token currToken = tokens[i];
        TokenType currTokenType = currToken.getType();
        bool isTokenNameType = currTokenType == TOKEN_NAME;
        if (!isTokenNameType) {
            continue;
        }

        std::string currTokenValue = currToken.getValue();
        bool isValidTokenName = stmtTokensMap.count(currTokenValue);
        if (isValidTokenName) {
            tokens[i] = Token(stmtTokensMap.at(currTokenValue), currTokenValue);
        }
    }
    return tokens;
}
