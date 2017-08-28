
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/imesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/csengine.hh>

csStaticMeshState::csStaticMeshState()
  : csRenderState()
  , m_mesh(0)
  , m_materials(0)
  , m_numberOfMaterialSlots(0)
  , m_castShadow(true)
  , m_staticCollider(0)
  , m_shapes(0)
  , m_friction(0.5)
  , m_restitution(0.5)
{

}

csStaticMeshState::~csStaticMeshState()
{
  CS_RELEASE(m_mesh);
  m_mesh = 0;

  for (csUInt32 i = 0; i < m_numberOfMaterialSlots; ++i)
  {
    CS_RELEASE(m_materials[i]);
    m_materials[i] = 0;
  }

  delete[] m_materials;
  m_materials = 0;

  m_numberOfMaterialSlots = 0;
}


void csStaticMeshState::SetMesh(iMesh *mesh)
{
  CS_SET(m_mesh, mesh);

  UpdateMaterialSlots();

  UpdateBoundingBox();
}

void csStaticMeshState::SetMaterial(csMaterial *material, csSize slot)
{
  if (slot >= m_numberOfMaterialSlots)
  {
    return;
  }

  CS_SET(m_materials[slot], material);
}


void csStaticMeshState::UpdateMaterialSlots()
{
  csSize numberOfSlots = m_mesh->GetNumberOfMaterials();
  if (numberOfSlots == m_numberOfMaterialSlots)
  {
    return;
  }

  // create new material slots
  csMaterial **materials = new csMaterial*[numberOfSlots];
  memset(materials, 0, sizeof(csMaterial*) * numberOfSlots);

  // copy all slots that remain from the old to the new
  for (csUInt32 i = 0; i < numberOfSlots && i < m_numberOfMaterialSlots; ++i)
  {
    CS_SET(materials[i], m_materials[i]);
  }

  for (csUInt32 i = 0; i < m_numberOfMaterialSlots; ++i)
  {
    CS_RELEASE(m_materials[i]);
    m_materials[i] = 0;
  }

  delete[] m_materials;
  m_materials = materials;
  m_numberOfMaterialSlots = numberOfSlots;
}

void csStaticMeshState::SetColliderShape(csPhysicsShapeContainer *shapes)
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

void csStaticMeshState::SetFriction(float friction)
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


void csStaticMeshState::SetRestitution(float restitution)
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

void csStaticMeshState::UpdateTransformation()
{
  csSpatialState::UpdateTransformation();

  if (m_staticCollider)
  {
    m_staticCollider->GetTransform().SetGlobalTransformation(GetGlobalTransformation());
    m_staticCollider->FinishTransformation();
  }

}

void csStaticMeshState::OnAttachedToScene(csEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->AddStaticCollider(m_staticCollider);
  }
}

void csStaticMeshState::OnDetachedFromScene(csEntityScene *scene)
{
  if (m_staticCollider && scene)
  {
    scene->GetPhysicsScene()->RemoveStaticCollider(m_staticCollider);
  }
}

void csStaticMeshState::FillBoundingBox(csBoundingBox &bbox)
{
  if (m_mesh)
  {
    bbox.Add(m_globalMatrix, m_mesh->GetBoundingBox());
  }

}

void csStaticMeshState::Render(iGraphics *graphics, csRenderPass pass) const
{
  if (m_mesh)
  {
    csRenderState::Render(graphics, pass);
    graphics->SetModelMatrix(GetGlobalTransformation());
    m_mesh->Render(graphics, pass, m_numberOfMaterialSlots, m_materials);
  }
}

unsigned csStaticMeshState::GetNumberOfRenderCalls() const
{
  if (m_mesh)
  {
    return m_mesh->GetNumberOfRenderCalls();
  }
  return 0;
}


unsigned csStaticMeshState::GetNumberOfTotalTrigons() const
{
  if (m_mesh)
  {
    return m_mesh->GetNumberOfTotalTrigons();
  }
  return 0;
}

void csStaticMeshState::PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (m_materials && m_mesh)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      m_mesh->Update(graphics, config.MainCameraPosition, config.FrameNo);
      entityScan->ScanRenderState(this);
    }
  }
}

