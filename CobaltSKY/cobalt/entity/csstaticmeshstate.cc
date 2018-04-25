
#include <cobalt/entity/csstaticmeshstate.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/ientityscan.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/imesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/physics/iphysicsscene.hh>
#include <cobalt/physics/iphysicsdynamiccollider.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <cobalt/csengine.hh>

csStaticMeshState::csStaticMeshState()
  : csRenderState()
  , m_mesh(0)
  , m_castShadow(true)
  , m_colliderShape(0)
  , m_friction(0.5)
  , m_restitution(0.5)
  , m_enableCollision(false)
  , m_colliderType(ePCT_Static)
  , m_autoInertia(true)
  , m_inertia(csVector3f(1.0f, 1.0f, 1.0f))
{

}

csStaticMeshState::~csStaticMeshState()
{
  CS_RELEASE(m_mesh);
  m_mesh = 0;

  for (csMaterial *material : m_materials)
  {
    CS_RELEASE(material);
  }
  m_materials.clear();

  CS_RELEASE(m_colliderShape);
}


void csStaticMeshState::SetMesh(iMesh *mesh)
{
  CS_SET(m_mesh, mesh);

  UpdateMaterialSlots();

  UpdateBoundingBox();
}

void csStaticMeshState::SetMaterial(csSize idx, csMaterial *material)
{
  if (idx >= m_materials.size())
  {
    return;
  }

  CS_SET(m_materials[idx], material);
}

void csStaticMeshState::SetColliderShape(csPhysicsShape *colliderShape)
{
  CS_SET(m_colliderShape, colliderShape);
}


void csStaticMeshState::UpdateMaterialSlots()
{
  csSize numMaterials = m_mesh->GetNumberOfMaterials();
  if (numMaterials == m_materials.size())
  {
    return;
  }

  // create new material slots
  csMaterial **materials = new csMaterial*[numMaterials];
  memset(materials, 0, sizeof(csMaterial*) * numMaterials);

  for (csUInt32 i= m_materials.size(); i<numMaterials; ++i)
  {
    m_materials.push_back(0);
  }

  while (m_materials.size() > numMaterials)
  {
    csMaterial *material = m_materials.back();
    CS_RELEASE(material);
    m_materials.pop_back();
  }
}

/*
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
*/

void csStaticMeshState::SetFriction(float friction)
{
  m_friction = friction;
  if (m_colliderShape)
  {
    m_colliderShape->SetFriction(m_friction);
  }
}


void csStaticMeshState::SetRestitution(float restitution)
{
  m_restitution = restitution;
  if (m_colliderShape)
  {
    m_colliderShape->SetRestitution(m_restitution);
  }
}

void csStaticMeshState::UpdateTransformation()
{
  csSpatialState::UpdateTransformation();

  if (m_colliderShape)
  {
    m_colliderShape->GetTransform().SetGlobalTransformation(GetGlobalTransformation());
    m_colliderShape->FinishTransformation();
  }
}

iPhysicsBaseCollider *csStaticMeshState::GetCollider()
{
  if (!m_colliderShape)
  {
    if (!m_enableCollision)
    {
      return 0;
    }
    switch (m_colliderType)
    {
    case ePCT_Static:
      {
        iPhysicsDynamicCollider *dynCol = csEng->CreateDynamicCollider();
        dynCol->SetAutoInertia(m_autoInertia);
        dynCol->SetInertia(m_inertia);
        dynCol->SetMass(m_mass);
        m_collider = dynCol;
      }
      break;
    case ePCT_Dynamic:
    case ePCT_Kinematic:
      m_collider = csEng->CreateDynamicCollider();
      break;
    }


    m_collider->AttachShape(m_colliderShape);
    m_colliderShape->SetFriction(m_friction);
    m_colliderShape->SetRestitution(m_restitution);
  }

  return m_colliderShape;
}

void csStaticMeshState::OnAttachedToScene(csEntityScene *scene)
{
  if (scene && GetCollider())
  {
    scene->GetPhysicsScene()->AddCollider(m_colliderShape);
  }
}

void csStaticMeshState::OnDetachedFromScene(csEntityScene *scene)
{
  if (scene && GetCollider())
  {
    scene->GetPhysicsScene()->RemoveCollider(m_colliderShape);
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
    m_mesh->Render(graphics, pass, m_materials);
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
  if (!m_materials.empty() && m_mesh)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      m_mesh->Update(graphics, config.MainCameraPosition, config.FrameNo);
      entityScan->ScanRenderState(this);
    }
  }
}

