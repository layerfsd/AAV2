#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

using T_NtCreateMutant = NTSTATUS(NTAPI*)(__out PHANDLE MutantHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in BOOLEAN InitialOwner);
using T_NtOpenMutant   = NTSTATUS(NTAPI*)(__out PHANDLE MutantHandle, __in ACCESS_MASK DesiredAccess, __in POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateMutant TrueNtCreateMutant = nullptr;
static T_NtOpenMutant   TrueNtOpenMutant = nullptr;

NTSTATUS NTAPI MyNtCreateMutant(__out PHANDLE MutantHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in BOOLEAN InitialOwner)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE2(RuleType::MUTANT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenMutant(__out PHANDLE MutantHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE2(RuleType::MUTANT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtOpenMutant(MutantHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtMutantFunctions()
{
  if (!TrueNtCreateMutant)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateMutant", reinterpret_cast<PVOID*>(&TrueNtCreateMutant), &MyNtCreateMutant);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenMutant", reinterpret_cast<PVOID*>(&TrueNtOpenMutant), &MyNtOpenMutant);
  }
}