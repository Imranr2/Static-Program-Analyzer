#include "NonContainerStatement.h"

std::string spa::OneVarNonContainerStatement::getVariableName() {
  return variableName;
}

// Constructor for ReadStatement
spa::ReadStatement::ReadStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for PrintStatement
spa::PrintStatement::PrintStatement(std::string parentProcedureVal, std::string variableName,
                                    std::unordered_set<int> whileStmtParents,
                                    std::unordered_set<int> ifStmtParents,
                                    int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for CallStatement
spa::CallStatement::CallStatement(std::string parentProcedureVal, std::string variableName,
                                  std::unordered_set<int> whileStmtParents,
                                  std::unordered_set<int> ifStmtParents,
                                  int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->variableName = variableName;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}

// Constructor for ifConditionStatement
spa::AssignStatement::AssignStatement(std::string parentProcedureVal, std::string assignVar,
                                      std::string postfixExpr,
                                      std::unordered_set<int> whileStmtParents,
                                      std::unordered_set<int> ifStmtParents, int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->assignVar = assignVar;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
};

// Constructor for AssignStatement
spa::IfConditionStatement::IfConditionStatement(std::string parentProcedureVal,
                                                std::string postfixExpr,
                                                std::unordered_set<int> whileStmtParents,
                                                std::unordered_set<int> ifStmtParents,
                                                int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}


// Constructor for whileConditionStatement
spa::WhileConditionStatement::WhileConditionStatement(std::string parentProcedureVal,
                                                      std::string postfixExpr,
                                                      std::unordered_set<int> whileStmtParents,
                                                      std::unordered_set<int> ifStmtParents,
                                                      int statementLineNum) {
  this->parentProcedureVal = parentProcedureVal;
  this->postfixExpr = postfixExpr;
  this->whileStmtParents = whileStmtParents;
  this->ifStmtParents = ifStmtParents;
  this->statementLineNum = statementLineNum;
}
