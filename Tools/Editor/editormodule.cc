

#include <editormodule.hh>
#include <master.refl.cc>

#include <valkyrie/core/vkresourcemanager.hh>
#include <editor.hh>
#include <samplereditor/samplereditorfactory.hh>
#include <shadergrapheditor/shadergrapheditorfactory.hh>
#include <shadergrapheditor/shadergrapheditormetaassetxmlloader.hh>
#include <textureeditor/textureeditorfactory.hh>

#include <loaders/loaderseditoriconassetxmlloader.hh>

void EditorModule::Initialize()
{
  register_classes();


  Editor *editor = Editor::Get();
  editor->AddEditorFactory(new SamplerEditorFactory());
  editor->AddEditorFactory(new ShaderGraphEditorFactory());
  editor->AddEditorFactory(new TextureEditorFactory());


  vkResourceManager *mgr = vkResourceManager::Get();
  mgr->RegisterLoader(new ShaderGraphEditorMetaAssetXMLLoader());
  mgr->RegisterLoader(new LoadersEditorIconAssetXMLLoader());
}