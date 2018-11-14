
#include <materialeditor/materialeditor.hh>
#include <materialeditor/materialeditorwidget.hh>
#include <materialeditor/materialeditorproperties.hh>

#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/itexture.hh>

#include <csfile/csffile.hh>

#include <QFile>
#include <editor/basicdockitem.hh>
#include <editor/editor.hh>
#include <editor/eventbus.hh>
#include <editor/dockitems.hh>
#include <editor/project/project.hh>
#include <editor/assetmodel/model.hh>
#include <editor/components/baseeditorwidget.hh>
#include <materialeditor/materialeditorevents.hh>

void material_attribute_changed(csEvent &event, void *ptr);

MaterialEditor::MaterialEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new MaterialEditorWidget(this);
  m_properties = new MaterialEditorProperties();

  BaseEditorWidget::Description descL;
  BaseEditorWidget::Description descR(m_properties);
  SetWidget(new BaseEditorWidget(0,
    descL,
    m_widget,
    descR));

  QObject::connect(m_properties, SIGNAL(MaterialChanged()), m_widget, SLOT(MaterialChanged()));
  EventBus::Get().Register(MaterialEditorAttributeChanged::GetStaticClass(), material_attribute_changed, this);
}

MaterialEditor::~MaterialEditor()
{
}


void MaterialEditor::UpdateAsset()
{
  const csResourceLocator &locator = GetAsset()->GetResourceLocator();


  m_material = csResourceManager::Get()->Aquire<csMaterialWrapper>(locator);
  m_widget->SetMaterial(m_material);

  m_properties->SetMaterial(m_material);
}


void MaterialEditor::Save()
{
  QString absFileName = GetResourceFileName();

  csfFile outputFile;
  if (!outputFile.Parse(std::string(absFileName.toLatin1())))
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
  if (!materialEntry)
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

  Save(outputFile);

}

void MaterialEditor::MaterialAttributeChanged(const csResourceLocator &materialLocator, const std::string &attributeID, const std::string &attribureName)
{
  printf("MaterialEditor::AttributeChanged %s -> %s -> %s\n",
    materialLocator.GetDebugName().c_str(),
    attributeID.c_str(),
    attribureName.c_str());
  csResourceLocator locator = m_material->Get()->GetMaterialDef()->GetLocator();
  if (Editor::Get()->GetProject()->GetModel()->IsFinalLocator(materialLocator))
  {
    m_properties->AttributeChanged(attributeID, attribureName);
  }
  else
  {
    printf("This is not the master locator\n");
  }

}

void material_attribute_changed(csEvent &event, void *ptr)
{
  MaterialEditorAttributeChanged &meac = (MaterialEditorAttributeChanged&)event;
  reinterpret_cast<MaterialEditor*>(ptr)->MaterialAttributeChanged(meac.GetMaterialLocator(), meac.GetAttributeID(), meac.GetAttributeName());
}


void MaterialEditor::ReplaceFileContent()
{
  csfFile outputFile;
  csfEntry *assetEntry = outputFile.CreateEntry("asset");
  csfEntry *dataEntry = outputFile.CreateEntry("data");
  csfEntry *materialEntry = outputFile.CreateEntry("material");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);

  FillEntry(materialEntry, outputFile);
  Save(outputFile);
}


void MaterialEditor::Save(csfFile &file)
{
  QString absFileName = GetResourceFileName();

  file.Output(std::string(absFileName.toLatin1()));
}

void MaterialEditor::FillEntry(csfEntry *materialEntry, csfFile &file)
{
  csfEntry *materialDefEntry = file.CreateEntry("materialDef");
  csfEntry *parametersEntry = file.CreateEntry("parameters");
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  csMaterial *material = m_material->Get();
  csMaterialDefWrapper *materialDefWrapper = material->GetMaterialDef();
  if (!materialDefWrapper || !materialDefWrapper->IsValid())
  {
    return;
  }

  if (materialDefWrapper->GetLocator().IsValid())
  {
    materialDefEntry->AddAttribute("locator", materialDefWrapper->GetLocator().AsAnonymous().Encode());
  }

  csMaterialDef *materialDef = materialDefWrapper->Get();

  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    if (material->IsInherited(i))
    {
      continue;
    }
    std::string parameterId = materialDef->GetParameterId(i);
    std::string parameterName = materialDef->GetParameterName(i);
    csfEntry *parameterEntry = file.CreateEntry("parameter");
    parametersEntry->AddChild(parameterEntry);
    parameterEntry->AddAttribute("id", parameterId);
    parameterEntry->AddAttribute("name", parameterName);

    switch (materialDef->GetParamType(i))
    {
    case eSPT_Float:
      parameterEntry->AddChild(file.CreateEntry("float"))
          ->AddAttributeFloat(material->GetFloat(i));
      break;
    case eSPT_Vector2:
      parameterEntry->AddChild(file.CreateEntry("float2"))
          ->AddAttributeFloat(material->GetFloat2(i).x)
          ->AddAttributeFloat(material->GetFloat2(i).y);
      break;
    case eSPT_Vector3:
      parameterEntry->AddChild(file.CreateEntry("float3"))
          ->AddAttributeFloat(material->GetFloat3(i).x)
          ->AddAttributeFloat(material->GetFloat3(i).y)
          ->AddAttributeFloat(material->GetFloat3(i).z);
      break;
    case eSPT_Vector4:
      parameterEntry->AddChild(file.CreateEntry("float4"))
          ->AddAttributeFloat(material->GetFloat4(i).x)
          ->AddAttributeFloat(material->GetFloat4(i).y)
          ->AddAttributeFloat(material->GetFloat4(i).y)
          ->AddAttributeFloat(material->GetFloat4(i).w);
      break;
    case eSPT_Color4:
      parameterEntry->AddChild(file.CreateEntry("color4"))
          ->AddAttributeFloat(material->GetColor4(i).r)
          ->AddAttributeFloat(material->GetColor4(i).g)
          ->AddAttributeFloat(material->GetColor4(i).b)
          ->AddAttributeFloat(material->GetColor4(i).a);
      break;
    case eSPT_Texture:
      if (material->GetTexture(i))
      {
        csResourceLocator loc = material->GetTexture(i)->GetLocator();
        parameterEntry->AddChild(file.CreateEntry("locator"))
            ->AddAttribute("locator", loc.Encode());
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
