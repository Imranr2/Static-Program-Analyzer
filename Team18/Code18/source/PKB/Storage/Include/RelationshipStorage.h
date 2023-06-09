#pragma once

#include "QueryResult.h"
#include "PKBQueryArg.h"
#include "CFGNode.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <unordered_set>

namespace spa {
class RelationshipStorage {
 private:
  std::unordered_map<int, int> followsTable;
  std::unordered_map<int, std::unordered_set<int>> followsStarTable;
  std::unordered_map<int, std::unordered_set<int>> parentTable;
  std::unordered_map<int, std::unordered_set<int>> parentStarTable;
  std::unordered_map<int, std::unordered_set<std::string>> usesTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> usesProcTable;
  std::unordered_map<int, std::unordered_set<std::string>> modifiesTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> modifiesProcTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> callsTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> callsStarTable;
  std::unordered_map<int, std::unordered_set<int>> nextTable;
  std::unordered_map<int, std::unordered_set<int>> nextStarTable;
  std::unordered_map<int, std::unordered_set<int>> affectsTable;
  std::unordered_map<int, std::unordered_set<int>> affectsStarTable;
  std::unordered_map<std::string, std::unordered_set<int>> callsContainerParentsTable;
  std::unordered_map<int, std::string> callsProcTable;
  std::unordered_map<int, StatementType> statementTypeTable;

 public:
  bool addStatementType(std::string lineNo, StatementType statementType);
  QueryResult getStatements(std::optional<StatementType> statementType);

  bool addFollows(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addFollowsStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getFollowsStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getFollowsStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addParent(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addParentStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getParentStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getParentStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addUses(std::string lineNo, std::string varName);
  QueryResult getUsesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addUsesProc(std::string procName, std::string varName);
  QueryResult getUsesProcNameVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesProcNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesProcNameVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesProcedureVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getUsesProcedureVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addModifies(std::string lineNo, std::string varName);
  QueryResult getModifiesLineVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesLineVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesStmtVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addModifiesProc(std::string procName, std::string varName);
  QueryResult getModifiesProcNameVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesProcNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesProcNameVar(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesProcedureVarName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getModifiesProcedureVar(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCalls(std::string firstProc, std::string secondProc);
  QueryResult getCallsNameName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsNameProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsUnderscoreProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsProcedureProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCallsStar(std::string firstProc, std::string secondProc);
  QueryResult getCallsStarNameName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarNameUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarNameProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarUnderscoreProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureName(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getCallsStarProcedureProcedure(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addNext(std::string firstLineNo, std::string secondLineNo);
  QueryResult getNextLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addNextStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getNextStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getNextStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addAffects(std::string firstLineNo, std::string secondLineNo);
  QueryResult getAffectsLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addAffectsStar(std::string firstLineNo, std::string secondLineNo);
  QueryResult getAffectsStarLineLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarLineStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarStatementLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarLineUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarUnderscoreLine(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarStatementStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarStatementUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarUnderscoreStatement(PKBQueryArg firstArg, PKBQueryArg secondArg);
  QueryResult getAffectsStarUnderscoreUnderscore(PKBQueryArg firstArg, PKBQueryArg secondArg);

  bool addCallsContainerParent(std::string procName, std::string lineNo);
  QueryResult getCallsContainerParent(std::string procName);

  bool addCallsProc(int lineNumber, std::string procName);
  QueryResult getCallsProc();

  bool isNextStarEmpty();
  bool isAffectsEmpty();
  bool isAffectsStarEmpty();
  QueryResult getNextTable();
  QueryResult getAffectsTable();

  void setFollowsTable(std::unordered_map<int, int> followsTable);
  void setFollowsStarTable(std::unordered_map<int, std::unordered_set<int>> followsStarTable);
  void setParentTable(std::unordered_map<int, std::unordered_set<int>> parentTable);
  void setParentStarTable(std::unordered_map<int, std::unordered_set<int>> parentStarTable);
  void setUsesTable(std::unordered_map<int, std::unordered_set<std::string>> usesTable);
  void setUsesProcTable(std::unordered_map<std::string, std::unordered_set<std::string>> usesProcTable);
  void setModifiesTable(std::unordered_map<int, std::unordered_set<std::string>> modifiesTable);
  void setModifiesProcTable(std::unordered_map<std::string, std::unordered_set<std::string>> modifiesProcTable);
  void setCallsTable(std::unordered_map<std::string, std::unordered_set<std::string>> callsTable);
  void setCallsStarTable(std::unordered_map<std::string, std::unordered_set<std::string>> callsStarTable);
  void setNextTable(std::unordered_map<int, std::unordered_set<int>> nextTable);
  void setNextStarTable(std::unordered_map<int, std::unordered_set<int>> nextStarTable);
  void setAffectsTable(std::unordered_map<int, std::unordered_set<int>> affectsTable);
  void setAffectsStarTable(std::unordered_map<int, std::unordered_set<int>> affectsStarTable);
  void setCallsContainerParentsTable(std::unordered_map<std::string,
                                                        std::unordered_set<int>> callsContainerParentsTable);
  void setCallsProcTable(std::unordered_map<int, std::string> statementTypeTable);
  void setStatementTypeTable(std::unordered_map<int, StatementType> statementTypeTable);
};
}  // namespace spa
