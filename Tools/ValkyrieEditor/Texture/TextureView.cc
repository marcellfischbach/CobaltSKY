

#include <Texture/TextureView.hh>
#include <Texture/PreviewWidget.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <qlayout.h>
#include <qpushbutton.h>



TextureWidget::TextureWidget(QWidget *parent)
  : QWidget(parent)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(3, 3, 3, 3);


  QPushButton *btnRed = new QPushButton(tr("Red"), this);
  QPushButton *btnGreen = new QPushButton(tr("Green"), this);
  QPushButton *btnBlue = new QPushButton(tr("Blue"), this);
  QPushButton *btnAlpha = new QPushButton(tr("Alpha"), this);
  m_previewWidget = new texture::PreviewWidget(this);


  m_previewWidget->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_previewWidget, 1, 0, 1, 5);
  layout->addWidget(btnRed, 0, 0, 0, 1);
  layout->addWidget(btnGreen, 0, 1, 0, 1);
  layout->addWidget(btnBlue, 0, 2, 0, 1);
  layout->addWidget(btnAlpha, 0, 3, 0, 1);
}

TextureWidget::~TextureWidget()
{

}


void TextureWidget::Set(const vkResourceLocator &resourceLocator)
{
  m_resourceLocator = resourceLocator;
  ITexture2D *texture = vkResourceManager::Get()->GetOrLoad<ITexture2D>(m_resourceLocator);

  m_previewWidget->SetTexture(texture);
  m_previewWidget->repaint();

}




TextureView::TextureView(QWidget *parent)
  : EditorView()
{
  m_textureWidget = new TextureWidget(parent);
  SetWidget(m_textureWidget);
  SetCanClose(true);
}

TextureView::~TextureView()
{
  m_textureWidget->deleteLater();
}

void TextureView::Initialize()
{
  m_textureWidget->Set(GetResourceLocator());
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()));
}

bool TextureView::Close()
{
  return true;
}







TextureViewFactory::TextureViewFactory()
  : IEditorViewFactory()
{

}

TextureViewFactory::~TextureViewFactory()
{

}


bool TextureViewFactory::CanEdit(const vkResourceLocator &resourceLocator, IObject *obj)
{
  if (vkQueryClass<ITexture2D>(obj))
  {
    return true;
  }

  return false;
}

EditorView *TextureViewFactory::CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj)
{
  TextureView *view = new TextureView(parent);
  view->SetEditorObject(obj);
  view->SetResourceLocator(resourceLocator);
  view->Initialize();
  return view;
}


