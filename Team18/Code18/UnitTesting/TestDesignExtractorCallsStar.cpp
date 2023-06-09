#include <iostream>
#include <vector>

#include "stdafx.h"

#include "DesignExtractor.h"
#include "PKB.h"
#include "SpParser.h"
#include "Token.h"
#include "UtilsFunction.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTesting {
    TEST_CLASS(TestDesignExtractorCallsStar) {
        std::string varA = "a";
        std::string varB = "b";
        std::string varC = "c";
        std::string varD = "d";
        std::string varE = "e";
        std::string constant = "1";

        std::string openBracket = "(";
        std::string closeBracket = ")";
        std::string openBrace = "{";
        std::string closeBrace = "}";
        std::string semiColon = ";";

        std::string minusOp = "-";
        std::string multiplyOp = "*";
        std::string greaterEqOp = ">=";
        std::string equal = "=";

        std::string readString = "read";
        std::string callString = "call";
        std::string printString = "print";
        std::string procedure = "procedure";
        std::string ifString = "if";
        std::string thenString = "then";
        std::string elseString = "else";
        std::string whileString = "while";

        spa::Token tokenA = spa::Token(spa::TOKEN_NAME, varA);
        spa::Token tokenB = spa::Token(spa::TOKEN_NAME, varB);
        spa::Token tokenC = spa::Token(spa::TOKEN_NAME, varC);
        spa::Token tokenD = spa::Token(spa::TOKEN_NAME, varD);
        spa::Token tokenE = spa::Token(spa::TOKEN_NAME, varE);
        spa::Token tokenConstant = spa::Token(spa::TOKEN_INTEGER, constant);

        spa::Token tokenOpenBracket = spa::Token(spa::TOKEN_OPEN_BRACKET, openBracket);
        spa::Token tokenCloseBracket = spa::Token(spa::TOKEN_CLOSE_BRACKET, closeBracket);
        spa::Token tokenOpenBrace = spa::Token(spa::TOKEN_OPEN_BRACE, openBrace);
        spa::Token tokenCloseBrace = spa::Token(spa::TOKEN_CLOSE_BRACE, closeBrace);
        spa::Token tokenSemiColon = spa::Token(spa::TOKEN_SEMICOLON, semiColon);

        spa::Token tokenMinusOp = spa::Token(spa::TOKEN_MINUS, minusOp);
        spa::Token tokenMultiply = spa::Token(spa::TOKEN_MULTIPLY, multiplyOp);
        spa::Token tokenAssign = spa::Token(spa::TOKEN_EQUAL, equal);
        spa::Token tokenGreaterEqual = spa::Token(spa::TOKEN_COND_GTE, greaterEqOp);

        spa::Token tokenRead = spa::Token(spa::TOKEN_READ, readString);
        spa::Token tokenCall = spa::Token(spa::TOKEN_CALL, callString);
        spa::Token tokenPrint = spa::Token(spa::TOKEN_PRINT, printString);
        spa::Token tokenProcedure = spa::Token(spa::TOKEN_PROCEDURE, procedure);
        spa::Token tokenIf = spa::Token(spa::TOKEN_IF, ifString);
        spa::Token tokenThen = spa::Token(spa::TOKEN_THEN, thenString);
        spa::Token tokenElse = spa::Token(spa::TOKEN_ELSE, elseString);
        spa::Token tokenWhile = spa::Token(spa::TOKEN_WHILE, whileString);

        std::vector<spa::Token> tokenList{};

public:
    TEST_METHOD(TestSingleCallsStar) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    a = 5;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 2);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = true;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);
    }

    TEST_METHOD(TestSingleCallsStarNegative) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    a = 5;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 2);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = false;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);
    }

    TEST_METHOD(TestDoubleCallsStar) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         * 4.    call e;
         *  }
         *   procedure d {
         * 5.    a = 5;
         *   }
         *   procedure e {
         * 6.    b = 6;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 3);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = true;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStarStar = secondRes.getIsTrue();

        bool expectedSecondCallsStarStar = true;

        Assert::IsTrue(expectedSecondCallsStarStar == testSecondCallsStarStar);
    }

    TEST_METHOD(TestDoubleCallsStarNegative) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         * 4.    call e;
         *  }
         *   procedure d {
         * 5.    a = 5;
         *   }
         *   procedure e {
         * 6.    b = 6;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 3);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = false;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStarStar = secondRes.getIsTrue();

        bool expectedSecondCallsStarStar = false;

        Assert::IsTrue(expectedSecondCallsStarStar == testSecondCallsStarStar);
    }

    TEST_METHOD(TestOneChainCallsStar) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    b = 6;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 3);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = true;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = true;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = secondRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);
    }

    TEST_METHOD(TestOneChainCallsStarNegative) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    b = 6;
         *   }
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 3);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = false;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = false;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = secondRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);
    }

    TEST_METHOD(TestTwoChainCallsStar) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    call b;
         *   }
         *   procedure b {
         * 6.    d = 9;
         *   }
         *
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenD, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = true;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = true;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);

        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStarStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStarStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestTwoChainCallsNegative) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    call b;
         *   }
         *   procedure b {
         * 6.    d = 9;
         *   }
         *
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenD, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };
        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();


        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = false;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = false;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);

        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "n", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStarStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStarStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "m", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestThreeChainCallsStar) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    call b;
         *   }
         *   procedure b {
         * 6.    call c;
         *   }
         *
         *   procedure c{
         * 7.     a = 1;
         *    }
         *
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 5);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = true;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = true;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);

        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);

        spa::PqlArgument pqlSeventhArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSeventhArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult seventhRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSeventhArgOne), spa::PKBQueryArg(pqlSeventhArgTwo));

        Assert::IsTrue(seventhRes.getQueryResultType() == spa::BOOL);

        bool testSeventhCallsStar = seventhRes.getIsTrue();

        bool expectedSeventhCalls = true;

        Assert::IsTrue(expectedSeventhCalls == testSeventhCallsStar);


        spa::PqlArgument pqlEighthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlEighthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult eighthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlEighthArgOne), spa::PKBQueryArg(pqlEighthArgTwo));

        Assert::IsTrue(eighthRes.getQueryResultType() == spa::BOOL);

        bool testEighthCallsStar = eighthRes.getIsTrue();

        bool expectedEighthCalls = true;

        Assert::IsTrue(expectedEighthCalls == testEighthCallsStar);


        spa::PqlArgument pqlNinthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlNinthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult ninthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlNinthArgOne), spa::PKBQueryArg(pqlNinthArgTwo));

        Assert::IsTrue(ninthRes.getQueryResultType() == spa::BOOL);

        bool testNinthCallsStar = ninthRes.getIsTrue();

        bool expectedNinthCalls = true;

        Assert::IsTrue(expectedNinthCalls == testNinthCallsStar);


        spa::PqlArgument pqlTenthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlTenthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult tenthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlTenthArgOne), spa::PKBQueryArg(pqlTenthArgTwo));

        Assert::IsTrue(tenthRes.getQueryResultType() == spa::BOOL);

        bool testTenthCallsStar = tenthRes.getIsTrue();

        bool expectedTenthCalls = true;

        Assert::IsTrue(expectedTenthCalls == testTenthCallsStar);
    }

    TEST_METHOD(TestThreeChainCallsNegative) {
        /*
         *   procedure a {
         * 1.    b = 1;
         * 2.    c = b;
         * 3.    call d;
         *  }
         *
         *   procedure d {
         * 4.    call e;
         *   }
         *   procedure e {
         * 5.    call b;
         *   }
         *   procedure b {
         * 6.    call c;
         *   }
         *
         *   procedure c{
         * 7.     a = 1;
         *    }
         *
         */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenC, tokenAssign, tokenB, tokenSemiColon,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 5);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "z", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCallsStar = firstRes.getIsTrue();

        bool expectedFirstCallsStar = false;

        Assert::IsTrue(expectedFirstCallsStar == testFirstCallsStar);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCallsStar = secondRes.getIsTrue();

        bool expectedSecondCallsStar = false;

        Assert::IsTrue(expectedSecondCallsStar == testSecondCallsStar);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCallsStar = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCallsStar);

        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "z", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "n", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);

        spa::PqlArgument pqlSeventhArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSeventhArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "g", {});
        spa::QueryResult seventhRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSeventhArgOne), spa::PKBQueryArg(pqlSeventhArgTwo));

        Assert::IsTrue(seventhRes.getQueryResultType() == spa::BOOL);

        bool testSeventhCallsStar = seventhRes.getIsTrue();

        bool expectedSeventhCalls = false;

        Assert::IsTrue(expectedSeventhCalls == testSeventhCallsStar);


        spa::PqlArgument pqlEighthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlEighthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "z", {});
        spa::QueryResult eighthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlEighthArgOne), spa::PKBQueryArg(pqlEighthArgTwo));

        Assert::IsTrue(eighthRes.getQueryResultType() == spa::BOOL);

        bool testEighthCallsStar = eighthRes.getIsTrue();

        bool expectedEighthCalls = false;

        Assert::IsTrue(expectedEighthCalls == testEighthCallsStar);


        spa::PqlArgument pqlNinthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlNinthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "m", {});
        spa::QueryResult ninthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlNinthArgOne), spa::PKBQueryArg(pqlNinthArgTwo));

        Assert::IsTrue(ninthRes.getQueryResultType() == spa::BOOL);

        bool testNinthCallsStar = ninthRes.getIsTrue();

        bool expectedNinthCalls = false;

        Assert::IsTrue(expectedNinthCalls == testNinthCallsStar);


        spa::PqlArgument pqlTenthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::PqlArgument pqlTenthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "z", {});
        spa::QueryResult tenthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlTenthArgOne), spa::PKBQueryArg(pqlTenthArgTwo));

        Assert::IsTrue(tenthRes.getQueryResultType() == spa::BOOL);

        bool testTenthCallsStar = tenthRes.getIsTrue();

        bool expectedTenthCalls = false;

        Assert::IsTrue(expectedTenthCalls == testTenthCallsStar);
    }

    TEST_METHOD(TestNestedOneCallsStar) {
        /*
        *  procedure a {
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        call c;
        *      }
        *  }
        * procedure b {
        *5.    call c;
        *  }
        * procedure c{
        *6.    call d;
        *  }
        * procedure d{
        *7.      c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure,
          tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = true;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = true;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedOneCallsStarNegative) {
        /*
        *  procedure a {
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        call c;
        *      }
        *  }
        * procedure b {
        *5.    call c;
        *  }
        * procedure c{
        *6.    call d;
        *  }
        * procedure d{
        *7.      c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure,
          tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = false;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "f", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = false;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "g", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;
        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
        }

    TEST_METHOD(TestNestedTwoCallsStarOne) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        if (d >= 1) then {
        *5.            b = 3;
        *          }
        *          else {
        *6.            call c;
        *          }
        *      }
        *  }
        *  procedure b{
        *7.     call c;
        *  }
        *  procedure c{
        *8.    call d;
        *  }
        *  procedure d{
        *9.   c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual,
          tokenC, tokenCloseBracket, tokenOpenBrace,
          tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual,
          tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace, tokenElse, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }
        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = true;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = true;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedTwoCallsStarOneNegative) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        if (d >= 1) then {
        *5.            b = 3;
        *          }
        *          else {
        *6.            call c;
        *          }
        *      }
        *  }
        *  procedure b{
        *7.     call c;
        *  }
        *  procedure c{
        *8.    call d;
        *  }
        *  procedure d{
        *9.   c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
          tokenCloseBracket, tokenOpenBrace,
          tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
          tokenCloseBracket, tokenThen, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace, tokenElse, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = false;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "f", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = false;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "g", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;
        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedTwoCallsStarTwo) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        if (d >= 1) then {
        *5.            b = 3;
        *6.            call c;
        *          }
        *          else {
        *7.           b = 3;
        *          }
        *      }
        *  }
        *  procedure b{
        *8.     call c;
        *  }
        *  procedure c{
        *9.    call d;
        *  }
        *  procedure d{
        *10.   c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
          tokenCloseBracket, tokenOpenBrace,
          tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant,
          tokenCloseBracket, tokenThen, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenElse, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
          tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }
        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = true;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = true;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedTwoCallsStarTwoNegative) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    call b;
        *3.    while (b >= c) {
        *4.        if (d >= 1) then {
        *5.            b = 3;
        *6.            call c;
        *          }
        *          else {
        *7.           b = 3;
        *          }
        *      }
        *  }
        *  procedure b{
        *8.     call c;
        *  }
        *  procedure c{
        *9.    call d;
        *  }
        *  procedure d{
        *10.   c = 10;
        *  }
        *
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC,
          tokenCloseBracket, tokenOpenBrace,
          tokenIf, tokenOpenBracket, tokenD, tokenGreaterEqual,
          tokenConstant, tokenCloseBracket, tokenThen, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenElse, tokenOpenBrace,
          tokenB, tokenAssign, tokenConstant, tokenSemiColon, tokenCloseBrace,
          tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenC, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenProcedure, tokenD, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };

        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }

        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 4);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = false;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "f", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = false;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "g", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;
        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedMultipleCallsStar) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    while (b >= c) {
        *3.        call c;
        *4.        while (c >= 8) {
        *5.            call b;
        *6.            while (d >= 6) {
        *7.                call d;
        *              }
        *          }
        *      }
        *  }
        *  procedure b{
        *8.  call c;
        *  }
        *  procedure c{
        *9.  c = 10;
        *  }
        *  procedure d{
        *10.  call e;
        *  }
        *  procedure e{
        *10.  a = 10;
        *  }
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };


        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }
        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 5);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = true;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = true;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = true;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = true;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "c", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = true;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "e", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = true;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
    }

    TEST_METHOD(TestNestedMultipleCallsStarNegative) {
        /*
        *  procedure a{
        *1.    a = 5;
        *2.    while (b >= c) {
        *3.        call c;
        *4.        while (c >= 8) {
        *5.            call b;
        *6.            while (d >= 6) {
        *7.                call d;
        *              }
        *          }
        *      }
        *  }
        *  procedure b{
        *8.  call c;
        *  }
        *  procedure c{
        *9.  c = 10;
        *  }
        *  procedure d{
        *10.  call e;
        *  }
        *  procedure e{
        *10.  a = 10;
        *  }
        */
        tokenList = {
          tokenProcedure, tokenA, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenB, tokenGreaterEqual, tokenC, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenC, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenB, tokenSemiColon,
          tokenWhile, tokenOpenBracket, tokenD, tokenGreaterEqual, tokenConstant, tokenCloseBracket, tokenOpenBrace,
          tokenCall, tokenD, tokenSemiColon,
          tokenCloseBrace, tokenCloseBrace, tokenCloseBrace, tokenCloseBrace,
          tokenProcedure, tokenB, tokenOpenBrace,
          tokenCall, tokenC, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenC, tokenOpenBrace,
          tokenC, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenD, tokenOpenBrace,
          tokenCall, tokenE, tokenSemiColon,
          tokenCloseBrace,
          tokenProcedure, tokenE, tokenOpenBrace,
          tokenA, tokenAssign, tokenConstant, tokenSemiColon,
          tokenCloseBrace
        };


        spa::Stream<spa::Token> tokenStream = spa::Stream<spa::Token>();
        for (auto token : tokenList) {
            tokenStream.pushBack(token);
        }
        std::unique_ptr<spa::PKBManager> pkbManager = std::make_unique<spa::PKB>();

        auto parser = spa::SpParser(tokenStream);
        std::vector<std::shared_ptr<spa::ProcedureStatement>>  procedureList = parser.parse();
        Assert::IsTrue(procedureList.size() == 5);

        spa::DesignExtractor designExtractor = spa::DesignExtractor(*pkbManager, procedureList);
        designExtractor.extractRelationship();

        spa::PqlArgument pqlArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "z", {});
        spa::QueryResult firstRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlArgOne), spa::PKBQueryArg(pqlArgTwo));

        Assert::IsTrue(firstRes.getQueryResultType() == spa::BOOL);

        bool testFirstCalls = firstRes.getIsTrue();

        bool expectedFirstCalls = false;

        Assert::IsTrue(expectedFirstCalls == testFirstCalls);

        spa::PqlArgument pqlSecondArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlSecondArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "y", {});
        spa::QueryResult secondRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSecondArgOne), spa::PKBQueryArg(pqlSecondArgTwo));

        Assert::IsTrue(secondRes.getQueryResultType() == spa::BOOL);

        bool testSecondCalls = secondRes.getIsTrue();

        bool expectedSecondCalls = false;

        Assert::IsTrue(expectedSecondCalls == testSecondCalls);

        spa::PqlArgument pqlThirdArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlThirdArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "x", {});
        spa::QueryResult thirdRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlThirdArgOne), spa::PKBQueryArg(pqlThirdArgTwo));

        Assert::IsTrue(thirdRes.getQueryResultType() == spa::BOOL);

        bool testThirdCalls = thirdRes.getIsTrue();

        bool expectedThirdCalls = false;

        Assert::IsTrue(expectedThirdCalls == testThirdCalls);


        spa::PqlArgument pqlFourthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::PqlArgument pqlFourthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "n", {});
        spa::QueryResult fourthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFourthArgOne), spa::PKBQueryArg(pqlFourthArgTwo));

        Assert::IsTrue(fourthRes.getQueryResultType() == spa::BOOL);

        bool testtFourthCallsStar = fourthRes.getIsTrue();

        bool expectedFourthCalls = false;

        Assert::IsTrue(expectedFourthCalls == testtFourthCallsStar);

        spa::PqlArgument pqlFifthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "b", {});
        spa::PqlArgument pqlFifthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult fifthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlFifthArgOne), spa::PKBQueryArg(pqlFifthArgTwo));

        Assert::IsTrue(fifthRes.getQueryResultType() == spa::BOOL);

        bool testtFifthCallsStar = fifthRes.getIsTrue();

        bool expectedFifthCalls = false;

        Assert::IsTrue(expectedFifthCalls == testtFifthCallsStar);

        spa::PqlArgument pqlSixthArgOne = spa::PqlArgument(spa::LITERAL_STRING, "d", {});
        spa::PqlArgument pqlSixthArgTwo = spa::PqlArgument(spa::LITERAL_STRING, "a", {});
        spa::QueryResult sixthRes = pkbManager->getRelationship(spa::CALLS_STAR,
            spa::PKBQueryArg(pqlSixthArgOne), spa::PKBQueryArg(pqlSixthArgTwo));

        Assert::IsTrue(sixthRes.getQueryResultType() == spa::BOOL);

        bool testSixthCallsStar = sixthRes.getIsTrue();

        bool expectedSixthCalls = false;

        Assert::IsTrue(expectedSixthCalls == testSixthCallsStar);
        }
};
}  // namespace UnitTesting
