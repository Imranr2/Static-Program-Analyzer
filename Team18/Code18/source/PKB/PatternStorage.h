#pragma once

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <string>

#include "QueryResult.h"
#include "PKBQueryArg.h"

namespace spa {
class PatternStorage {
 private:
  std::unordered_map<int, std::pair<std::string, std::string>> assignTable;
  std::unordered_map<int, std::unordered_set<std::string>> patternIfTable;
  std::unordered_map<int, std::unordered_set<std::string>> patternWhileTable;

 public:
  bool addAssign(std::string lineNo, std::string varName, std::string postfixString);
  bool addPatternIf(std::string lineNo, std::string varName);
  bool addPatternWhile(std::string lineNo, std::string varName);
  QueryResult getAssignUnderscore(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssignVar(PKBQueryArg lhs, Pattern rhs);
  QueryResult getAssignVarName(PKBQueryArg lhs, Pattern rhs);

  void setAssignTable(std::unordered_map<int, std::pair<std::string, std::string>> assignTable);
  void setPatternIfTable(std::unordered_map<int, std::unordered_set<std::string>> patternIfTable);
  void setPatternWhileTable(std::unordered_map<int, std::unordered_set<std::string>> patternWhileTable);
};
}  // namespace spa
