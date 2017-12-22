#include "stdafx.h"
#include "Cloudhouse.SharedMemory.h"

using namespace Cloudhouse;

SharedMemory::SharedMemory()
  : _baseAddress(nullptr)
  , _fileSize(0)
  , _fileMapping(INVALID_HANDLE_VALUE)
  , _mapView(INVALID_HANDLE_VALUE)
{
}

SharedMemory::~SharedMemory()
{
  if (_baseAddress)
  {
    UnmapViewOfFile(_baseAddress);
  }

  if (_mapView != INVALID_HANDLE_VALUE)
  {
    CloseHandle(_mapView);
  }

  if (_fileMapping)
  {
    CloseHandle(_fileMapping);
  }
}

LPVOID SharedMemory::Create(CString const& name, DWORD const& fileSize)
{
  if (_fileMapping == INVALID_HANDLE_VALUE)
  {
    _fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, fileSize, name);
    _fileSize = fileSize;
  }

  return MapView();
}

LPVOID SharedMemory::Open(CString const& name)
{
  if (_fileMapping == INVALID_HANDLE_VALUE)
  {
    _fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);
  }

  return MapView();
}

LPVOID SharedMemory::MapView()
{
  if (_fileMapping != INVALID_HANDLE_VALUE)
  {
    _baseAddress = MapViewOfFile(_fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, _fileSize);
  }

  return _baseAddress;
}
