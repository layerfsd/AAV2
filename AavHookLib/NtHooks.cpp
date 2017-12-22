#include "stdafx.h"
#include "NtHooks.h"
#include "NtAttributesFileHooks.h"
#include "NtDirectoryObjectHooks.h"
#include "NtEventHooks.h"
#include "NtPortHooks.h"
#include "NtSectionHooks.h"
#include "NtSemaphoreHooks.h"
#include "NtTimerHooks.h"
#include "NtMutantHooks.h"
#include "RTL.h"

void HookNtFunctions()
{
  InitRTLFunctions();
    
  //HookNtAttributesFileFunctions();
  //HookNtDirectoryObjectFunctions();
  HookNtEventFunctions();
  //HookNtPortFunctions();
  //HookNtSectionFunctions();
  //HookNtSemaphoreFunctions();
  //HookNtTimerFunctions();
  //HookNtMutantFunctions();
}
