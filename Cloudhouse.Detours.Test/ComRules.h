#pragma once
#include "CMike.h"

class COMRules
{
public:
  bool InitializeRules()
  {
    // Add some dummy "rules"
    _guids.emplace_back(IID_IMike);

    return true;
  }


  bool MatchIID(REFCLSID rclsid)
  {
    auto matched = false;

    for (auto const& guid : _guids)
    {
      if (IsEqualGUID(rclsid, guid))
      {
        matched = true;
        break;
      }
    }

    return matched;
  }


private:
  std::vector<GUID> _guids;
};
