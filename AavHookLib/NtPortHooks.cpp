#include "stdafx.h"
#include "MyObjectAttributes.h"
#include "../Cloudhouse.Detours/Cloudhouse.Detour.h"
#include "RulesEngine.h"
#include "MyUnicodeString.h"

typedef struct _PORT_VIEW {
  ULONG Length;
  HANDLE SectionHandle;
  ULONG SectionOffset;
  SIZE_T ViewSize;
  PVOID ViewBase;
  PVOID ViewRemoteBase;
} PORT_VIEW, *PPORT_VIEW;

typedef struct _REMOTE_PORT_VIEW {
  ULONG Length;
  SIZE_T ViewSize;
  PVOID ViewBase;
} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;

typedef struct _ALPC_PORT_ATTRIBUTES
{
  ULONG Flags;
  SECURITY_QUALITY_OF_SERVICE SecurityQos;
  SIZE_T MaxMessageLength;
  SIZE_T MemoryBandwidth;
  SIZE_T MaxPoolUsage;
  SIZE_T MaxSectionSize;
  SIZE_T MaxViewSize;
  SIZE_T MaxTotalSectionSize;
  ULONG DupObjectTypes;
#ifdef _M_X64
  ULONG Reserved;
#endif
} ALPC_PORT_ATTRIBUTES, *PALPC_PORT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
  HANDLE UniqueProcess;
  HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef short CSHORT;

typedef struct _QUAD
{
  double DoNotUseThisField;
} QUAD, *PQUAD, UQUAD, *PUQUAD;

typedef struct _PORT_MESSAGE
{
  union
  {
    struct
    {
      CSHORT DataLength;
      CSHORT TotalLength;
    } s1;
    ULONG Length;
  } u1;
  union
  {
    struct
    {
      CSHORT Type;
      CSHORT DataInfoOffset;
    } s2;
    ULONG ZeroInit;
  } u2;
  union
  {
    CLIENT_ID ClientId;
    QUAD DoNotUseThisField;
  };
  ULONG MessageId;
  union
  {
    SIZE_T ClientViewSize; // only valid for LPC_CONNECTION_REQUEST messages
    ULONG CallbackId; // only valid for LPC_REQUEST messages
  };
} PORT_MESSAGE, *PPORT_MESSAGE;

typedef struct _ALPC_MESSAGE_ATTRIBUTES
{
  ULONG AllocatedAttributes;
  ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, *PALPC_MESSAGE_ATTRIBUTES;

using T_NtCreatePort = NTSTATUS(NTAPI*)(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in ULONG MaxConnectionInfoLength, __in ULONG MaxMessageLength, __in_opt ULONG MaxPoolUsage);
using T_NtCreateWaitablePort = NTSTATUS(NTAPI*)(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in ULONG MaxConnectionInfoLength, __in ULONG MaxMessageLength, __in_opt ULONG MaxPoolUsage);
using T_NtSecureConnectPort = NTSTATUS(NTAPI*)(__out PHANDLE PortHandle, __in PUNICODE_STRING PortName, __in PSECURITY_QUALITY_OF_SERVICE SecurityQos, __inout_opt PPORT_VIEW ClientView, __in_opt PSID RequiredServerSid, __inout_opt PREMOTE_PORT_VIEW ServerView, __out_opt PULONG MaxMessageLength, __inout_opt PVOID ConnectionInformation, __inout_opt PULONG ConnectionInformationLength);
using T_NtConnectPort = NTSTATUS(NTAPI*)(__out PHANDLE PortHandle, __in PUNICODE_STRING PortName, __in PSECURITY_QUALITY_OF_SERVICE SecurityQos, __inout_opt PPORT_VIEW ClientView, __inout_opt PREMOTE_PORT_VIEW ServerView, __out_opt PULONG MaxMessageLength, __inout_opt PVOID ConnectionInformation, __inout_opt PULONG ConnectionInformationLength);
using T_NtAlpcCreatePort = NTSTATUS(NTAPI*)(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in_opt PALPC_PORT_ATTRIBUTES PortAttributes);
using T_NtAlpcConnectPort = NTSTATUS(NTAPI*)(__out PHANDLE  PortHandle, __in PUNICODE_STRING  PortName, __in POBJECT_ATTRIBUTES  ObjectAttributes, __in_opt PALPC_PORT_ATTRIBUTES  PortAttributes, __in ULONG  Flags, __in_opt PSID  RequiredServerSid, __inout PPORT_MESSAGE  ConnectionMessage, __inout_opt PULONG  BufferLength, __inout_opt PALPC_MESSAGE_ATTRIBUTES  OutMessageAttributes, __inout_opt PALPC_MESSAGE_ATTRIBUTES  InMessageAttributes, __in_opt PLARGE_INTEGER  Timeout);

static T_NtCreatePort TrueNtCreatePort = nullptr;
static T_NtCreateWaitablePort TrueNtCreateWaitablePort = nullptr;
static T_NtSecureConnectPort TrueNtSecureConnectPort = nullptr;
static T_NtConnectPort TrueNtConnectPort = nullptr;
static T_NtAlpcCreatePort TrueNtAlpcCreatePort = nullptr;
static T_NtAlpcConnectPort TrueNtAlpcConnectPort = nullptr;

NTSTATUS NTAPI MyNtCreatePort(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in ULONG MaxConnectionInfoLength, __in ULONG MaxMessageLength, __in_opt ULONG MaxPoolUsage)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreatePort(PortHandle, ObjectAttributes, MaxConnectionInfoLength, MaxMessageLength, MaxPoolUsage);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtCreateWaitablePort(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in ULONG MaxConnectionInfoLength, __in ULONG MaxMessageLength, __in_opt ULONG MaxPoolUsage)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtCreateWaitablePort(PortHandle, ObjectAttributes, MaxConnectionInfoLength, MaxMessageLength, MaxPoolUsage);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtSecureConnectPort(__out PHANDLE PortHandle, __in PUNICODE_STRING PortName, __in PSECURITY_QUALITY_OF_SERVICE SecurityQos, __inout_opt PPORT_VIEW ClientView, __in_opt PSID RequiredServerSid, __inout_opt PREMOTE_PORT_VIEW ServerView, __out_opt PULONG MaxMessageLength, __inout_opt PVOID ConnectionInformation, __inout_opt PULONG ConnectionInformationLength)
{
  MyUnicodeString portName(PortName);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    portName.Virtualize();
  }

  ntStatus = TrueNtSecureConnectPort(PortHandle, PortName, SecurityQos, ClientView, RequiredServerSid, ServerView, MaxMessageLength, ConnectionInformation, ConnectionInformationLength);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtConnectPort(__out PHANDLE PortHandle, __in PUNICODE_STRING PortName, __in PSECURITY_QUALITY_OF_SERVICE SecurityQos, __inout_opt PPORT_VIEW ClientView, __inout_opt PREMOTE_PORT_VIEW ServerView, __out_opt PULONG MaxMessageLength, __inout_opt PVOID ConnectionInformation, __inout_opt PULONG ConnectionInformationLength)
{
  MyUnicodeString portName(PortName);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    portName.Virtualize();
  }

  ntStatus = TrueNtConnectPort(PortHandle, PortName, SecurityQos, ClientView, ServerView, MaxMessageLength, ConnectionInformation, ConnectionInformationLength);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtAlpcCreatePort(__out PHANDLE PortHandle, __in POBJECT_ATTRIBUTES ObjectAttributes, __in_opt PALPC_PORT_ATTRIBUTES PortAttributes)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtAlpcCreatePort(PortHandle, ObjectAttributes, PortAttributes);

  // LOG HERE

  return ntStatus;
}

NTSTATUS NTAPI MyNtAlpcConnectPort(__out PHANDLE  PortHandle, __in PUNICODE_STRING  PortName, __in POBJECT_ATTRIBUTES  ObjectAttributes, __in_opt PALPC_PORT_ATTRIBUTES  PortAttributes, __in ULONG  Flags, __in_opt PSID  RequiredServerSid, __inout PPORT_MESSAGE  ConnectionMessage, __inout_opt PULONG  BufferLength, __inout_opt PALPC_MESSAGE_ATTRIBUTES  OutMessageAttributes, __inout_opt PALPC_MESSAGE_ATTRIBUTES  InMessageAttributes, __in_opt PLARGE_INTEGER  Timeout)
{
  MyObjectAttributes objectAttributes(ObjectAttributes);

  NTSTATUS ntStatus = STATUS_SUCCESS;

  if (CHECK_RULE(RuleType::PORT, ObjectAttributes))
  {
    objectAttributes.Virtualize();
  }

  ntStatus = TrueNtAlpcConnectPort(PortHandle, PortName, ObjectAttributes, PortAttributes, Flags, RequiredServerSid, ConnectionMessage, BufferLength, OutMessageAttributes, InMessageAttributes, Timeout);

  // LOG HERE

  return ntStatus;
}

void HookNtPortFunctions()
{
  if (!TrueNtCreatePort)
  {
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreatePort", reinterpret_cast<PVOID*>(&TrueNtCreatePort), &MyNtCreatePort);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtCreateWaitablePort", reinterpret_cast<PVOID*>(&TrueNtCreateWaitablePort), &MyNtCreateWaitablePort);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtSecureConnectPort", reinterpret_cast<PVOID*>(&TrueNtSecureConnectPort), &MyNtSecureConnectPort);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtConnectPort", reinterpret_cast<PVOID*>(&TrueNtConnectPort), &MyNtConnectPort);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtAlpcCreatePort", reinterpret_cast<PVOID*>(&TrueNtAlpcCreatePort), &MyNtAlpcCreatePort);
    Cloudhouse::Detour::Hook("ntdll.dll", "NtAlpcConnectPort", reinterpret_cast<PVOID*>(&TrueNtAlpcConnectPort), &MyNtAlpcConnectPort);
  }
}