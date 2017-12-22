#pragma once

namespace Cloudhouse
{
  class SharedMemory
  {
  public:
    SharedMemory();
    ~SharedMemory();

    LPVOID Create(CString const& name, DWORD const& fileSize);

    LPVOID Open(CString const& name);

  private:
    LPVOID MapView();

    LPVOID _baseAddress;
    DWORD _fileSize;
    HANDLE _fileMapping;
    HANDLE _mapView;
  };
}
