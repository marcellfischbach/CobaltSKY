

#include <cobalt/loaders/cscollisionassetcsfloader.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/csphysicsshape.hh>
#include <cobalt/csengine.hh>
#include <cobalt/csenums.hh>

cs::CollisionAssetCSFLoader::CollisionAssetCSFLoader()
  : cs::BaseCSFLoader()
{
}

cs::CollisionAssetCSFLoader::~CollisionAssetCSFLoader()
{

}


bool cs::CollisionAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("collider") ||
    entry->GetTagName() == std::string("shapes");
}

const cs::Class *cs::CollisionAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::PhysicsShape::GetStaticClass();
}

cs::ResourceWrapper *cs::CollisionAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::PhysicsShape *container = new cs::PhysicsShape();

  const cs::file::Entry *shapesEntry = nullptr;
  if (entry->GetTagName() == std::string("shapes"))
  {
    shapesEntry = entry;
  }
  else
  {
    shapesEntry = entry->GetEntry("shapes");
  }
  if (!shapesEntry)
  {
    return new cs::PhysicsShapeWrapper(container);
  }



  for (const cs::file::Entry *shapeEntry = shapesEntry->GetEntry("shape");
       shapeEntry;
       shapeEntry = shapeEntry->GetSiblingEntry("shape"))
  {

    cs::Matrix4f localTransform;
    localTransform.SetIdentity();
    cs::PhysGeometry geometryDesc;
    memset(&geometryDesc, 0, sizeof(geometryDesc));
    geometryDesc.Type = (cs::ePhysGeometryType)-1;

    for (const cs::file::Entry *child = shapeEntry->GetEntry();
         child;
         child = child->GetSiblingEntry())
    {
      std::string tag = child->GetTagName();
      if (tag == std::string("transform"))
      {
        LoadTransform(child, localTransform);
      }
      else if (tag == "box")
      {
        LoadBox(child, geometryDesc);
      }
      else if (tag == "cylinder")
      {
        LoadCylinder(child, geometryDesc);
      }
      else if (tag == "sphere")
      {
        LoadSphere(child, geometryDesc);
      }
    }
    if (geometryDesc.Type == -1)
    {
      continue;
    }
    cs::iPhysicsShape *shape = csEng->CreateShape(geometryDesc);
    shape->SetLocalTransform(localTransform);
    container->AddShape(shape);
  }

  return new cs::PhysicsShapeWrapper(container);
}


void cs::CollisionAssetCSFLoader::LoadTransform(const cs::file::Entry *transformEntry, cs::Matrix4f &localTransform) const
{
  const cs::file::Entry *matrixEntry = transformEntry->GetEntry("matrix");
  if (!matrixEntry)
  {
    return;
  }


  cs::Vector4f r0, r1, r2, r3;
  const cs::file::Entry *row0 = matrixEntry->GetEntry("row0");
  const cs::file::Entry *row1 = matrixEntry->GetEntry("row1");
  const cs::file::Entry *row2 = matrixEntry->GetEntry("row2");
  const cs::file::Entry *row3 = matrixEntry->GetEntry("row3");
  if (!row0 || !row1 || !row2 || !row3)
  {
    return;
  }
  r0 = LoadVector4f(row0);
  r1 = LoadVector4f(row1);
  r2 = LoadVector4f(row2);
  r3 = LoadVector4f(row3);
  localTransform.Set(
    r0.x, r0.y, r0.z, r0.w,
    r1.x, r1.y, r1.z, r1.w,
    r2.x, r2.y, r2.z, r2.w,
    r3.x, r3.y, r3.z, r3.w
  );
}

void cs::CollisionAssetCSFLoader::LoadBox(const cs::file::Entry *transformEntry, cs::PhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("halfX") &&
      transformEntry->HasAttribute("halfY") &&
      transformEntry->HasAttribute("halfZ"))
  {
    geometry.Type = cs::ePGT_Box;
    geometry.Dimensions.x = transformEntry->GetAttributeFloat("halfX") * 2.0f;
    geometry.Dimensions.y = transformEntry->GetAttributeFloat("halfY") * 2.0f;
    geometry.Dimensions.z = transformEntry->GetAttributeFloat("halfZ") * 2.0f;
  }
}


void cs::CollisionAssetCSFLoader::LoadCylinder(const cs::file::Entry *transformEntry, cs::PhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("radius") &&
      transformEntry->HasAttribute("halfHeight"))
  {
    geometry.Type = cs::ePGT_CylinderZ;
    geometry.Radius = transformEntry->GetAttributeFloat("radius");
    geometry.Height = transformEntry->GetAttributeFloat("halfHeight") * 2.0f;
  }
}

void cs::CollisionAssetCSFLoader::LoadSphere(const cs::file::Entry *transformEntry, cs::PhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("radius"))
  {
    geometry.Type = cs::ePGT_Sphere;
    geometry.Radius = transformEntry->GetAttributeFloat("radius");
  }
}


