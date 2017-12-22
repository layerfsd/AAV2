#pragma once

enum class RuleType
{
  INVALID,

  // NT Security projects
  ATTRIBUTES_FILE,
  DIRECOTRY_OBJECT,
  EVENT,
  PORT,
  SECTION,
  SEMAPHORE,
  TIMER,
  MUTANT,

  // COM
  OUT_OF_PROCESS_COM,
};
