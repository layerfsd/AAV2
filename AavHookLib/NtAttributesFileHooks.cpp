#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"

typedef struct _FILE_BASIC_INFORMATION {
  LARGE_INTEGER CreationTime;
  LARGE_INTEGER LastAccessTime;
  LARGE_INTEGER LastWriteTime;
  LARGE_INTEGER ChangeTime;
  ULONG FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct _FILE_NETWORK_OPEN_INFORMATION
{
  LARGE_INTEGER CreationTime;
  LARGE_INTEGER LastAccessTime;
  LARGE_INTEGER LastWriteTime;
  LARGE_INTEGER ChangeTime;
  LARGE_INTEGER AllocationSize;
  LARGE_INTEGER EndOfFile;
  ULONG FileAttributes;
} FILE_NETWORK_OPEN_INFORMATION, *PFILE_NETWORK_OPEN_INFORMATION;

using T_NtQueryAttributesFile = NTSTATUS(NTAPI*)(const POBJECT_ATTRIBUTES, PFILE_BASIC_INFORMATION);
using T_NtQueryFullAttributesFile = NTSTATUS(NTAPI*)(const POBJECT_ATTRIBUTES, PFILE_NETWORK_OPEN_INFORMATION);

static T_NtQueryAttributesFile TrueNtQueryAttributesFile = nullptr;
static T_NtQueryFullAttributesFile TrueNtQueryFullAttributesFile = nullptr;

NTSTATUS NTAPI MyNtQueryAttributesFile(const POBJECT_ATTRIBUTES ObjectAttributes, PFILE_BASIC_INFORMATION FileInformation)
{
  MyObjectAttributes objectAttributes(const_cast<POBJECT_ATTRIBUTES>(ObjectAttributes));

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if(CHECK_RULE(RuleType::ATTRIBUTES_FILE, ObjectAttributes))
  {
    objectAttributes.Redirect();
  }

  ntStatus = TrueNtQueryAttributesFile(ObjectAttributes, FileInformation);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtQueryFullAttributesFile(const POBJECT_ATTRIBUTES ObjectAttributes, PFILE_NETWORK_OPEN_INFORMATION FileNetworkOpenInformation)
{
  MyObjectAttributes objectAttributes(const_cast<POBJECT_ATTRIBUTES>(ObjectAttributes));

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::ATTRIBUTES_FILE, ObjectAttributes))
  {
    objectAttributes.Redirect();
  }

  ntStatus = TrueNtQueryFullAttributesFile(ObjectAttributes, FileNetworkOpenInformation);

  // LOG HERE

  return ntStatus;
}

void HookNtAttributesFileFunctions()
{
  if (!TrueNtQueryAttributesFile)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtQueryAttributesFile", reinterpret_cast<PVOID*>(&TrueNtQueryAttributesFile), &MyNtQueryAttributesFile);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtQueryFullAttributesFile", reinterpret_cast<PVOID*>(&TrueNtQueryFullAttributesFile), &MyNtQueryFullAttributesFile);
  }
}
