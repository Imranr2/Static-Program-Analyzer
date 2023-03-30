#include "CFGStorage.h"
#include "PKBQueryTypes.h"
#include "Literal.h"

#include <string>
#include <vector>
#include <utility>
#include <unordered_set>

bool spa::CFGStorage::popDummyNode(int lineNumber, RelationshipStorage& relationshipStorage) {
  if (lineNumber == DUMMY_NODE_VAL) {
    return true;
  }
  CFGNode& dummyNode = cfgNodeTable[DUMMY_NODE_VAL];
  bool isAddEdge = true;

  for (auto& incomingNode : dummyNode.getIncomingEdges()) {
    isAddEdge = isAddEdge && addEdge(incomingNode->getLineNumber(), lineNumber, relationshipStorage);
  }
  for (auto& node : dummyNode.getIncomingEdges()) {
    node->removeOutgoingEdge(&dummyNode);
  }
  cfgNodeTable[DUMMY_NODE_VAL] = CFGNode();
  return isAddEdge;
}

bool spa::CFGStorage::addCfgNode(int lineNumber, spa::CFGNode cfgNode) {
  if (cfgNodeTable.find(lineNumber) != cfgNodeTable.end()) {
    return false;
  }
  cfgNodeTable.insert({ lineNumber, cfgNode });
  return true;
}

bool spa::CFGStorage::addCfgEndNode(int lineNumber) {
  if (cfgEndNodes.find(lineNumber) != cfgEndNodes.end()) {
    return false;
  }
  cfgEndNodes.insert(lineNumber);
  return true;
}

bool spa::CFGStorage::addEdge(int lineNumberOne, int lineNumberTwo, RelationshipStorage& relationshipStorage) {
  if (lineNumberOne == DUMMY_NODE_VAL) {
    popDummyNode(lineNumberTwo, relationshipStorage);
    return true;
  }
  CFGNode& nodeOne = cfgNodeTable[lineNumberOne];
  CFGNode& nodeTwo = cfgNodeTable[lineNumberTwo];
  nodeOne.addOutgoingEdge(&nodeTwo);
  nodeTwo.addIncomingEdge(&nodeOne);
  bool isAddNext = relationshipStorage.addNext(std::to_string(lineNumberOne), std::to_string(lineNumberTwo));
  return isAddNext;
}

bool spa::CFGStorage::addModifiedVariable(int lineNumber, std::string varName) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  CFGNode& node = cfgNodeTable[lineNumber];
  node.addModifiedVariable(varName);
  return true;
}

bool spa::CFGStorage::addUsesVariable(int lineNumber, std::string varName) {
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return false;
  }

  CFGNode& node = cfgNodeTable[lineNumber];
  node.addUsesVariable(varName);
  return true;
}

spa::QueryResult spa::CFGStorage::getCfgNode(int lineNumber) {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<spa::CFGNode*> cfgNodes;
  queryResult.setCfgNodes(cfgNodes);
  if (cfgNodeTable.find(lineNumber) == cfgNodeTable.end()) {
    return queryResult;
  }

  cfgNodes.push_back(&cfgNodeTable[lineNumber]);
  queryResult.setCfgNodes(cfgNodes);
  return queryResult;
}

spa::QueryResult spa::CFGStorage::getCfgEndNodes() {
  QueryResult queryResult;
  queryResult.setQueryResultType(TUPLE);

  std::vector<CFGNode*> cfgEnd;

  for (auto lineNo : cfgEndNodes) {
    cfgEnd.push_back(&cfgNodeTable[lineNo]);
  }

  queryResult.setCfgNodes(cfgEnd);
  return queryResult;
}

void spa::CFGStorage::setCfgNodeTable(std::unordered_map<int, spa::CFGNode> cfgNodeTable) {
  this->cfgNodeTable = cfgNodeTable;
}
