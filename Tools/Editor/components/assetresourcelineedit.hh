#pragma once

#include <QLineEdit>
#include <QList>

class csClass;
class csResourceLocator;
class AssetResourceLineEdit : public QLineEdit
{
  Q_OBJECT
public:
  AssetResourceLineEdit(QWidget *parent = Q_NULLPTR);
  AssetResourceLineEdit(const QString &contents, QWidget *parent = Q_NULLPTR);
  ~AssetResourceLineEdit();
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dropEvent(QDropEvent *event);

  void AddValidClass(const csClass *cls);

signals:
  void ResourceChanged(const csResourceLocator &locator);
private:
  QList<const csClass*> m_validClasses;
};