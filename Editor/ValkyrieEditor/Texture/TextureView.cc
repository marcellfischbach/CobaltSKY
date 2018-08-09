

#include <Texture/TextureView.hh>
#include <Texture/PreviewWidget.hh>
#include <AssetManager/Utils.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <qlayout.h>
#include <qpushbutton.h>



TextureWidget::TextureWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);

  QGridLayout *layout = new QGridLayout(m_gui.wPreview);
  layout->setContentsMargins(0, 0, 0, 0);
  m_previewWidget = new texture::PreviewWidget(this);
  m_previewWidget->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_previewWidget, 1, 0, 1, 5);
}

TextureWidget::~TextureWidget()
{

}

void TextureWidget::on_btnRed_toggled(bool checked)
{
  m_previewWidget->SetRed(checked);
}

void TextureWidget::on_btnGreen_toggled(bool checked)
{
  m_previewWidget->SetGreen(checked);
}

void TextureWidget::on_btnBlue_toggled(bool checked)
{
  m_previewWidget->SetBlue(checked);
}

void TextureWidget::on_btnAlpha_toggled(bool checked)
{
  m_previewWidget->SetAlpha(checked);
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
  SetName(assetmanager::GetNameFromResource(GetResourceLocator()) + QString("(Texture)"));
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


