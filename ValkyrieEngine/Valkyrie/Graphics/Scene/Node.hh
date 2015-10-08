#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/BoundingVolume.hh>
#include <Valkyrie/Graphics/Scene/Node.refl.hh>


class vkGroupNode;


VK_CLASS();
class VKE_API vkNode : public vkObject
{
  friend class vkGroupNode;
  VK_CLASS_GEN;
public:
  enum UpdateFlag
  {
    eUF_UpdateBoundingBox = 0x01,
  };

public:
  vkNode();
  virtual ~vkNode();


  VK_FORCEINLINE vkBoundingBox &GetBoundingBox()
  {
    return m_boundingBox;
  }

  VK_FORCEINLINE const vkBoundingBox &GetBoundingBox() const
  {
    return m_boundingBox;
  }

  VK_FORCEINLINE vkGroupNode *GetParent()
  {
    return m_parent;
  }

  VK_FORCEINLINE const vkGroupNode *GetParent() const
  {
    return m_parent;
  }

  bool AttachTo(vkGroupNode *parent);
  void DetachFromParent();

  virtual void UpdateStates();

protected:
  void FlagUpdateBoundingBox();

private:
  void SetParent(vkGroupNode *parent);

  virtual void UpdateBoundingBox();

  vkUInt32 m_updateFlags;

  vkBoundingBox m_boundingBox;

  vkGroupNode *m_parent;
};
