
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

cs::StaticMeshState::StaticMeshState()
  : cs::RenderState()
  , m_mesh(0)
  , m_castShadow(true)
  , m_collider(0)
  , m_colliderShape(0)
  , m_friction(0.5)
  , m_restitution(0.5)
  , m_enableCollision(false)
  , m_colliderType(cs::ePCT_Static)
  , m_autoInertia(true)
  , m_inertia(cs::Vector3f(1.0f, 1.0f, 1.0f))
{

}

cs::StaticMeshState::~StaticMeshState()
{
  CS_RELEASE(m_mesh);
  m_mesh = 0;

  for (cs::Material *material : m_materials)
  {
    CS_RELEASE(material);
  }
  m_materials.clear();

  CS_RELEASE(m_colliderShape);
}


void cs::StaticMeshState::SetMesh(cs::iMesh *mesh)
{
  CS_SET(m_mesh, mesh);

  UpdateMaterialSlots();

  UpdateBoundingBox();
}

void cs::StaticMeshState::SetMaterial(csSize idx, cs::Material *material)
{
  if (idx >= m_materials.size())
  {
    return;
  }

  CS_SET(m_materials[idx], material);
}

void cs::StaticMeshState::SetColliderShape(cs::PhysicsShape *colliderShape)
{
  CS_SET(m_colliderShape, colliderShape);
}


void cs::StaticMeshState::UpdateMaterialSlots()
{
  csSize numMaterials = m_mesh->GetNumberOfMaterials();
  if (numMaterials == m_materials.size())
  {
    return;
  }

  // create new material slots
  cs::Material **materials = new cs::Material*[numMaterials];
  memset(materials, 0, sizeof(cs::Material*) * numMaterials);

  for (size_t i= m_materials.size(); i<numMaterials; ++i)
  {
    m_materials.push_back(0);
  }

  while (m_materials.size() > numMaterials)
  {
    cs::Material *material = m_materials.back();
    CS_RELEASE(material);
    m_materials.pop_back();
  }
}

/*
void cs::StaticMeshState::SetColliderShape(cs::PhysicsShapeContainer *shapes)
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

void cs::StaticMeshState::SetFriction(float friction)
{
  m_friction = friction;
  if (m_collider)
  {
    m_collider->SetFriction(m_friction);
  }
}


void cs::StaticMeshState::SetRestitution(float restitution)
{
  m_restitution = restitution;
  if (m_collider)
  {
    m_collider->SetRestitution(m_restitution);
  }
}

void cs::StaticMeshState::UpdateTransformation()
{
  cs::SpatialState::UpdateTransformation();

  if (m_collider)
  {
    m_collider->GetTransform().SetGlobalTransformation(GetGlobalTransformation());
    m_collider->FinishTransformation();
  }
}

cs::iPhysicsBaseCollider *cs::StaticMeshState::GetCollider()
{
  if (!m_collider)
  {
    if (!m_enableCollision)
    {
      return 0;
    }
    switch (m_colliderType)
    {
    case cs::ePCT_Static:
      {
        cs::iPhysicsDynamicCollider *dynCol = csEng->CreateDynamicCollider();
        dynCol->SetAutoInertia(m_autoInertia);
        dynCol->SetInertia(m_inertia);
        dynCol->SetMass(m_mass);
        m_collider = dynCol;
      }
      break;
    case cs::ePCT_Dynamic:
    case cs::ePCT_Kinematic:
      m_collider = csEng->CreateDynamicCollider();
      break;
    }


    m_collider->AttachShape(m_colliderShape);
    m_collider->SetFriction(m_friction);
    m_collider->SetRestitution(m_restitution);
  }

  return m_collider;
}

void cs::StaticMeshState::OnAttachedToScene(cs::EntityScene *scene)
{
  if (scene && GetCollider())
  {
    scene->GetPhysicsScene()->AddCollider(m_collider);
  }
}

void cs::StaticMeshState::OnDetachedFromScene(cs::EntityScene *scene)
{
  if (scene && GetCollider())
  {
    scene->GetPhysicsScene()->RemoveCollider(m_collider);
  }
}

void cs::StaticMeshState::FillBoundingBox(cs::BoundingBox &bbox)
{
  if (m_mesh)
  {
    bbox.Add(m_globalMatrix, m_mesh->GetBoundingBox());
  }

}

void cs::StaticMeshState::Render(cs::iGraphics *graphics, cs::eRenderPass pass) const
{
  if (m_mesh)
  {
    cs::RenderState::Render(graphics, pass);
    graphics->SetModelMatrix(GetGlobalTransformation());
    m_mesh->Render(graphics, pass, m_materials);
  }
}

size_t cs::StaticMeshState::GetNumberOfRenderCalls() const
{
  if (m_mesh)
  {
    return m_mesh->GetNumberOfRenderCalls();
  }
  return 0;
}


size_t cs::StaticMeshState::GetNumberOfTotalTrigons() const
{
  if (m_mesh)
  {
    return m_mesh->GetNumberOfTotalTrigons();
  }
  return 0;
}

void cs::StaticMeshState::PrivScan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
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

