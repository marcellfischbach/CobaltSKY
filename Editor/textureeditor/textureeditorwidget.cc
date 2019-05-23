#include <textureeditor/textureeditorwidget.hh>
#include <textureeditor/textureeditor.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <editor/editor.hh>
#include <editor/project/project.hh>
#include <QFile>



TextureEditorWidget::TextureEditorWidget(TextureEditor *parent)
  : QWidget()
  , m_editor(parent)
  , m_texture(0)
{
  m_gui.setupUi(this);
  m_gui.openGLWidget->SetName("TextureEditor");
}

TextureEditorWidget::~TextureEditorWidget()
{

}

void TextureEditorWidget::SetTexture(cs::Texture2DWrapper *texture)
{
  CS_SET(m_texture, texture);

  m_gui.openGLWidget->SetTexture(texture);
  m_gui.spLOD->setMaximum(texture->Get()->GetNumberOfLODs() - 1);
}

void TextureEditorWidget::SamplerChanged(const cs::ResourceLocator &locator)
{
  cs::SamplerWrapper *sampler = cs::ResourceManager::Get()->Aquire<cs::SamplerWrapper>(locator);
  if (sampler)
  {
    m_texture->Get()->SetSampler(sampler);
    m_gui.openGLWidget->repaint();
  }
}


void TextureEditorWidget::on_pbRed_toggled(bool checked)
{
  m_gui.openGLWidget->SetRed(checked);
}

void TextureEditorWidget::on_pbGreen_toggled(bool checked)
{
  m_gui.openGLWidget->SetGreen(checked);
}

void TextureEditorWidget::on_pbBlue_toggled(bool checked)
{
  m_gui.openGLWidget->SetBlue(checked);
}

void TextureEditorWidget::on_pbAlpha_toggled(bool checked)
{
  m_gui.openGLWidget->SetAlpha(checked);
}


void TextureEditorWidget::on_spLOD_valueChanged(int value)
{
  m_gui.openGLWidget->SetLOD(value);
}

void TextureEditorWidget::on_pbSave_clicked()
{

  cs::Texture2DWrapper *managerTexture = cs::ResourceManager::Get()->Get<cs::Texture2DWrapper>(m_editor->GetAsset()->GetResourceLocator());
  if (managerTexture)
  {
    managerTexture->Get()->SetSampler(m_texture->Get()->GetSampler());
  }


  QString absFileName = m_editor->GetResourceFileName();


  csfFile outputFile;
  if (!outputFile.Parse(std::string(absFileName.toLatin1())))
  {
    printf("Unable to save\n");
    //SaveNew();
    return;
  }
  csfEntry *assetEntry = outputFile.GetEntry("asset");
  if (!assetEntry)
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }


  csfEntry *dataEntry = assetEntry->GetEntry("data");
  if (!dataEntry)
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }


  csfEntry *texture2dEntry = dataEntry->GetEntry("texture2d");
  if (!texture2dEntry)
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }


  csfEntry *samplerEntry = texture2dEntry->GetEntry("sampler");
  if (!samplerEntry)
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }

  samplerEntry->RemoveAttributes();

  cs::ResourceLocator samplerLocator = m_texture->Get()->GetSampler()->GetLocator();
  samplerEntry->AddAttribute(samplerLocator.GetResourceFile());

  outputFile.Output(std::string(absFileName.toLatin1()));

  // Editor::Get()->GetProject()->GetReferenceTree().UpdateDependencyTree(m_editor->GetAsset()->GetResourceLocator().GetResourceFile());
}
