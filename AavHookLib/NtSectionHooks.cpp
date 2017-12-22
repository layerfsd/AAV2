#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

using T_NtCreateSection = NTSTATUS(NTAPI*)(__out PHANDLE SectionHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in_opt PLARGE_INTEGER MaximumSize, __in ULONG SectionPageProtection, __in ULONG AllocationAttributes, __in_opt HANDLE FileHandle);
using T_NtOpenSection = NTSTATUS(NTAPI*)(OUT PHANDLE SectionHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateSection TrueNtCreateSection = nullptr;
static T_NtOpenSection   TrueNtOpenSection = nullptr;

NTSTATUS NTAPI MyNtCreateSection(__out PHANDLE SectionHandle, __in ACCESS_MASK DesiredAccess, __in_opt POBJECT_ATTRIBUTES ObjectAttributes, __in_opt PLARGE_INTEGER MaximumSize, __in ULONG SectionPageProtection, __in ULONG AllocationAttributes, __in_opt HANDLE FileHandle)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE2(RuleType::SECTION, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateSection(SectionHandle, DesiredAccess, ObjectAttributes, MaximumSize, SectionPageProtection, AllocationAttributes, FileHandle);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenSection(OUT PHANDLE SectionHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE2(RuleType::SECTION, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtOpenSection(SectionHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtSectionFunctions()
{
  if (!TrueNtCreateSection)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateSection", reinterpret_cast<PVOID*>(&TrueNtCreateSection), &MyNtCreateSection);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenSection", reinterpret_cast<PVOID*>(&TrueNtOpenSection), &MyNtOpenSection);
  }
}