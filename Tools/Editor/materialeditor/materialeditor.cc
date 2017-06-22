
#include <materialeditor/materialeditor.hh>
#include <materialeditor/materialeditorwidget.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/csmaterial.hh>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <editor.hh>
#include <dockitems.hh>
#include <project/project.hh>

MaterialEditor::MaterialEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new MaterialEditorWidget(this);
  SetWidget(m_widget);

  AddDockItemName(PROPERTIES_DOCK_NAME);

}

MaterialEditor::~MaterialEditor()
{
}


void MaterialEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  csMaterial *material = csResourceManager::Get()->Aquire<csMaterial>(descriptor.GetLocator());
  m_widget->SetMaterial(material);
}
