#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class MATERIALEDITOR_API MaterialEditorFactory : public iAssetEditorFactory
{
public:
  MaterialEditorFactory() { }
  virtual ~MaterialEditorFactory() { }

  virtual bool CanEdit(iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(iObject *object, asset::model::Asset*asset) const;

};