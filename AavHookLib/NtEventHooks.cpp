#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

typedef enum _EVENT_TYPE {
  NotificationEvent,
  SynchronizationEvent
} EVENT_TYPE;

using T_NtCreateEvent = NTSTATUS(NTAPI*)(__out PHANDLE EventHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in EVENT_TYPE EventType, __in BOOLEAN InitialState);
using T_NtOpenEvent = NTSTATUS(NTAPI*)(__out PHANDLE EventHandle, __in ACCESS_MASK DesiredAccess, __in POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateEvent TrueNtCreateEvent = nullptr;
static T_NtOpenEvent   TrueNtOpenEvent = nullptr;

NTSTATUS NTAPI MyNtCreateEvent(__out PHANDLE EventHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in EVENT_TYPE EventType, __in BOOLEAN InitialState)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE(RuleType::EVENT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateEvent(EventHandle, DesiredAccess, ObjectAttributes, EventType, InitialState);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenEvent(__out PHANDLE EventHandle, __in ACCESS_MASK DesiredAccess, __in POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::EVENT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtOpenEvent(EventHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtEventFunctions()
{
  if (!TrueNtCreateEvent)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateEvent", reinterpret_cast<PVOID*>(&TrueNtCreateEvent), &MyNtCreateEvent);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenEvent", reinterpret_cast<PVOID*>(&TrueNtOpenEvent), &MyNtOpenEvent);
  }
}