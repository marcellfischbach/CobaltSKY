#ifndef SHADEREDITOR_HH
#define SHADEREDITOR_HH

#include <QWidget>
#include <ui_ShaderEditor.h>

namespace shadergraph
{


class SGNode;
class SGShaderGraphNode;


class ShaderEditorWidget : public QWidget
{
  Q_OBJECT
public:
  ShaderEditorWidget(QWidget *parent = 0);

  void SetShader (SGShaderGraphNode *shaderGraph);

private:
  void SyncFromGraph ();
  void SyncToGraph ();


  Ui::ShaderEditor m_gui;
  SGShaderGraphNode *m_shader;

  bool m_syncGuard;
private slots:
  void on_cbBlendBinaryGradient_stateChanged(int state);
  void on_cbDiscardAlpha_stateChanged(int state);
  void on_cbDiscardAlphaCompareMode_currentIndexChanged(int index);
  void on_sbDiscardAlphaThreshold_valueChanged(double value);
};

class NodeEditorWidget : public QWidget
{
  Q_OBJECT
public:
  NodeEditorWidget(QWidget *parent = 0);

  void SetNode (SGNode *node);

private:
  void SyncFromNode ();
  void SyncToNode ();

  SGNode *m_node;

  bool m_syncGuard;
  QLineEdit *m_leResourceName;

  QDoubleSpinBox *m_spResource0;
  QDoubleSpinBox *m_spResource1;
  QDoubleSpinBox *m_spResource2;
  QDoubleSpinBox *m_spResource3;
  QLineEdit *m_leResourceValue;\

  QDoubleSpinBox **m_constInputs;

private slots:
  void doubleChanged(double value);
  void textChanged(const QString &text);

};

}

#endif // SHADEREDITOR_HH
