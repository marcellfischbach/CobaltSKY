#pragma once

#include <nodegraph/nodegraphnodeproperty.hh>
#include <QImage>

class NodeGraphNodeImageProperty : public NodeGraphNodeProperty
{
public:
  NodeGraphNodeImageProperty();
  virtual ~NodeGraphNodeImageProperty();

  virtual QRectF GetMinSize();
  virtual void Paint(QPainter *painter);
  virtual NodeGraphNodeAnchor *GetAnchor(const QPointF &point) const { return 0; }

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

