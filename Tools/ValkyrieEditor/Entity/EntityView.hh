#pragma once

#include <qwidget.h>
#include <EditorView.hh>
#include <Entity/EntityView.refl.hh>

class vkResourceLocator;
namespace entity
{
class PreviewWidget;
}


class EntityWidget : public QWidget
{
  Q_OBJECT
public:
  EntityWidget(QWidget *parent);
  virtual ~EntityWidget();

  void Set(const vkResourceLocator &resourceLocator);

private:

  entity::PreviewWidget *m_previewWidget;
  
  vkResourceLocator m_resourceLocator;
  
};

class EntityView : public EditorView
{
public:
  EntityView(QWidget *parent);
  virtual ~EntityView();

  void Initialize();
  virtual bool Close();

private:
  EntityWidget *m_entityWidget;
};

VK_CLASS()
class EntityViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  EntityViewFactory();
  virtual ~EntityViewFactory();


  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};
