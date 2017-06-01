#pragma once

#include <valkyrie/core/vkresourcelocator.hh>


class AssetManagerRenamer
{
public:
  AssetManagerRenamer(const vkResourceLocator &from, const vkResourceLocator &to);

  bool Execute();

private:
  vkResourceLocator m_from;
  vkResourceLocator m_to;

};