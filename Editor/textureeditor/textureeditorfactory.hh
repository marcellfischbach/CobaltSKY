#pragma once


#include <textureeditor/textureeditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class TEXTUREEDITOR_API TextureEditorFactory : public iAssetEditorFactory
{
public:
  TextureEditorFactory() { }
  virtual ~TextureEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset *asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset *asset) const;

};