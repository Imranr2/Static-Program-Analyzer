#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <memory>
#include <queue>

#include "ContainerStatement.h"
#include "PKB.h"
#include "ProcedureStatement.h"

namespace spa {

struct AffectsNode {
  CFGNode* nodeP;
  std::unordered_set<std::string> usedVariables;
};

struct AffectsNodeEquality {
  bool operator()(const AffectsNode& first, const AffectsNode& second) const;
};

struct AffectsNodeHash {
  size_t operator()(const AffectsNode& node) const;
};

class DesignExtractor {
 private:
  PKBManager& pkbManager;
  std::vector<std::shared_ptr<ProcedureStatement>>& procedureList;
  std::unordered_map<std::string, std::unordered_set<std::string>> procCallMap;

  void buildProcCallMap(std::shared_ptr<ProcedureStatement>procedure);
  void extractDesignAbstraction(std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  void extractParentAbstraction(std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  void extractFollows(std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  void extractFollowsStar(std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  void extractParent(std::shared_ptr<ContainerStatement>& containerStatement);
  void extractParentStar(std::shared_ptr <ContainerStatement>& containerStatement, std::string ancestorLineNum);
  void extractUsesAndModifies(std::vector<std::shared_ptr<ProgramStatement>>& statementList);
  void extractCallsStar();
  void extractUsesAndModifiesProc();
  void extractNestedProcUsesAndModifies();
  void extractCallsModifiesAndUses();

  void dfsCallsStar(std::string parent, std::string child);
  std::vector<std::pair<std::string, std::string>> getResFromPkbHelper(
    std::string procName, std::string synonym, DesignEntityType type, RelationshipType relType);
  void addUsesModifies(std::string relArg, std::vector<std::pair<std::string, std::string>> varUses,
                       std::vector<std::pair<std::string, std::string>> varModifies);
  void addUsesModifies(std::string relArg, std::vector<std::pair<std::string, std::string>> varUses,
                       std::vector<std::pair<std::string, std::string>> varModifies, bool isCallStmt);
  void populateAffectsForNode(CFGNode* initial);

 public:
  DesignExtractor(PKBManager& pkbManager, std::vector<std::shared_ptr<ProcedureStatement>>& procedureList);
  void extractRelationship();
  void populateAffects();
  std::unordered_map<int, std::unordered_set<int>> extractNextAffectsStar(
    const std::unordered_map<int, std::unordered_set<int>>& table);
};
}  // namespace spa
