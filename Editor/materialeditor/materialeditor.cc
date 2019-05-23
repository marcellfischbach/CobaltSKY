
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

void material_attribute_changed(cs::Event &event, void *ptr);

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
  const cs::ResourceLocator &locator = GetAsset()->GetResourceLocator();


  m_material = cs::ResourceManager::Get()->Aquire<cs::MaterialWrapper>(locator);
  m_widget->SetMaterial(m_material);

  m_properties->SetMaterial(m_material);
}


void MaterialEditor::Save()
{
  QString absFileName = GetResourceFileName();

  cs::file::File outputFile;
  if (!outputFile.Parse(std::string(absFileName.toLatin1())))
  {
    printf("Unable to open asset: %s\n", (const char*)absFileName.toLatin1());
    ReplaceFileContent();
    return;
  }

  cs::file::Entry *assetEntry = outputFile.GetRoot()->GetEntry("asset");
  if (!assetEntry)
  {
    ReplaceFileContent();
    return;
  }

  cs::file::Entry *dataEntry = assetEntry->GetEntry("data");
  if (!dataEntry)
  {
    ReplaceFileContent();
    return;
  }

  cs::file::Entry *materialEntry = dataEntry->GetEntry("material");
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

void MaterialEditor::MaterialAttributeChanged(const cs::ResourceLocator &materialLocator, const std::string &attributeID, const std::string &attribureName)
{
  printf("MaterialEditor::AttributeChanged %s -> %s -> %s\n",
    materialLocator.GetDebugName().c_str(),
    attributeID.c_str(),
    attribureName.c_str());
  cs::ResourceLocator locator = m_material->Get()->GetMaterialDef()->GetLocator();
  if (Editor::Get()->GetProject()->GetModel()->IsFinalLocator(materialLocator))
  {
    m_properties->AttributeChanged(attributeID, attribureName);
  }
  else
  {
    printf("This is not the master locator\n");
  }

}

void material_attribute_changed(cs::Event &event, void *ptr)
{
  MaterialEditorAttributeChanged &meac = (MaterialEditorAttributeChanged&)event;
  reinterpret_cast<MaterialEditor*>(ptr)->MaterialAttributeChanged(meac.GetMaterialLocator(), meac.GetAttributeID(), meac.GetAttributeName());
}


void MaterialEditor::ReplaceFileContent()
{
  cs::file::File outputFile;
  cs::file::Entry *assetEntry = outputFile.CreateEntry("asset");
  cs::file::Entry *dataEntry = outputFile.CreateEntry("data");
  cs::file::Entry *materialEntry = outputFile.CreateEntry("material");

  outputFile.GetRoot()->AddChild(assetEntry);
  assetEntry->AddChild(dataEntry);
  dataEntry->AddChild(materialEntry);

  FillEntry(materialEntry, outputFile);
  Save(outputFile);
}


void MaterialEditor::Save(cs::file::File &file)
{
  QString absFileName = GetResourceFileName();

  file.Output(std::string(absFileName.toLatin1()));
}

void MaterialEditor::FillEntry(cs::file::Entry *materialEntry, cs::file::File &file)
{
  cs::file::Entry *materialDefEntry = file.CreateEntry("materialDef");
  cs::file::Entry *parametersEntry = file.CreateEntry("parameters");
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  cs::Material *material = m_material->Get();
  cs::MaterialDefWrapper *materialDefWrapper = material->GetMaterialDef();
  if (!materialDefWrapper || !materialDefWrapper->IsValid())
  {
    return;
  }

  if (materialDefWrapper->GetLocator().IsValid())
  {
    materialDefEntry->AddAttribute("locator", materialDefWrapper->GetLocator().AsAnonymous().Encode());
  }

  cs::MaterialDef *materialDef = materialDefWrapper->Get();

  for (csSize i = 0, in = materialDef->GetNumberOfParameters(); i < in; ++i)
  {
    if (material->IsInherited(i))
    {
      continue;
    }
    std::string parameterId = materialDef->GetParameterId(i);
    std::string parameterName = materialDef->GetParameterName(i);
    cs::file::Entry *parameterEntry = file.CreateEntry("parameter");
    parametersEntry->AddChild(parameterEntry);
    parameterEntry->AddAttribute("id", parameterId);
    parameterEntry->AddAttribute("name", parameterName);

    switch (materialDef->GetParamType(i))
    {
    case cs::eSPT_Float:
      parameterEntry->AddChild(file.CreateEntry("float"))
          ->AddAttributeFloat(material->GetFloat(i));
      break;
    case cs::eSPT_Vector2:
      parameterEntry->AddChild(file.CreateEntry("float2"))
          ->AddAttributeFloat(material->GetFloat2(i).x)
          ->AddAttributeFloat(material->GetFloat2(i).y);
      break;
    case cs::eSPT_Vector3:
      parameterEntry->AddChild(file.CreateEntry("float3"))
          ->AddAttributeFloat(material->GetFloat3(i).x)
          ->AddAttributeFloat(material->GetFloat3(i).y)
          ->AddAttributeFloat(material->GetFloat3(i).z);
      break;
    case cs::eSPT_Vector4:
      parameterEntry->AddChild(file.CreateEntry("float4"))
          ->AddAttributeFloat(material->GetFloat4(i).x)
          ->AddAttributeFloat(material->GetFloat4(i).y)
          ->AddAttributeFloat(material->GetFloat4(i).y)
          ->AddAttributeFloat(material->GetFloat4(i).w);
      break;
    case cs::eSPT_Color4:
      parameterEntry->AddChild(file.CreateEntry("color4"))
          ->AddAttributeFloat(material->GetColor4(i).r)
          ->AddAttributeFloat(material->GetColor4(i).g)
          ->AddAttributeFloat(material->GetColor4(i).b)
          ->AddAttributeFloat(material->GetColor4(i).a);
      break;
    case cs::eSPT_Texture:
      if (material->GetTexture(i))
      {
        cs::ResourceLocator loc = material->GetTexture(i)->GetLocator();
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
