#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/scene/iscancallback.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csclipper.hh>
#include <cobalt/graphics/scene/csnode.refl.hh>


class csGroupNode;
struct iGraphics;

CS_INTERFACE()
class CSE_API csNode : public CS_SUPER(csObject)
{
  friend class csGroupNode;
  CS_CLASS_GEN;
public:
  enum UpdateFlag
  {
    eUF_UpdateBoundingBox = 0x01,
  };

public:
  virtual ~csNode();

  CS_FORCEINLINE void SetName(const std::string &name)
  {
    m_name = name;
  }

  CS_FORCEINLINE const std::string &GetName() const
  {
    return m_name;
  }

  CS_FORCEINLINE csBoundingBox &GetBoundingBox()
  {
    return m_boundingBox;
  }

  CS_FORCEINLINE const csBoundingBox &GetBoundingBox() const
  {
    return m_boundingBox;
  }

  CS_FORCEINLINE csGroupNode *GetParent()
  {
    return m_parent;
  }

  CS_FORCEINLINE const csGroupNode *GetParent() const
  {
    return m_parent;
  }

  bool AttachTo(csGroupNode *parent);
  void DetachFromParent();

  virtual void Scan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  virtual void UpdateStates();

protected:
  csNode();

  virtual void PrivScan (const csClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  void FlagUpdateBoundingBox();

  virtual void UpdateBoundingBox(csBoundingBox &bbox);

private:
  void SetParent(csGroupNode *parent);

  csUInt32 m_updateFlags;

  csBoundingBox m_boundingBox;

  csGroupNode *m_parent;

  std::string m_name;
};
