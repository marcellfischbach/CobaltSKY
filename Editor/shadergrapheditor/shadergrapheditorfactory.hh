#pragma once


#include <shadergrapheditor/shadergrapheditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class ShaderGraphEditorFactory : public iAssetEditorFactory
{
public:
  ShaderGraphEditorFactory() { }
  virtual ~ShaderGraphEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset*asset) const;

};