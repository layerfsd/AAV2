#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

using T_NtCreateDirectoryObject = NTSTATUS(NTAPI*)(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);
using T_NtOpenDirectoryObject = NTSTATUS(NTAPI*)(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes);

static T_NtCreateDirectoryObject TrueNtCreateDirectoryObject = nullptr;
static T_NtOpenDirectoryObject   TrueNtOpenDirectoryObject = nullptr;

NTSTATUS NTAPI MyNtCreateDirectoryObject(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE(RuleType::DIRECTORY_OBJECT, ObjectAttributes))
  {
    objectAttributes.Redirect();
  }

  ntStatus = TrueNtCreateDirectoryObject(FileHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtOpenDirectoryObject(OUT PHANDLE FileHandle, IN ACCESS_MASK DesiredAccess, IN POBJECT_ATTRIBUTES ObjectAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::DIRECTORY_OBJECT, ObjectAttributes))
  {
    objectAttributes.Redirect();
  }

  ntStatus = TrueNtOpenDirectoryObject(FileHandle, DesiredAccess, ObjectAttributes);

  // LOG HERE

  return ntStatus;
}

void HookNtDirectoryObjectFunctions()
{
  if (!TrueNtCreateDirectoryObject)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateDirectoryObject", reinterpret_cast<PVOID*>(&TrueNtCreateDirectoryObject), &MyNtCreateDirectoryObject);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtOpenDirectoryObject", reinterpret_cast<PVOID*>(&TrueNtOpenDirectoryObject), &MyNtOpenDirectoryObject);
  }
}