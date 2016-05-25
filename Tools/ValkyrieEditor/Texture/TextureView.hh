#pragma once

#include <EditorView.hh>
#include <Texture/TextureView.refl.hh>

#include <ui_TextureView.h>

namespace texture
{
class PreviewWidget;
}

class TextureWidget : public QWidget
{
  Q_OBJECT
public:
  TextureWidget(QWidget *parent);
  virtual ~TextureWidget();

  void Set(const vkResourceLocator &resourceLocator);

private:
  texture::PreviewWidget *m_previewWidget;

  vkResourceLocator m_resourceLocator;

  Ui::TextureWidget m_gui;

private slots:

  void on_btnRed_toggled(bool checked);
  void on_btnGreen_toggled(bool checked);
  void on_btnBlue_toggled(bool checked);
  void on_btnAlpha_toggled(bool checked);
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

