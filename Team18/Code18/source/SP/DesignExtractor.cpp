#include "DesignExtractor.h"

#include <vector>
#include <string>

#include "ContainerStatement.h"
#include "NonContainerStatement.h"
#include "ProcedureStatement.h"
#include "SpCyclicValidator.h"

spa::DesignExtractor::DesignExtractor(PKBManager& pkbManager,
                                      std::vector<ProcedureStatement>& procedureList) :
  pkbManager(pkbManager), procedureList(procedureList) {
    for (auto& procedure : procedureList) {
        auto statements = procedure.getStatementLst();
        for (auto& statement : statements) {
            if (dynamic_cast<spa::CallStatement*>(statement)) {
                auto callStatement = dynamic_cast<spa::CallStatement*>(statement);
                procedure.addCalledVars(callStatement->getVariableName());
            }
        }
        procCallMap.emplace(procedure.getProcedureVarToken().getValue(),
            procedure.getCalledVars());
    }
}

void spa::DesignExtractor::extractRelationship() {
  spa::SpCyclicValidator cyclicValidator(procCallMap);
  if (cyclicValidator.validateCyclic()) {
    exit(1);
  }
  for (ProcedureStatement& procedure : procedureList) {
    pkbManager.addEntity(PROCEDURE, procedure.getProcedureVarToken().getValue());
    std::vector<ProgramStatement*> statementList = procedure.getStatementLst();
    extractDesignAbstraction(statementList);
  }
}

void spa::DesignExtractor::extractDesignAbstraction(std::vector<ProgramStatement*> statementList) {
  extractFollows(statementList);
  extractFollowsStar(statementList);
  extractParentAbstraction(statementList);
  extractUsesAndModifies(statementList);
  extractCallsStar();
}

void spa::DesignExtractor::extractParentAbstraction(std::vector<ProgramStatement*> statementList) {
  for (ProgramStatement* statement : statementList) {
    if (dynamic_cast<ContainerStatement*>(statement)) {
      auto containerStatement = dynamic_cast<ContainerStatement*>(statement);
      extractParent(containerStatement);
      extractParentStar(containerStatement,
        std::to_string(containerStatement->getStatementLineNum()));
    }
  }
}

void spa::DesignExtractor::extractFollows(std::vector<ProgramStatement*> statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    if (i + 1 != statementList.size()) {
      std::string followStmtOne = std::to_string(statementList[i]->getStatementLineNum());
      std::string followStmtTwo = std::to_string(statementList[i + 1]->getStatementLineNum());

      if (followStmtOne != "-1" && followStmtTwo != "-1") {
        pkbManager.addRelationship(FOLLOWS, followStmtOne, followStmtTwo);
      }
    }
    if (dynamic_cast<ContainerStatement*>(statementList[i])) {
      ContainerStatement* containerStatement = dynamic_cast<ContainerStatement*>(statementList[
        i]);
      extractFollows(containerStatement->getStatementList());
    }
  }
}

void spa::DesignExtractor::extractFollowsStar(std::vector<ProgramStatement*> statementList) {
  for (unsigned i = 0; i < statementList.size(); i++) {
    if (i + 1 != statementList.size()) {
      for (unsigned j = i + 1; j < statementList.size(); j++) {
        std::string followStarStmtOne = std::to_string(statementList[i]
          ->getStatementLineNum());
        std::string followStarStmtTwo = std::to_string(statementList[j]
          ->getStatementLineNum());
        if (followStarStmtOne != "-1" && followStarStmtTwo != "-1") {
          pkbManager.addRelationship(FOLLOWS_STAR, followStarStmtOne, followStarStmtTwo);
        }
      }
    }
    if (dynamic_cast<ContainerStatement*>(statementList[i])) {
      ContainerStatement* containerStatement = dynamic_cast<ContainerStatement*>(statementList[
        i]);
      extractFollowsStar(containerStatement->getStatementList());
    }
  }
}

void spa::DesignExtractor::extractParent(ContainerStatement* containerStatement) {
  std::vector<ProgramStatement*> statementList = containerStatement->getStatementList();
  for (ProgramStatement* statement : statementList) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    if (statement->getStatementLineNum() == containerStatement->getStatementLineNum()) {
      continue;
    }
    if (statement->getStatementLineNum() != -1) {
      std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());
      pkbManager.addRelationship(PARENT, parentStmtOne, parentStmtTwo);
      continue;
    }
    auto innerBlockStatements = dynamic_cast<ContainerStatement*>(statement);
    std::vector<ProgramStatement*> innerBlockStmtList = innerBlockStatements->getStatementList();
    for (ProgramStatement* childStatement : innerBlockStmtList) {
      std::string parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT, parentStmtOne, parentStmtTwo);
      if (dynamic_cast<ContainerStatement*>(childStatement)) {
        auto childContainerStmt = dynamic_cast<ContainerStatement*>(childStatement);
        extractParent(childContainerStmt);
      }
    }
  }
}

void spa::DesignExtractor::extractParentStar(ContainerStatement* containerStatement,
                                             std::string ancestorLineNum) {
  std::vector<ProgramStatement*> statementList = containerStatement->getStatementList();
  for (ProgramStatement* statement : statementList) {
    std::string parentStmtOne = std::to_string(containerStatement->getStatementLineNum());
    std::string parentStmtTwo = std::to_string(statement->getStatementLineNum());

    if (parentStmtOne == parentStmtTwo) {
      continue;
    }
    if (parentStmtTwo != "-1") {
      pkbManager.addRelationship(PARENT_STAR, ancestorLineNum, parentStmtTwo);
      continue;
    }

    auto innerBlockStatements = dynamic_cast<ContainerStatement*>(statement);
    std::vector<ProgramStatement*> innerBlockStmtList = innerBlockStatements->getStatementList();

    for (ProgramStatement* childStatement : innerBlockStmtList) {
      parentStmtTwo = std::to_string(childStatement->getStatementLineNum());
      pkbManager.addRelationship(PARENT_STAR, ancestorLineNum, parentStmtTwo);

      if (dynamic_cast<ContainerStatement*>(childStatement)) {
        auto childContainerStmt = dynamic_cast<ContainerStatement*>(childStatement);
        extractParentStar(childContainerStmt, ancestorLineNum);
        extractParentStar(childContainerStmt,
          std::to_string(childStatement->getStatementLineNum()));
      }
    }
  }
}

void spa::DesignExtractor::extractUsesAndModifies(std::vector<ProgramStatement*> statementList) {
  for (auto statement : statementList) {
    statement->processStatement(pkbManager);
  }
}

void spa::DesignExtractor::dfsCallsStar(std::string parent, std::string child) {
    if (procCallMap.find(child) == procCallMap.end()) return;
    for (auto& childChild : procCallMap[child]) {
        pkbManager.addRelationship(CALLS_STAR, parent, childChild);
        dfsCallsStar(parent, childChild);
    }
}

void spa::DesignExtractor::extractCallsStar() {
    for (auto& procedure : procedureList) {
        auto currentProcedure = procedure.getProcedureVarToken().getValue();
        for (auto& directCall : procedure.getCalledVars()) {
            pkbManager.addRelationship(CALLS_STAR, currentProcedure, directCall);
            dfsCallsStar(currentProcedure, directCall);
        }
    }
}
