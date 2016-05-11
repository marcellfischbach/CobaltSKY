#pragma once

#include <qwidget.h>
#include <Valkyrie/Core/ResourceManager.hh>
#include <Valkyrie/Core/Object.hh>
#include <EditorView.refl.hh>

class EditorView
{
public:
  virtual ~EditorView();

  QWidget *GetWidget();
  const QString &GetName() const;

  void SetResourceLocator(const vkResourceLocator &resourceLocator);
  const vkResourceLocator &GetResourceLocator() const;

  void SetEditorObject(IObject *editorObject);
  const IObject *GetEditorObject() const;

  bool CanClose() const;
  virtual bool Close();

protected:
  EditorView();

  void SetWidget(QWidget *widget);
  void SetName(const QString &name);
  void SetCanClose(bool canClose);

private:
  vkResourceLocator m_resourceLocator;
  IObject *m_editorObject;

  QWidget *m_widget;
  QString m_name;
  bool m_canClose;

};

VK_INTERFACE()
struct IEditorViewFactory : public IObject
{
  VK_CLASS_GEN;

  virtual ~IEditorViewFactory() { }

  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj) = 0;

  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj) = 0;
};


class EditorViewFactoryManager
{
public:
  static EditorViewFactoryManager *Get();

  IEditorViewFactory *GetFactory(const vkResourceLocator &resourceLocator, IObject *obj);

private:
  EditorViewFactoryManager();

  QList<IEditorViewFactory*> m_factories;
};