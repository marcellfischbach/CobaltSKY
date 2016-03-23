#pragma once

#include <qobject.h>
#include <qgraphicsitem.h>


class ShaderGraphNode;
class ShaderGraphConnection : public QObject
{
  Q_OBJECT
public:
  ShaderGraphConnection(ShaderGraphNode *out, int outIndex, ShaderGraphNode *in, int inIndex);

  void UpdateValues ();
  QGraphicsPathItem *GetItem ();

private:
  ShaderGraphNode *m_out;
  int m_outIndex;

  ShaderGraphNode *m_in;
  int m_inIndex;

  QGraphicsPathItem *m_pathItem;

};

