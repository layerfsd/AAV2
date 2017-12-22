#pragma once

using T_RtlInitUnicodeString = VOID(NTAPI*)(PUNICODE_STRING DestinationString, PCWSTR SourceString);

extern T_RtlInitUnicodeString TrueRtlInitUnicodeString;

void InitRTLFunctions();
