#pragma once

#include <editor/editorexport.hh>
#include <editor/components/nodegraph/nodegraphnodepropertytype.hh>

#include <QFont>
#include <QRectF>
#include <QString>

class QPainter;
class NodeGraphNode;
class NodeGraphNodeAnchor;
class EDITOR_API NodeGraphNodeProperty
{
public:
  NodeGraphNodeProperty(NodeGraphNode *node);
  ~NodeGraphNodeProperty();

  virtual void Initialize() { }

  virtual QRectF GetMinSize() = 0;
  virtual void Paint(QPainter *painter) = 0;
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const = 0;
  virtual void CollectAllAnchors(QList<NodeGraphNodeAnchor*> &result) const = 0;

  virtual void SetAllAnchorsDisconnected() = 0;

  void SetFont(const QFont &font)
  {
    m_font = font;
    m_minSizeDirty = true;
  }

  void SetType(NodeGraphNodePropertyType type)
  {
    m_type = type;
    m_minSizeDirty = true;
    Initialize();
  }

  NodeGraphNodePropertyType GetType() const
  {
    return m_type;
  }

  void SetName(const QString &name)
  {
    m_name = name;
    m_minSizeDirty = true;
  }
  const QString &GetName() const
  {
    return m_name;
  }


  void SetBounds(const QRectF &bounds)
  {
    m_bounds = bounds;
    UpdateBounds();
  }

  NodeGraphNode *GetNode() const
  {
    return m_node;
  }

  void SetIdx(int idx)
  {
    m_idx = idx;
  }

  int GetIdx() const
  {
    return m_idx;
  }

protected:
  virtual void UpdateBounds() { }

protected:
  NodeGraphNode *m_node;
  NodeGraphNodePropertyType m_type;
  QString m_name;
  QFont m_font;
  bool m_minSizeDirty;
  QRectF m_minSize;

  QRectF m_bounds;
  int m_idx;
};