#pragma once

#include <QWidget>
#include <map>
#include <vector>

class QDoubleSpinBox;
class QGridLayout;
class QSpinBox;
class ShaderGraphEditorNode;
class vkResourceLocator;
class ShaderGraphEditorProperties : public QWidget
{
  Q_OBJECT;
public:
  ShaderGraphEditorProperties();
  virtual ~ShaderGraphEditorProperties();


public slots:
  void SetNodes(const QList<ShaderGraphEditorNode*> &nodes);

private slots:
  void SpinBox_valueChanged(double value);
  void Resource_nameChanged(const QString &text);
  void DefaultFloat_valueChanged(double value);
  void DefaultInt_valueChanged(int value);
  void ResourceChanged(const vkResourceLocator &locator);

signals:
  void NodeChanged();

private:
  void Cleanup();
  ShaderGraphEditorNode *m_node;
  QWidget *m_group;
  std::map<unsigned, QDoubleSpinBox*> m_spinBoxes;
  std::vector<QDoubleSpinBox*> m_spinBoxDefaultFloatResource;
  std::vector<QSpinBox*> m_spinBoxDefaultIntResource;
};