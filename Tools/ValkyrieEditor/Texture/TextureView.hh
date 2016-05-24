#pragma once

#include <EditorView.hh>
#include <Texture/TextureView.refl.hh>

namespace texture
{
class PreviewWidget;
}

class TextureWidget : public QWidget
{
public:
  TextureWidget(QWidget *parent);
  virtual ~TextureWidget();

  void Set(const vkResourceLocator &resourceLocator);

private:
  texture::PreviewWidget *m_previewWidget;

  vkResourceLocator m_resourceLocator;
};

class TextureView : public EditorView
{
public:
  TextureView(QWidget *parent);
  virtual ~TextureView();

  void Initialize();
  virtual bool Close();

private:
  TextureWidget *m_textureWidget;
};

VK_CLASS()
class TextureViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  TextureViewFactory();
  virtual ~TextureViewFactory();


  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};

