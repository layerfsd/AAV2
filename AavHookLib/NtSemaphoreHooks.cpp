#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

using T_NtCreateSemaphore = NTSTATUS(NTAPI*)(__out PHANDLE SemaphoreHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in LONG InitialCount, __in LONG MaximumCount);
using T_NtOpenSemaphore = NTSTATUS(NTAPI*)(__out PHANDLE SemaphoreHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateSemaphore TrueNtCreateSemaphore = nullptr;
static T_NtOpenSemaphore   TrueNtOpenSemaphore = nullptr;

using T_ = NTSTATUS(NTAPI*)();
NTSTATUS NTAPI MyNtCreateSemaphore(__out PHANDLE SemaphoreHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in LONG InitialCount, __in LONG MaximumCount)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE2(RuleType::SEMAPHORE, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateSemaphore(SemaphoreHandle, DesiredAccess, ObjectAttributes, InitialCount, MaximumCount);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenSemaphore(__out PHANDLE SemaphoreHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE2(RuleType::SEMAPHORE, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtOpenSemaphore(SemaphoreHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtSemaphoreFunctions()
{
  if (!TrueNtCreateSemaphore)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateSemaphore", reinterpret_cast<PVOID*>(&TrueNtCreateSemaphore), &MyNtCreateSemaphore);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenSemaphore", reinterpret_cast<PVOID*>(&TrueNtOpenSemaphore), &MyNtOpenSemaphore);
  }
}