#pragma once

#include <qwidget.h>
#include <EditorView.hh>
#include <StaticMesh/StaticMeshView.refl.hh>
#include <ui_StaticMeshView.h>

class vkResourceLocator;
namespace staticmesh
{
class PreviewWidget;
}


class StaticMeshWidget : public QWidget
{
  Q_OBJECT
public:
  StaticMeshWidget(QWidget *parent);
  virtual ~StaticMeshWidget();

  void Set(const vkResourceLocator &resourceLocator);

private slots:
void on_pbRenderGeometry_toggled(bool checked);
void on_pbRenderCollision_toggled(bool checked);
private:
  Ui::StaticMeshWidget m_gui;

  
  vkResourceLocator m_resourceLocator;
  
};

class StaticMeshView : public EditorView
{
public:
  StaticMeshView(QWidget *parent);
  virtual ~StaticMeshView();

  void Initialize();
  virtual bool Close();

private:
  StaticMeshWidget *m_entityWidget;
};

VK_CLASS()
class StaticMeshViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  StaticMeshViewFactory();
  virtual ~StaticMeshViewFactory();


  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};
