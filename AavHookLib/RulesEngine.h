#pragma once

#define CHECK_RULE1(rule) true
#define CHECK_RULE2(rule,path) true

enum class RuleType;
  
class RuleEngine
{
public:
  bool Match(RuleType const& ruleType);
  bool Match(RuleType const& ruleType, POBJECT_ATTRIBUTES objectAttributes);
  bool Match(RuleType const& ruleType, PUNICODE_STRING unicodeString);
  bool Match(RuleType const& ruleType, LPCTSTR szPath);

};