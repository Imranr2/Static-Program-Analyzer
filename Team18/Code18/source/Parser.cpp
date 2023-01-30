#include <vector>
#include <unordered_set>

#include "Parser.h"

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "Statement.h"
#include "Procedure.h"
#include "UtilsFunction.h"

//Constructor for Parser
Parser::Parser(spa::Stream<spa::Token>& tokenStream) : tokenStream(tokenStream) {}

UtilsFunction utils;

//TODO to replace TOKEN_SEMICOLON with TOKEN_PROCEDURE
std::vector<Procedure> Parser::parse() {
  std::vector<Procedure> procedureList;
  while (!isEndOfProgram()) {
    if (matchToken(spa::TOKEN_SEMICOLON)) {
      procedureList.push_back(processProcedure());
    }
  }
  return procedureList;
}

Procedure Parser::processProcedure() {
  skipCurrToken(); // To skip over the procedure token
  spa::Token procedureVarToken = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over open brace token
  std::unordered_set<int> whileStmtParents;
  std::unordered_set<int> ifStmtParents;
  std::vector<Statement*> statementLst = processStmtList(procedureVarToken.getValue(), whileStmtParents,
    ifStmtParents);
  Procedure procedure = Procedure(procedureVarToken, statementLst);
  return procedure;
}

std::vector<Statement*> Parser::processStmtList(std::string parentProcedureVal,
                                                std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents) {
  std::vector<Statement*> statements;
  while (!matchToken(spa::TOKEN_CLOSE_BRACE) && !isEndOfProgram()) {
    statements.push_back(handleStatements(parentProcedureVal, whileStmtParents, ifStmtParents));
  }
  skipCurrToken(); // Skip over close brace token
  return statements;
}

//TODO To replace all the TOKEN_OPEN_BRACE with if, while, call, read, print, assign stmt tokens
Statement* Parser::handleStatements(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents) {
  spa::TokenType currentTokenType = getCurrTokenAndAdvance().getType();
  switch (currentTokenType) {
    case spa::TOKEN_OPEN_BRACE:
      return processReadStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_BOOL_AND:
      return processPrintStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    case spa::TOKEN_BOOL_OR:
      return processCallStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    //case spa::TOKEN_BOOL_NOT:
    //return processWhileStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    //case spa::TOKEN_COND_LT:
    //return processIfStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
    //default:
    //return processAssignStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  }
}


Statement* Parser::processReadStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                        std::unordered_set<int> ifStmtParents) {
  spa::Token readVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* readStatement = new ReadStatement(parentProcedureVal, readVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return readStatement;
}

Statement* Parser::processPrintStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                         std::unordered_set<int> ifStmtParents) {
  spa::Token printVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* printStatement = new PrintStatement(parentProcedureVal, printVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return printStatement;
}

Statement* Parser::processCallStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                        std::unordered_set<int> ifStmtParents) {
  spa::Token callVariable = getCurrTokenAndAdvance();
  skipCurrToken(); // Skip over semi colon token
  Statement* callStatement = new CallStatement(parentProcedureVal, callVariable.getValue(), whileStmtParents,
    ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return callStatement;
}

Statement* Parser::processWhileStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                         std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  whileStmtParents.insert(currentLineNum);
  std::vector<Statement*> whileStatementBlock{};
  Statement* whileConditionStatement = processWhileConditionStatement(parentProcedureVal, whileStmtParents,
    ifStmtParents);
  whileStatementBlock.push_back(whileConditionStatement);
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> whileStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* whileInnerBlockStatement = new InnerBlockStatement(parentProcedureVal, whileStatementList);
  whileStatementBlock.push_back(whileInnerBlockStatement);
  Statement* whileContainerStatement = new WhileContainerStatement(parentProcedureVal, currentLineNum,
    whileStatementBlock);
  return whileContainerStatement;
}

Statement* Parser::processWhileConditionStatement(std::string parentProcedureVal,
                                                  std::unordered_set<int> whileStmtParents,
                                                  std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_OPEN_BRACE) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  Statement* whileConditionStatement = new WhileConditionStatement(parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return whileConditionStatement;
}

Statement* Parser::processIfStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                      std::unordered_set<int> ifStmtParents) {
  int currentLineNum = statementLineNum;
  ifStmtParents.insert(currentLineNum);
  std::vector<Statement*> ifStatementBlock{};
  Statement* ifConditionStatement = processIfConditionStatement(parentProcedureVal, whileStmtParents, ifStmtParents);
  ifStatementBlock.push_back(ifConditionStatement);
  skipCurrToken(); // Skip over then token
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> thenStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* thenStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, thenStatementList);
  ifStatementBlock.push_back(thenStatementInnerBlock);
  skipCurrToken(); // Skip over else token 
  skipCurrToken(); // Skip over open brace token
  std::vector<Statement*> elseStatementList = processStmtList(parentProcedureVal, whileStmtParents, ifStmtParents);
  Statement* elseStatementInnerBlock = new InnerBlockStatement(parentProcedureVal, elseStatementList);
  ifStatementBlock.push_back(elseStatementInnerBlock);
  Statement* ifContainerStatement = new IfContainerStatement(parentProcedureVal, currentLineNum, ifStatementBlock);
  return ifContainerStatement;
}


//TODO: Replace TOKEN_BOOL_AND with TOKEN_THEN
Statement* Parser::processIfConditionStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                               std::unordered_set<int> ifStmtParents) {
  std::vector<spa::Token> rawConditionExpression{};
  while (getCurrToken().getType() != spa::TOKEN_BOOL_AND) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawConditionExpression.push_back(currToken);
  }
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawConditionExpression);
  Statement* ifConditionStatement = new IfConditionStatement(
    parentProcedureVal, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return ifConditionStatement;
}

Statement* Parser::processAssignStatement(std::string parentProcedureVal, std::unordered_set<int> whileStmtParents,
                                          std::unordered_set<int> ifStmtParents) {
  std::string assignmentVar = getPrevToken().getValue();
  skipCurrToken(); // Skip over equal token
  std::vector<spa::Token> rawAssignExpression = {};
  while (getCurrToken().getType() != spa::TOKEN_SEMICOLON) {
    spa::Token currToken = getCurrTokenAndAdvance();
    rawAssignExpression.push_back(currToken);
  }
  skipCurrToken(); // Skip over semi colon token
  std::string postfixExpression = UtilsFunction::infixToPostfix(rawAssignExpression);
  Statement* assignStatement = new AssignStatement(parentProcedureVal, assignmentVar, postfixExpression,
    whileStmtParents, ifStmtParents, statementLineNum);
  increaseStatementLineNum();
  return assignStatement;
}


// Private functions to traverse the stream of tokens
void Parser::advanceStream(int64_t offset) {
  tokenStream.seek(offset);
}

void Parser::skipCurrToken() {
  advanceStream(1);
}

bool Parser::matchToken(spa::TokenType tokenType) {
  if (isEndOfProgram()) {
    return false;
  }
  spa::TokenType nextTokenType = getCurrToken().getType();
  return nextTokenType == tokenType;
}


spa::Token Parser::getCurrToken() {
  return tokenStream[0];
}

spa::Token Parser::getPrevToken() {
  return tokenStream[-1];
}


spa::Token Parser::getCurrTokenAndAdvance() {
  spa::Token currToken = getCurrToken();
  advanceStream(1);
  return currToken;
}

//TODO to change to suitable tokenType in the future to check for end of program or add in function in stream to check for eof
bool Parser::isEndOfProgram() {
  spa::TokenType nextTokenType = getCurrToken().getType();
  if (nextTokenType == 1) {
    return true;
  }
  return false;
}

void Parser::increaseStatementLineNum() {
  statementLineNum++;
}
