#pragma once

#include <vector>

struct AssetManagerNewHandler;
class AssetManagerNewManager
{
public:
  static AssetManagerNewManager *Get();

  void RegisterNewHandler(AssetManagerNewHandler *handler);

  std::vector<AssetManagerNewHandler*> &GetHandlers();
  const std::vector<AssetManagerNewHandler*> &GetHandlers() const;

private:
  AssetManagerNewManager();

  std::vector<AssetManagerNewHandler*> m_handlers;
};