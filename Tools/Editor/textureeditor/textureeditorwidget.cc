#include <textureeditor/textureeditorwidget.hh>
#include <textureeditor/textureeditor.hh>
#include <valkyrie/core/vkresourcemanager.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/isampler.hh>
#include <QDomDocument>
#include <QDomElement>
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

void TextureEditorWidget::SetTexture(iTexture2D *texture)
{
  VK_SET(m_texture, texture);

  m_gui.openGLWidget->SetTexture(texture);
  m_gui.spLOD->setMaximum(texture->GetNumberOfLODs() - 1);
}

void TextureEditorWidget::SamplerChanged(const vkResourceLocator &locator)
{
  iSampler *sampler = vkResourceManager::Get()->Aquire<iSampler>(locator);
  if (sampler)
  {
    m_texture->SetSampler(sampler);
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
  QString filename(m_editor->GetAssetDescriptor().GetAssetFileName().c_str());

  QFile file(filename);
  QDomDocument doc;
  if (!doc.setContent(&file))
  {
    file.close();
    printf("Unable to save\n");
    //SaveNew();
    return;
  }
  file.close();

  QDomElement assetElement = doc.documentElement();
  if (assetElement.isNull() || assetElement.tagName() != QString("asset"))
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }

  QDomElement dataElement = assetElement.firstChildElement("data");
  if (dataElement.isNull())
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }


  QDomElement texture2dElement = dataElement.firstChildElement("texture2d");
  if (texture2dElement.isNull())
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }


  QDomElement samplerElement = texture2dElement.firstChildElement("sampler");
  if (samplerElement.isNull())
  {
    printf("Malformed asset\n");
    // SaveNew ();
    return;
  }

  QDomNode child;
  while (!(child = samplerElement.firstChild()).isNull())
  {
    samplerElement.removeChild(child);
  }

  vkResourceLocator samplerLocator = vkResourceManager::Get()->GetLocator(m_texture->GetSampler());
  QDomText text = doc.createTextNode(QString(samplerLocator.GetResourceFile().c_str()));
  samplerElement.appendChild(text);

  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    file.write(doc.toString(2).toLatin1());
    file.close();
  }

}