#pragma once


#include <iasseteditorfactory.hh>


class TextureEditorFactory : public iAssetEditorFactory
{
public:
  TextureEditorFactory() { }
  virtual ~TextureEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset *asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset *asset) const;

};