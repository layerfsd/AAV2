#include "stdafx.h"
#include "RulesEngine.h"
#include "RuleType.h"

bool RuleEngine::Match(RuleType const& ruleType)
{
  UNREFERENCED_PARAMETER(ruleType);
  return true;
}

bool RuleEngine::Match(RuleType const& ruleType, POBJECT_ATTRIBUTES objectAttributes)
{
  UNREFERENCED_PARAMETER(ruleType);
  UNREFERENCED_PARAMETER(objectAttributes);
  return true;
}

bool RuleEngine::Match(RuleType const& ruleType, PUNICODE_STRING unicodeString)
{
  UNREFERENCED_PARAMETER(ruleType);
  UNREFERENCED_PARAMETER(unicodeString);
  return true;
}

bool RuleEngine::Match(RuleType const& ruleType, LPCTSTR szPath)
{
  UNREFERENCED_PARAMETER(ruleType);
  UNREFERENCED_PARAMETER(szPath);
  return true;
}
