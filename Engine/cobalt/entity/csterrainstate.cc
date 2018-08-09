
#include <cobalt/entity/csterrainstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csterrainmesh.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

csTerrainState::csTerrainState()
  : csRenderState()
  , m_terrainMesh(0)
  , m_material(0)
  , m_castShadow(true)
  , m_staticCollider(0)
  , m_shapes(0)
  , m_friction(0.5)
  , m_restitution(0.5)
{

}

csTerrainState::~csTerrainState()
{
  CS_RELEASE(m_terrainMesh);
  m_terrainMesh = 0;

  CS_RELEASE(m_material);
  m_material = 0;

}


void csTerrainState::SetTerrainMesh(csTerrainMesh *terrainMesh)
{
  CS_SET(m_terrainMesh, terrainMesh);

  UpdateBoundingBox();
}

void csTerrainState::SetMaterial(csMaterial *material)
{
  CS_SET(m_material, material);
}



void csTerrainState::SetColliderShape(csPhysicsShapeContainer *shapes)
{
  if (shapes == m_shapes)
  {
    return;
  }
  if (!m_staticCollider)
  {
    m_staticCollider = csEng->CreateStaticCollider();
    m_staticCollider->SetFriction(m_friction);
    m_staticCollider->SetRestitution(m_restitution);
  }
  if (m_shapes)
  {
    m_staticCollider->DetachShape(m_shapes);
  }
  if (shapes)
  {
    m_staticCollider->AttachShape(shapes);
  }

  CS_SET(m_shapes, shapes);
}

void csTerrainState::SetFriction(float friction)
{
  if (!m_staticCollider)
  {
    m_staticCollider = csEng->CreateStaticCollider();
    m_staticCollider->AttachShape(m_shapes);
    m_staticCollider->SetRestitution(m_restitution);
  }

  m_friction = friction;
  m_staticCollider->SetFriction(m_friction);
}


void csTerrainState::SetRestitution(float restitution)
{
  if (!m_staticCollider)
  {
    m_staticCollider = csEng->CreateStaticCollider();
    m_staticCollider->AttachShape(m_shapes);
    m_staticCollider->SetFriction(m_friction);
  }

  m_restitution = restitution;
  m_staticCollider->SetRestitution(m_restitution);
}

void csTerrainState::UpdateTransformation()
{
  csSpatialState::UpdateTransformation();

  if (m_staticCollider)
  {
    m_staticCollider->GetTransform().SetGlobalTransformation(GetGlobalTransformation());
    m_staticCollider->FinishTransformation();
  }

}

void csTerrainState::OnAttachedToScene(csEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void csTerrainState::OnDetachedFromScene(csEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }
}

void csTerrainState::FillBoundingBox(csBoundingBox &bbox)
{
  if (m_terrainMesh)
  {
    bbox.Add(m_globalMatrix, m_terrainMesh->GetBoundingBox());
  }

}

void csTerrainState::Render(iGraphics *graphics, csRenderPass pass) const
{
  if (m_terrainMesh)
  {
    csRenderState::Render(graphics, pass);
    graphics->SetModelMatrix(GetGlobalTransformation());
    m_terrainMesh->Render(graphics, pass, m_material);
  }
}

unsigned csTerrainState::GetNumberOfRenderCalls() const
{
  if (m_terrainMesh)
  {
    return m_terrainMesh->GetNumberOfRenderCalls();
  }
  return 0;
}


unsigned csTerrainState::GetNumberOfTotalTrigons() const
{
  if (m_terrainMesh)
  {
    return m_terrainMesh->GetNumberOfTotalTrigons();
  }
  return 0;
}

void csTerrainState::PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (m_materials && m_terrainMesh)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      entityScan->ScanRenderState(this);
    }
  }
}

