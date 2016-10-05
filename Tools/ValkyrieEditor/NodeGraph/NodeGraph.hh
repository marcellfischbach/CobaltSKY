
#ifndef __NODEGRAPH_HH__
#define __NODEGRAPH_HH__

#include <EditorView.hh>
#include <QWidget>
#include <QMap>

#include <Valkyrie/Core/ResourceManager.hh>
#include <NodeGraph/NodeGraph.refl.hh>

class NodeGraphNodeInput : public QObject
{
  Q_OBJECT
public:
  NodeGraphNodeInput ();
  virtual ~NodeGraphNodeInput();

};

class NodeGraphNode : public QWidget
{
  Q_OBJECT
public:
  virtual ~NodeGraphNode();

protected:
  NodeGraphNode ();

private:
  QMap<QString, NodeGraphNodeInput*> m_inputs;
};

class NodeGraphWidget : public QWidget
{
  Q_OBJECT
public:
  NodeGraphWidget(QWidget *parent);
  virtual ~NodeGraphWidget();

  void Set(const vkResourceLocator &resourceLocator);

protected:
  void paintEvent(QPaintEvent *event);

private:
  vkResourceLocator m_resourceLocator;

  QColor m_backgroundColor;
  QColor m_gridColor;

};


class NodeGraphView : public EditorView
{
public:
  NodeGraphView(QWidget *parent);
  virtual ~NodeGraphView();

  void Initialize();
  virtual bool Close();

private:
  NodeGraphWidget *m_nodeGraphWidget;
};

VK_CLASS()
class NodeGraphViewFactory : public IEditorViewFactory
{
  VK_CLASS_GEN_OBJECT;
public:
  NodeGraphViewFactory();
  virtual ~NodeGraphViewFactory();


  virtual bool CanEdit(const vkResourceLocator &resourceLocator, IObject *obj);
  virtual EditorView *CreateView(QWidget *parent, const vkResourceLocator &resourceLocator, IObject *obj);

};


#endif // __NODEGRAPH_HH__
