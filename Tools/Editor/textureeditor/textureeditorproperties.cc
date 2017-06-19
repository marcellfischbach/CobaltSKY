
#include <textureeditor/textureeditorproperties.hh>
#include <components/assetresourcewidget.hh>
#include <valkyrie/core/csresourcemanager.hh>
#include <valkyrie/graphics/isampler.hh>
#include <valkyrie/graphics/itexture.hh>

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

void TextureEditorProperties::SetTexture(iTexture *texture)
{
  CS_SET(m_texture, texture);
  if (m_texture)
  {
    csResourceLocator locator = csResourceManager::Get()->GetLocator(texture->GetSampler());
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
  m_samplerWidget->AddValidClass(iSampler::GetStaticClass());
  connect(m_samplerWidget, SIGNAL(ResourceChanged(const csResourceLocator &)), this, SIGNAL(SamplerChanged(const csResourceLocator &)));

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
