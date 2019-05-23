

#include <cobalt/graphics/scene/csspatialnode.hh>


cs::SpatialNode::SpatialNode()
  : cs::Node()
{
}

cs::SpatialNode::~SpatialNode()
{

}


void cs::SpatialNode::SetMatrix(const cs::Matrix4f &matrix)
{
  m_matrix.Set(matrix);
  FlagUpdateBoundingBox();
  TransformationChanged();
}

void cs::SpatialNode::TransformationChanged()
{

}