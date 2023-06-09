#include "SimpleEvaluator.h"
#include "PqlAttributes.h"
#include "Literal.h"

spa::SimpleEvaluator::SimpleEvaluator(std::string selectSynonym, DesignEntityType designEntityType) :
  selectSynonym(selectSynonym), designEntityType(designEntityType) {
}

spa::QpsResultTable spa::SimpleEvaluator::evaluate(PKBManager& pkbManager) {
  switch (designEntityType) {
  case CONSTANT:
    return evaluateConstants(pkbManager);
  case PROCEDURE:
  case VARIABLE: {
    return evaluateNames(pkbManager);
  }
  case STMT:
  case ASSIGN:
  case WHILE:
  case IF: {
    return evaluateStatementNumbers(pkbManager);
  }
  case READ: {
    return evaluateRead(pkbManager);
  }
  case PRINT: {
    return evaluatePrint(pkbManager);
  }
  case CALL: {
    return evaluateCall(pkbManager);
  }
  default: {
    return {};
  }
  }
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateRead(PKBManager& pkbManager) {
  PKBQueryArg readSynonymArg(PqlArgument(ArgumentType::SYNONYM, READ_SYNONYM_LITERAL, designEntityType));
  PKBQueryArg varSynonymArg(PqlArgument(ArgumentType::SYNONYM, VARIABLE_SYNONYM_LITERAL, DesignEntityType::VARIABLE));
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getRelationship(RelationshipType::MODIFIES, readSynonymArg, varSynonymArg);
  for (auto& pair : result.getLineNumberNamePairs()) {
    int lineNumber = pair.first;
    std::string varName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluatePrint(PKBManager& pkbManager) {
  PKBQueryArg printSynonymArg(PqlArgument(ArgumentType::SYNONYM, PRINT_SYNONYM_LITERAL, designEntityType));
  PKBQueryArg varSynonymArg(PqlArgument(ArgumentType::SYNONYM, VARIABLE_SYNONYM_LITERAL, DesignEntityType::VARIABLE));
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getRelationship(RelationshipType::USES, printSynonymArg, varSynonymArg);
  for (auto& pair : result.getLineNumberNamePairs()) {
    int lineNumber = pair.first;
    std::string varName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateCall(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getCallsProc();
  for (auto& pair : result.getLineNumberNamePairs()) {
    int lineNumber = pair.first;
    std::string procName = pair.second;
    resultTable.addRow({ QpsValue(lineNumber), QpsValue(lineNumber), QpsValue(procName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateStatementNumbers(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& stmtNum : result.getLineNumbers()) {
    resultTable.addRow({ QpsValue(stmtNum), QpsValue(stmtNum) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateNames(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& varName : result.getNames()) {
    resultTable.addRow({ QpsValue(varName), QpsValue(varName) });
  }

  return resultTable;
}

spa::QpsResultTable spa::SimpleEvaluator::evaluateConstants(PKBManager& pkbManager) {
  QpsResultTable resultTable;
  resultTable.addHeader(selectSynonym);
  for (auto& attr : pqlAttributesMap[designEntityType]) {
    resultTable.addHeader(selectSynonym + FULL_STOP_LITERAL + attr);
  }

  QueryResult result = pkbManager.getEntity(designEntityType);
  for (auto& constant : result.getNames()) {
    int val = std::stoi(constant);
    resultTable.addRow({ QpsValue(val), QpsValue(val) });
  }

  return resultTable;
}
