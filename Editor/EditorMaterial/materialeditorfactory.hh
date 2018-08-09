#pragma once


#include <iasseteditorfactory.hh>


class MaterialEditorFactory : public iAssetEditorFactory
{
public:
  MaterialEditorFactory() { }
  virtual ~MaterialEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset*asset) const;

};