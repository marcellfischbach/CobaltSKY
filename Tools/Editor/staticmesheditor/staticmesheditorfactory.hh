#pragma once


#include <iasseteditorfactory.hh>


class StaticMeshEditorFactory : public iAssetEditorFactory
{
public:
  StaticMeshEditorFactory() { }
  virtual ~StaticMeshEditorFactory() { }

  virtual bool CanEdit(iObject *object, const AssetDescriptor &descriptor) const;

  virtual iAssetEditor *CreateEditor(iObject *object, const AssetDescriptor &descriptor) const;

};