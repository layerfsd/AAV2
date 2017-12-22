#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

typedef enum _TIMER_TYPE {
  NotificationTimer,
  SynchronizationTimer
} TIMER_TYPE;

using T_NtCreateTimer = NTSTATUS(NTAPI*)(__out PHANDLE TimerHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in TIMER_TYPE TimerType);
using T_NtOpenTimer = NTSTATUS(NTAPI*)(__out PHANDLE TimerHandle, __in ACCESS_MASK DesiredAccess, __in POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateTimer TrueNtCreateTimer = nullptr;
static T_NtOpenTimer   TrueNtOpenTimer = nullptr;

NTSTATUS NTAPI MyNtCreateTimer(__out PHANDLE TimerHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in TIMER_TYPE TimerType)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE(RuleType::TIMER, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateTimer(TimerHandle, DesiredAccess, ObjectAttributes, TimerType);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenTimer(__out PHANDLE TimerHandle, __in ACCESS_MASK DesiredAccess, __in POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::TIMER, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtOpenTimer(TimerHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtTimerFunctions()
{
  if (!TrueNtCreateTimer)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateTimer", reinterpret_cast<PVOID*>(&TrueNtCreateTimer), &MyNtCreateTimer);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenTimer", reinterpret_cast<PVOID*>(&TrueNtOpenTimer), &MyNtOpenTimer);
  }
}
