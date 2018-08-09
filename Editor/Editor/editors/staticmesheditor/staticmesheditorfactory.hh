#pragma once


#include <iasseteditorfactory.hh>


class StaticMeshEditorFactory : public iAssetEditorFactory
{
public:
  StaticMeshEditorFactory() { }
  virtual ~StaticMeshEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset *asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset *asset) const;

};