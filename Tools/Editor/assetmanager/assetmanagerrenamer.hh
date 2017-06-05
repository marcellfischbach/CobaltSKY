#pragma once

#include <valkyrie/core/vkresourcelocator.hh>

class QDomDocument;
class QDomElement;
class AssetManagerRenamer
{
public:
  AssetManagerRenamer(const vkResourceLocator &from, const vkResourceLocator &to);

  bool Execute();

private:
  void Rename(const vkResourceLocator &from, const vkResourceLocator &to);
  void Rename(const vkResourceLocator &resource, const vkResourceLocator &from, const vkResourceLocator &to);
  void Rename(QDomDocument doc, QDomElement element, const vkResourceLocator &from, const vkResourceLocator &to);
  void MoveTheFile(const vkResourceLocator &from, const vkResourceLocator &to);

private:
  vkResourceLocator m_from;
  vkResourceLocator m_to;

};