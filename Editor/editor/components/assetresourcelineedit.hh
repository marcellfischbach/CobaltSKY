#pragma once

#include <editor/editorexport.hh>
#include <QLineEdit>
#include <QList>
#include <vector>

class csClass;
class csResourceLocator;
namespace asset::model
{
  class Entry;
}

class EDITOR_API AssetResourceLineEdit : public QLineEdit
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
  void get(QDataStream &stream, std::vector<asset::model::Entry*> &entries) const;
  QList<const csClass*> m_validClasses;
};