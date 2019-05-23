#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/scene/iscancallback.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/graphics/scene/csnode.refl.hh>



namespace cs
{
struct iGraphics;
class GroupNode;

CS_CLASS()
class CSE_API Node : public CS_SUPER(cs::Object)
{
  friend class cs::GroupNode;
  CS_CLASS_GEN;
public:
  enum UpdateFlag
  {
    eUF_UpdateBoundingBox = 0x01,
  };

public:
  virtual ~Node();

  CS_FORCEINLINE void SetName(const std::string & name)
  {
    m_name = name;
  }

  CS_FORCEINLINE const std::string& GetName() const
  {
    return m_name;
  }

  CS_FORCEINLINE cs::BoundingBox& GetBoundingBox()
  {
    return m_boundingBox;
  }

  CS_FORCEINLINE const cs::BoundingBox& GetBoundingBox() const
  {
    return m_boundingBox;
  }

  CS_FORCEINLINE cs::GroupNode* GetParent()
  {
    return m_parent;
  }

  CS_FORCEINLINE const cs::GroupNode* GetParent() const
  {
    return m_parent;
  }

  bool AttachTo(cs::GroupNode * parent);
  void DetachFromParent();

  virtual void Scan(const cs::Clipper * clipper, cs::iGraphics * renderer, cs::iScanCallback * callback);

  virtual void UpdateStates();

protected:
  Node();

  virtual void PrivScan(const cs::Clipper * clipper, cs::iGraphics * renderer, cs::iScanCallback * callback);

  void FlagUpdateBoundingBox();

  virtual void UpdateBoundingBox(cs::BoundingBox & bbox);

private:
  void SetParent(cs::GroupNode * parent);

  csUInt32 m_updateFlags;

  cs::BoundingBox m_boundingBox;

  cs::GroupNode* m_parent;

  std::string m_name;
};

}

