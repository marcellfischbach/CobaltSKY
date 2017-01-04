#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkclass.hh>
#include <Valkyrie/Graphics/Scene/Scan.hh>
#include <Valkyrie/Math/BoundingVolume.hh>
#include <Valkyrie/Math/Clipper.hh>
#include <Valkyrie/Graphics/Scene/Node.refl.hh>


class vkGroupNode;
struct IGraphics;

VK_INTERFACE()
class VKE_API vkNode : public VK_SUPER(vkObject)
{
  friend class vkGroupNode;
  VK_CLASS_GEN;
public:
  enum UpdateFlag
  {
    eUF_UpdateBoundingBox = 0x01,
  };

public:
  virtual ~vkNode();

  VK_FORCEINLINE void SetName(const vkString &name)
  {
    m_name = name;
  }

  VK_FORCEINLINE const vkString &GetName() const
  {
    return m_name;
  }

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

  virtual void Scan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback);

  virtual void UpdateStates();

protected:
  vkNode();

  virtual void PrivScan (const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback);

  void FlagUpdateBoundingBox();

  virtual void UpdateBoundingBox(vkBoundingBox &bbox);

private:
  void SetParent(vkGroupNode *parent);

  vkUInt32 m_updateFlags;

  vkBoundingBox m_boundingBox;

  vkGroupNode *m_parent;

  vkString m_name;
};
