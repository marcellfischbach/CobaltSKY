#pragma once


#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class ShaderGraphEditorFactory : public iAssetEditorFactory
{
public:
  ShaderGraphEditorFactory() { }
  virtual ~ShaderGraphEditorFactory() { }

  virtual bool CanEdit(cs::iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(cs::iObject *object, asset::model::Asset*asset) const;

};