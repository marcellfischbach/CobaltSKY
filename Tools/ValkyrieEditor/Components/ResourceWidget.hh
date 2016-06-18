
#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <Valkyrie/Core/ResourceManager.hh>

class ResourceLineEdit;

class ResourceWidget : public QWidget
{
  friend class ResourceLineEdit;
  Q_OBJECT
public:
  ResourceWidget(QWidget* parent = 0);
  virtual ~ResourceWidget ();

  void SetAllValid(bool allValid);
  void AddValidType(const QString &validType);

  void SetResourceLocator (const vkResourceLocator &locator);
  const vkResourceLocator &GetResourceLocator () const;

  void SetEnabled (bool enabled);

signals:
  void ResourceChanged (const vkResourceLocator &locator);

private slots:
  void Clear (bool);

private:
  ResourceLineEdit *m_lineEdit;
  QToolButton *m_clearButton;

  bool m_allValid;
  QList<QString> m_validTypes;

  vkResourceLocator m_locator;

  void UpdateText();
};

