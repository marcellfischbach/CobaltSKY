#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csspatialnode.hh>
#include <cobalt/graphics/scene/cscameranode.refl.hh>


class csCamera;

CS_CLASS()
class CSE_API csCameraNode : public CS_SUPER(csSpatialNode)
{
  CS_CLASS_GEN;
public:
  csCameraNode();
  virtual ~csCameraNode();

  void SetCamera(csCamera *camera);
  const csCamera *GetCamera() const;
  csCamera *GetCamera();

  

protected:
  virtual void TransformationChanged ();

private:
  csCamera *m_camera;


};

CS_FORCEINLINE csCamera *csCameraNode::GetCamera()
{
  return m_camera;
}

CS_FORCEINLINE const csCamera *csCameraNode::GetCamera() const
{
  return m_camera;
}

