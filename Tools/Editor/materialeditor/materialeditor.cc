
#include <materialeditor/materialeditor.hh>
#include <materialeditor/materialeditorwidget.hh>
#include <materialeditor/materialeditorproperties.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>

#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <basicdockitem.hh>
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
  m_properties = new MaterialEditorProperties();

  QObject::connect(m_properties, SIGNAL(MaterialChanged()), m_widget, SLOT(MaterialChanged()));
}

MaterialEditor::~MaterialEditor()
{
}


void MaterialEditor::OpenAsset()
{
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  m_material = csResourceManager::Get()->Aquire<csMaterial>(descriptor.GetLocator());
  m_widget->SetMaterial(m_material);

  m_properties->SetMaterial(m_material);
}


void MaterialEditor::PopulateDockItems()
{
  BasicDockItem *properties = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(PROPERTIES_DOCK_NAME));
  properties->SetContent(m_properties);
}

void MaterialEditor::Save()
{
  QString absFileName = GetResourceFileName();
  QFile file(absFileName);
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    printf("Unable to open asset: %s\n", (const char*)absFileName.toLatin1());
    ReplaceFileContent();
    return;
  }

  QDomElement assetElement = doc.documentElement();
  if (assetElement.tagName() != QString("asset"))
  {
    ReplaceFileContent();
    return;
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    ReplaceFileContent();
    return;
  }

  QDomElement materialElement = dataElement.firstChildElement("material");
  if (materialElement.isNull())
  {
    ReplaceFileContent();
    return;
  }

  QDomElement materialDefElement = materialElement.firstChildElement("materialDef");
  if (materialDefElement.isNull())
  {
    ReplaceFileContent();
    return;
  }

  QDomElement parametersElement = materialElement.firstChildElement("parameters");
  if (parametersElement.isNull())
  {
    ReplaceFileContent();
    return;
  }

  // remove all
  QDomNodeList children = materialDefElement.childNodes();
  for (int i = 0, in = children.length(); i < in; ++i)
  {
    materialDefElement.removeChild(children.at(i));
  }
  children = parametersElement.childNodes();
  for (int i = 0, in = children.length(); i < in; ++i)
  {
    parametersElement.removeChild(children.at(i));
  }

  csMaterialDef *materialDef = m_material->GetMaterialDef();
  if (materialDef)
  {

    csResourceLocator materialDefLocator = csResourceManager::Get()->GetLocator(materialDef);
    materialDefElement.appendChild(doc.createTextNode(QString(materialDefLocator.GetText().c_str())));
  }
  QString xml = doc.toString(2);
  printf("xml\n%s\n",
    (const char*)xml.toLatin1());
  if (file.isOpen())
  {
    file.close();
  }
  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(xml.toLatin1());
    file.close();
  }

  Editor::Get()->GetProject()->GetDependencyTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());

}



void MaterialEditor::ReplaceFileContent()
{
  printf("replace file\n");
}