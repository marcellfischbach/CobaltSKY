

#include <cobalt/graphics/scene/csspatialnode.hh>


csSpatialNode::csSpatialNode()
  : csNode()
{
}

csSpatialNode::~csSpatialNode()
{

}


void csSpatialNode::SetMatrix(const csMatrix4f &matrix)
{
  m_matrix.Set(matrix);
  FlagUpdateBoundingBox();
  TransformationChanged();
}

void csSpatialNode::TransformationChanged()
{

}