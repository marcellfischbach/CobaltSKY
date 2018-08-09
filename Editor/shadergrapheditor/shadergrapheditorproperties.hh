#pragma once

#include <QWidget>
#include <map>
#include <vector>

class QDoubleSpinBox;
class QGridLayout;
class QScrollArea;
class QSpinBox;
class ShaderGraphEditorNode;
class csResourceLocator;
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
  void ResourceChanged(const csResourceLocator &locator);

signals:
  void NodeChanged(ShaderGraphEditorNode *node);

private:
  void Cleanup();
  ShaderGraphEditorNode *m_node;
  QWidget *m_group;
  QScrollArea *m_scrollArea;
  std::map<unsigned, QDoubleSpinBox*> m_spinBoxes;
  std::vector<QDoubleSpinBox*> m_spinBoxDefaultFloatResource;
  std::vector<QSpinBox*> m_spinBoxDefaultIntResource;
};