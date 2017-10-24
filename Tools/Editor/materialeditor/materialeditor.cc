
#include <materialeditor/materialeditor.hh>
#include <materialeditor/materialeditorwidget.hh>
#include <materialeditor/materialeditorproperties.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/itexture.hh>

#include <csfile/csffile.hh>

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


void MaterialEditor::UpdateAsset()
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
    materialElement = doc.createElement("material");
    dataElement.appendChild(materialElement);
  }
  else
  {
    QDomElement newMaterialElement = doc.createElement("material");
    dataElement.replaceChild(newMaterialElement, materialElement);
    materialElement = newMaterialElement;
  }

  FillElement(materialElement, doc);

  SaveDocument(doc);


  csfFile outputFile;
  if (!outputFile.Parse(std::string(absFileName)))
  {
    printf("Unable to open asset: %s\n", (const char*)absFileName.toLatin1());
    ReplaceFileContent();
    return;
  }

  csfEntry *assetEntry = outputFile.GetRoot()->GetEntry("asset");
  if (!assetEntry)
  {
    ReplaceFileContent();
    return;
  }

  csfEntry *dataEntry = assetEntry->GetEntry("data");
  if (!dataEntry)
  {
    ReplaceFileContent();
    return;
  }

  csfEntry *materialEntry = dataEntry->GetEntry("material");
  if (!materialElement)
  {
    materialEntry = outputFile.CreateEntry("material");
    dataEntry->AddChild(materialEntry);
  }
  else
  {
    materialEntry->RemoveAttributes();
    materialEntry->RemoveAllChildren();
  }



  FillEntry(materialEntry, outputFile);

  SaveDocument(outputFile);

}


void MaterialEditor::ReplaceFileContent()
{
  QDomDocument doc;

  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement materialElement = doc.createElement("material");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(materialElement);

  FillElement(materialElement, doc);
  SaveDocument(doc);


  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *materialEntry = outputFile.CreateEntry("material");

  outputFile.GetRoot()->AddAttribute(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);

  FillEntry(materialEntry, outputFile);
  SaveDocument(outputFile);
}


void MaterialEditor::SaveDocument(QDomDocument doc)
{
  QString absFileName = GetResourceFileName();
  QFile file(absFileName);

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

  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());


}

void MaterialEditor::SaveDocument(csfFile &file)
{
  QString absFileName = GetResourceFileName();

  file.Output(std::string(absFileName.toLatin1()));
  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());
}


void MaterialEditor::FillElement(QDomElement materialElement, QDomDocument doc)
{
  QDomElement materialDefElement = doc.createElement("materialDef");
  QDomElement parametersElement = doc.createElement("parameters");
  materialElement.appendChild(materialDefElement);
  materialElement.appendChild(parametersElement);

  csMaterialDef *materialDef = m_material->GetMaterialDef();
  if (materialDef)
  {
    csResourceLocator materialDefLocator = csResourceManager::Get()->GetLocator(materialDef);
    materialDefElement.appendChild(doc.createTextNode(QString(materialDefLocator.GetText().c_str())));
  }

  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    if (m_material->IsInherited(i))
    {
      continue;
    }
    std::string parameterName = materialDef->GetParamName(i);
    QDomElement parameterElement = doc.createElement("parameter");
    parametersElement.appendChild(parameterElement);
    parameterElement.setAttribute("name", QString(parameterName.c_str()));

    switch (materialDef->GetParamType(i))
    {
    case eSPT_Float:
      parameterElement.appendChild(doc.createElement("float"))
        .appendChild(doc.createTextNode(QString("%1")
          .arg((double)m_material->GetFloat(i), 0, 'f')));
      break;
    case eSPT_Vector2:
      parameterElement.appendChild(doc.createElement("float2"))
        .appendChild(doc.createTextNode(QString("%1, %2")
          .arg((double)m_material->GetFloat2(i).x, 0, 'f')
          .arg((double)m_material->GetFloat2(i).y, 0, 'f')
        ));
      break;
    case eSPT_Vector3:
      parameterElement.appendChild(doc.createElement("float3"))
        .appendChild(doc.createTextNode(QString("%1, %2, %3")
          .arg((double)m_material->GetFloat3(i).x, 0, 'f')
          .arg((double)m_material->GetFloat3(i).y, 0, 'f')
          .arg((double)m_material->GetFloat3(i).z, 0, 'f')
        ));
      break;
    case eSPT_Vector4:
      parameterElement.appendChild(doc.createElement("float4"))
        .appendChild(doc.createTextNode(QString("%1, %2, %3, %4")
          .arg((double)m_material->GetFloat4(i).x, 0, 'f')
          .arg((double)m_material->GetFloat4(i).y, 0, 'f')
          .arg((double)m_material->GetFloat4(i).z, 0, 'f')
          .arg((double)m_material->GetFloat4(i).w, 0, 'f')
        ));
      break;
    case eSPT_Color4:
      parameterElement.appendChild(doc.createElement("color4"))
        .appendChild(doc.createTextNode(QString("%1, %2, %3, %4")
          .arg((double)m_material->GetColor4(i).r, 0, 'f')
          .arg((double)m_material->GetColor4(i).g, 0, 'f')
          .arg((double)m_material->GetColor4(i).b, 0, 'f')
          .arg((double)m_material->GetColor4(i).a, 0, 'f')
        ));
      break;
    case eSPT_Texture:
      if (m_material->GetTexture(i))
      {
        csResourceLocator loc = csResourceManager::Get()->GetLocator(m_material->GetTexture(i));
        std::string name = loc.GetText();
        parameterElement.appendChild(doc.createElement("locator"))
          .appendChild(doc.createTextNode(QString(name.c_str())));
      }
      break;
    }
  }
}

void MaterialEditor::FillEntry(csfEntry *materialEntry, csfFile &file)
{
  const csfEntry *materialDefEntry = file.CreateEntry("materialDef");
  const csfEntry *parametersEntry = file.CreateEntry("parameters");
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  csMaterialDef *materialDef = m_material->GetMaterialDef();
  if (materialDef)
  {
    csResourceLocator materialDefLocator = csResourceManager::Get()->GetLocator(materialDef);
    materialDefEntry->AddAttribute(materialDefLocator.GetText());
  }

  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    if (m_material->IsInherited(i))
    {
      continue;
    }
    std::string parameterName = materialDef->GetParamName(i);
    csfEntry *parameterEntry = file.CreateEntry("parameter");
    parametersEntry->AddChild(parameterEntry);
    parameterEntry->AddAttribute("name", parameterName);

    switch (materialDef->GetParamType(i))
    {
    case eSPT_Float:
      parameterEntry->AddChild(file.CreateEntry("float"))
          ->AddAttributeFloat(m_material->GetFloat(i));
      break;
    case eSPT_Vector2:
      parameterEntry->AddChild(file.CreateEntry("float2"))
          ->AddAttributeFloat(m_material->GetFloat2(i).x)
          ->AddAttributeFloat(m_material->GetFloat2(i).y);
      break;
    case eSPT_Vector3:
      parameterEntry->AddChild(file.CreateEntry("float3"))
          ->AddAttributeFloat(m_material->GetFloat3(i).x)
          ->AddAttributeFloat(m_material->GetFloat3(i).y)
          ->AddAttributeFloat(m_material->GetFloat3(i).z);
      break;
    case eSPT_Vector4:
      parameterEntry->AddChild(file.CreateEntry("float4"))
          ->AddAttributeFloat(m_material->GetFloat4(i).x)
          ->AddAttributeFloat(m_material->GetFloat4(i).y)
          ->AddAttributeFloat(m_material->GetFloat4(i).y)
          ->AddAttributeFloat(m_material->GetFloat4(i).w);
      break;
    case eSPT_Color4:
      parameterEntry->AddChild(file.CreateEntry("color4"))
          ->AddAttributeFloat(m_material->GetColor4(i).r)
          ->AddAttributeFloat(m_material->GetColor4(i).g)
          ->AddAttributeFloat(m_material->GetColor4(i).b)
          ->AddAttributeFloat(m_material->GetColor4(i).a);
      break;
    case eSPT_Texture:
      if (m_material->GetTexture(i))
      {
        csResourceLocator loc = csResourceManager::Get()->GetLocator(m_material->GetTexture(i));
        parameterEntry->AddChild(file.CreateEntry("locator"))
            ->AddAttribute(loc.GetText());
      }
      break;
    }
  }
}





void MaterialEditor::UpdatePreview()
{
  QImage preview = TakeScreenshot(64, 64);
  if (!preview.isNull())
  {
    ReplacePreviewIcon(preview);
  }
}

QImage MaterialEditor::TakeScreenshot(unsigned width, unsigned height)
{
  return m_widget->GetSceneView()->TakeScreenshot(width, height);
}
