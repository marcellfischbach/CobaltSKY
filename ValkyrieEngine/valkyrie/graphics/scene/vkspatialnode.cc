

#include <valkyrie/graphics/scene/vkspatialnode.hh>


vkSpatialNode::vkSpatialNode()
  : vkNode()
{
}

vkSpatialNode::~vkSpatialNode()
{

}


void vkSpatialNode::SetMatrix(const vkMatrix4f &matrix)
{
  m_matrix.Set(matrix);
  FlagUpdateBoundingBox();
  TransformationChanged();
}

void vkSpatialNode::TransformationChanged()
{

}