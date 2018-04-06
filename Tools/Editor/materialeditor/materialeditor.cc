
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
#include <basicdockitem.hh>
#include <editor.hh>
#include <dockitems.hh>
#include <project/project.hh>
#include <components/baseeditorwidget.hh>

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
  Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(GetAssetDescriptor().GetLocator().GetResourceFile());
}

void MaterialEditor::FillEntry(csfEntry *materialEntry, csfFile &file)
{
  csfEntry *materialDefEntry = file.CreateEntry("materialDef");
  csfEntry *parametersEntry = file.CreateEntry("parameters");
  materialEntry->AddChild(materialDefEntry);
  materialEntry->AddChild(parametersEntry);

  csMaterialDef *materialDef = m_material->GetMaterialDef();
  if (materialDef)
  {
    csResourceLocator materialDefLocator = csResourceManager::Get()->GetLocator(materialDef);
    materialDefEntry->AddAttribute("locator", materialDefLocator.GetText());
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
            ->AddAttribute("locator", loc.GetText());
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
