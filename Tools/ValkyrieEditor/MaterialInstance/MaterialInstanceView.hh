#pragma once

#include <EditorView.hh>
#include <ui_MaterialInstanceView.h>

#include <MaterialInstance/MaterialInstanceView.refl.hh>


class vkMaterialInstance;
class MaterialInstanceWidget : public QWidget
{
  Q_OBJECT
public:
  MaterialInstanceWidget (QWidget *parent);
  virtual ~MaterialInstanceWidget();

  void Set(const vkResourceLocator &resourceLocator);

private slots:
  void DataChanged ();

private:
  Ui::MaterialInstanceWidget m_gui;
  vkMaterialInstance *m_materialInstance;
};



class MaterialInstanceView : public EditorView
{
public:
  MaterialInstanceView(QWidget *parent);
  virtual ~MaterialInstanceView();

  void Initialize();
  virtual bool Close();

private:
  MaterialInstanceWidget *m_materialInstanceWidget;
};


VK_CLASS()
class MaterialInstanceViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  MaterialInstanceViewFactory();
  virtual ~MaterialInstanceViewFactory ();

  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};

