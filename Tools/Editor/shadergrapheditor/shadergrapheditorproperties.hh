#pragma once

#include <QWidget>
#include <map>

class QDoubleSpinBox;
class QGridLayout;
class ShaderGraphEditorNode;
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

signals:
  void NodeChanged();

private:
  void Cleanup();
  ShaderGraphEditorNode *m_node;
  QWidget *m_group;
  std::map<unsigned, QDoubleSpinBox*> m_spinBoxes;
};