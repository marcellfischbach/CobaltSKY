#pragma once

#include <assetmanager/assetmanagernewhandler.hh>

class ShaderGraphEditorNewHandler : public AssetManagerNewHandler
{
public:
  ShaderGraphEditorNewHandler();
  virtual ~ShaderGraphEditorNewHandler();

  virtual QString GetTypeName() const;
  virtual bool CreateNewAsset(const QDir &folder, const QString &assetName);

};