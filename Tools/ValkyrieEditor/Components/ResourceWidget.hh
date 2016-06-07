
#pragma once

#include <QLineEdit>
#include <QList>
#include <Valkyrie/Core/ResourceManager.hh>


class ResourceWidget : public QLineEdit
{
  Q_OBJECT
public:
  ResourceWidget(QWidget* parent = 0);
  virtual ~ResourceWidget ();

  void AddValidType(const QString &validType);

  void SetResourceLocator (const vkResourceLocator &locator);
  const vkResourceLocator &GetResourceLocator () const;

signals:
  void ResourceChanged (const vkResourceLocator &locator);

protected:
  virtual void dragEnterEvent(QDragEnterEvent *event);
  virtual void dragMoveEvent(QDragMoveEvent *event);
  virtual void dropEvent(QDropEvent *event);


private:
  bool m_lastValidDrag;

  QList<QString> m_validTypes;

  vkResourceLocator m_locator;

  void UpdateText();
};

