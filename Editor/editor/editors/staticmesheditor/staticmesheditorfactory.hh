#pragma once


#include <editor/editorexport.hh>
#include <editor/iasseteditorfactory.hh>


class StaticMeshEditorFactory : public iAssetEditorFactory
{
public:
  StaticMeshEditorFactory() { }
  virtual ~StaticMeshEditorFactory() { }

  virtual bool CanEdit(cs::iObject *object, asset::model::Asset *asset) const;

  virtual iAssetEditor *CreateEditor(cs::iObject *object, asset::model::Asset *asset) const;

};