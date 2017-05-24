
#include <textureeditor/textureeditorproperties.hh>
#include <components/assetresourcewidget.hh>
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
  VK_RELEASE(m_texture);
}

void TextureEditorProperties::SetTexture(iTexture *texture)
{
  VK_SET(m_texture, texture);
  if (m_texture)
  {
    m_samplerWidget->SetResourceLocator()
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

  frameLayout->addWidget(label, 0, 0, 1, 1, Qt::AlignLeft);
  frameLayout->addWidget(m_samplerWidget, 0, 1, 1, 1, Qt::AlignLeft);

  frameLayout->addItem(new QSpacerItem(1, 1, QSizePolicy::Maximum, QSizePolicy::MinimumExpanding), 1, 0, 1, 2);

  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(frame);

  QGridLayout *thisLayout = new QGridLayout(this);
  thisLayout->setContentsMargins(0, 0, 0, 0);
  thisLayout->addWidget(scrollArea);
  

}
