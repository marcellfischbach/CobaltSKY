#pragma once


#include <materialeditor/materialeditorexport.hh>
#include <editor/iasseteditorfactory.hh>


class MATERIALEDITOR_API MaterialEditorFactory : public iAssetEditorFactory
{
public:
  MaterialEditorFactory() { }
  virtual ~MaterialEditorFactory() { }

  virtual bool CanEdit(cs::iObject *object, asset::model::Asset*asset) const;

  virtual iAssetEditor *CreateEditor(cs::iObject *object, asset::model::Asset*asset) const;

};