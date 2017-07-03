#pragma once

#include <cobalt/core/csresourcelocator.hh>

class QDomDocument;
class QDomElement;
class AssetManagerRenamer
{
public:
  AssetManagerRenamer(const csResourceLocator &from, const csResourceLocator &to);

  bool Execute();

private:
  void MoveTheFile(const csResourceLocator &from, const csResourceLocator &to);

private:
  csResourceLocator m_from;
  csResourceLocator m_to;

};