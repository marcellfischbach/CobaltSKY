
#include <textureeditor/textureeditorproperties.hh>
#include <editor/components/assetresourcewidget.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/itexture2d.hh>

#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

TextureEditorProperties::TextureEditorProperties()
  : QWidget()
{
  InitGUI();
}

TextureEditorProperties::~TextureEditorProperties()
{
  CS_RELEASE(m_texture);
}

void TextureEditorProperties::SetTexture(cs::TextureWrapper *texture)
{
  CS_SET(m_texture, texture);
  if (m_texture)
  {
    cs::ResourceLocator locator = texture->GetLocator();
    m_samplerWidget->SetResourceLocator(locator);
  }
}

void TextureEditorProperties::InitGUI()
{
  QFrame *frame = new QFrame(this);
  QGridLayout *frameLayout = new QGridLayout(frame);
  frameLayout->setContentsMargins(3, 3, 3, 3);
  frameLayout->setHorizontalSpacing(3);
  frameLayout->setVerticalSpacing(3);

  QLabel *label = new QLabel(tr("Sampler"), frame);
  m_samplerWidget = new AssetResourceWidget(frame);
  m_samplerWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_samplerWidget->AddValidClass(cs::SamplerWrapper::GetStaticClass());
  connect(m_samplerWidget, SIGNAL(ResourceChanged(const cs::ResourceLocator &)), this, SIGNAL(SamplerChanged(const cs::ResourceLocator &)));

  frameLayout->addWidget(label, 0, 0, 1, 1);
  frameLayout->addWidget(m_samplerWidget, 0, 1, 1, 1);

  frameLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), 1, 0, 1, 2);

  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(frame);

  QGridLayout *thisLayout = new QGridLayout(this);
  thisLayout->setContentsMargins(0, 0, 0, 0);
  thisLayout->addWidget(scrollArea);
  

}
