#include "PqlSemanticChecker.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <cassert>

#include "Literal.h"

bool spa::PqlSemanticChecker::isSemanticallyValid(ParsedQuery& parsedQuery) {
  for (auto& p : parsedQuery.getDeclarationsCount()) {
    if (p.second > 1) {
      return false;
    }
  }
  for (auto& p : parsedQuery.getUsedDeclarations()) {
    if (p.second == UNKNOWN) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getSuchThatClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getPatternClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  for (auto& clause : parsedQuery.getWithClauses()) {
    if (!isValid(clause)) {
      return false;
    }
  }
  return true;
}

bool spa::PqlSemanticChecker::isValid(SuchThatClause& suchThatClause) {
  RelationshipType designAbstraction = suchThatClause.getDesignAbstraction();
  PqlArgument firstArg = suchThatClause.getFirstArg();
  PqlArgument secondArg = suchThatClause.getSecondArg();
  switch (designAbstraction) {
  case MODIFIES:
    return checkModifiesArguments(firstArg, secondArg);
  case USES:
    return checkUsesArguments(firstArg, secondArg);
  case CALLS:
  case CALLS_STAR:
    return checkCallsArguments(firstArg, secondArg);
  case PARENT:
  case PARENT_STAR:
  case FOLLOWS:
  case FOLLOWS_STAR:
  case NEXT:
  case NEXT_STAR:
  case AFFECTS:
  case AFFECTS_STAR:
    return checkStmtRefArguments(firstArg, secondArg);
  }
  return false;
}

bool spa::PqlSemanticChecker::isValid(PatternClause& patternClause) {
  PqlArgument& firstArg = patternClause.getFirstArg();
  ArgumentType firstArgType = patternClause.getFirstArgType();
  if (firstArgType == LINE_NO) {
    return false;
  }

  if (firstArgType == SYNONYM) {
    std::optional<DesignEntityType> deTypeOpt = firstArg.getDesignEntity();
    assert(deTypeOpt.has_value());
    DesignEntityType deType = deTypeOpt.value();
    if (deType != VARIABLE) {
      return false;
    }
  }

  DesignEntityType entityType = patternClause.getSynonymType();
  switch (entityType) {
    case ASSIGN:
      if (patternClause.getNumArgs() != 2) {
        return false;
      }
      break;
    case IF:
      if (patternClause.getNumArgs() != 3 || patternClause.getPatternType() != ANY) {
        return false;
      }
      break;
    case WHILE:
      if (patternClause.getNumArgs() != 2 || patternClause.getPatternType() != ANY) {
        return false;
      }
      break;
    default:
      return false;
  }
  return true;
}

bool spa::PqlSemanticChecker::isValid(WithClause& withClause) {
  WithArgument firstArg = withClause.getFirstArg();
  WithArgument secondArg = withClause.getSecondArg();
  QpsValueType firstArgType = getWithArgumentType(firstArg);
  QpsValueType secondArgType = getWithArgumentType(secondArg);

  return firstArgType == secondArgType;
}

bool spa::PqlSemanticChecker::checkModifiesArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD) {
    return false;
  }

  if (firstArgType == SYNONYM) {
    std::optional<DesignEntityType> deTypeOpt = firstArg.getDesignEntity();
    assert(deTypeOpt.has_value());
    DesignEntityType deType = deTypeOpt.value();
    if (deType == VARIABLE || deType == PRINT || deType == CONSTANT) {
      return false;
    }
  }

  ArgumentType secondArgType = secondArg.getType();
  if (secondArgType == SYNONYM && secondArg.getDesignEntity().value() != VARIABLE) {
    return false;
  }

  return true;
}

bool spa::PqlSemanticChecker::checkUsesArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  ArgumentType firstArgType = firstArg.getType();
  if (firstArgType == WILDCARD) {
    return false;
  }
  if (firstArgType == SYNONYM) {
    std::optional<DesignEntityType> deTypeOpt = firstArg.getDesignEntity();
    assert(deTypeOpt.has_value());
    DesignEntityType deType = deTypeOpt.value();
    if (deType == VARIABLE || deType == READ || deType == CONSTANT) {
      return false;
    }
  }

  ArgumentType secondArgType = secondArg.getType();

  if (secondArgType == SYNONYM && secondArg.getDesignEntity().value() != VARIABLE) {
    return false;
  }

  return true;
}

bool spa::PqlSemanticChecker::checkStmtRefArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();

    if (argType == SYNONYM) {
      std::optional<DesignEntityType> deTypeOpt = arg.getDesignEntity();
      assert(deTypeOpt.has_value());
      DesignEntityType deType = deTypeOpt.value();
      if (deType == PROCEDURE || deType == VARIABLE || deType == CONSTANT) {
        return false;
      }
    }
  }

  return true;
}

bool spa::PqlSemanticChecker::checkCallsArguments(PqlArgument& firstArg, PqlArgument& secondArg) {
  for (PqlArgument arg : { firstArg, secondArg }) {
    ArgumentType argType = arg.getType();

    if (argType == SYNONYM) {
      std::optional<DesignEntityType> deTypeOpt = arg.getDesignEntity();
      assert(deTypeOpt.has_value());
      DesignEntityType deType = deTypeOpt.value();
      if (deType != PROCEDURE) {
        return false;
      }
    }
  }

  return true;
}

spa::QpsValueType spa::PqlSemanticChecker::getWithArgumentType(WithArgument& withArgument) {
  std::unordered_map<std::string, QpsValueType> attributeNameTypeMap = {
    {STMT_NUM_ATTR_LITERAL, QpsValueType::INTEGER},
    {VALUE_ATTR_LITERAL, QpsValueType::INTEGER},
    {PROC_NAME_ATTR_LITERAL, QpsValueType::STRING},
    {VAR_NAME_ATTR_LITERAL, QpsValueType::STRING},
  };

  if (withArgument.getType() == WithArgumentType::WITH_VALUE) {
    return withArgument.getValue().getType();
  }

  std::string attribute = withArgument.getAttribute();
  return attributeNameTypeMap[attribute.substr(attribute.find('.') + 1)];
}
