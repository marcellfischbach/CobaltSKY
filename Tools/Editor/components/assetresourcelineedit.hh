#pragma once

#include <QLineEdit>
#include <QList>

class vkClass;
class vkResourceLocator;
class AssetResourceLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  AssetResourceLineEdit(QWidget *parent = Q_NULLPTR);
  AssetResourceLineEdit(const QString &contents, QWidget *parent = Q_NULLPTR);
  ~AssetResourceLineEdit();
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent *event);

  void AddValidClass(const vkClass *cls);

signals:
  void ResourceChanged(const vkResourceLocator &locator);
private:
  QList<const vkClass*> m_validClasses;
};