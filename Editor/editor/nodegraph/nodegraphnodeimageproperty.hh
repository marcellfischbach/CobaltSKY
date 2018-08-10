#pragma once

#include <editor/editorexport.hh>
#include <editor/nodegraph/nodegraphnodeproperty.hh>
#include <QImage>

class EDITOR_API NodeGraphNodeImageProperty : public NodeGraphNodeProperty
{
public:
  NodeGraphNodeImageProperty(NodeGraphNode *node);
  virtual ~NodeGraphNodeImageProperty();

  virtual QRectF GetMinSize();
  virtual void Paint(QPainter *painter);
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const { return 0; }
  virtual void CollectAllAnchors(QList<NodeGraphNodeAnchor*> &result) const { }
  virtual void SetAllAnchorsDisconnected() {}

  void SetImage(const QImage &image)
  {
    m_image = image;
    m_minSizeDirty = true;
  }

  const QImage &GetImage() const
  {
    return m_image;
  }

private:
  QImage m_image;
};

