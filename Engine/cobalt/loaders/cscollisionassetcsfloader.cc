

#include <cobalt/loaders/cscollisionassetcsfloader.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/csphysicsshape.hh>
#include <cobalt/csengine.hh>
#include <cobalt/csenums.hh>

csCollisionAssetCSFLoader::csCollisionAssetCSFLoader()
  : csBaseCSFLoader()
{
}

csCollisionAssetCSFLoader::~csCollisionAssetCSFLoader()
{

}


bool csCollisionAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("collider") ||
    entry->GetTagName() == std::string("shapes");
}

const cs::Class *csCollisionAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return csPhysicsShape::GetStaticClass();
}

csResourceWrapper *csCollisionAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  csPhysicsShape *container = new csPhysicsShape();

  const csfEntry *shapesEntry = nullptr;
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
    return new csPhysicsShapeWrapper(container);
  }



  for (const csfEntry *shapeEntry = shapesEntry->GetEntry("shape");
       shapeEntry;
       shapeEntry = shapeEntry->GetSiblingEntry("shape"))
  {

    csMatrix4f localTransform;
    localTransform.SetIdentity();
    csPhysGeometry geometryDesc;
    memset(&geometryDesc, 0, sizeof(geometryDesc));
    geometryDesc.Type = (csPhysGeometryType)-1;

    for (const csfEntry *child = shapeEntry->GetEntry();
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
    iPhysicsShape *shape = csEng->CreateShape(geometryDesc);
    shape->SetLocalTransform(localTransform);
    container->AddShape(shape);
  }

  return new csPhysicsShapeWrapper(container);
}


void csCollisionAssetCSFLoader::LoadTransform(const csfEntry *transformEntry, csMatrix4f &localTransform) const
{
  const csfEntry *matrixEntry = transformEntry->GetEntry("matrix");
  if (!matrixEntry)
  {
    return;
  }


  csVector4f r0, r1, r2, r3;
  const csfEntry *row0 = matrixEntry->GetEntry("row0");
  const csfEntry *row1 = matrixEntry->GetEntry("row1");
  const csfEntry *row2 = matrixEntry->GetEntry("row2");
  const csfEntry *row3 = matrixEntry->GetEntry("row3");
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

void csCollisionAssetCSFLoader::LoadBox(const csfEntry *transformEntry, csPhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("halfX") &&
      transformEntry->HasAttribute("halfY") &&
      transformEntry->HasAttribute("halfZ"))
  {
    geometry.Type = ePGT_Box;
    geometry.Dimensions.x = transformEntry->GetAttributeFloat("halfX") * 2.0f;
    geometry.Dimensions.y = transformEntry->GetAttributeFloat("halfY") * 2.0f;
    geometry.Dimensions.z = transformEntry->GetAttributeFloat("halfZ") * 2.0f;
  }
}


void csCollisionAssetCSFLoader::LoadCylinder(const csfEntry *transformEntry, csPhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("radius") &&
      transformEntry->HasAttribute("halfHeight"))
  {
    geometry.Type = ePGT_CylinderZ;
    geometry.Radius = transformEntry->GetAttributeFloat("radius");
    geometry.Height = transformEntry->GetAttributeFloat("halfHeight") * 2.0f;
  }
}

void csCollisionAssetCSFLoader::LoadSphere(const csfEntry *transformEntry, csPhysGeometry &geometry) const
{
  if (transformEntry->HasAttribute("radius"))
  {
    geometry.Type = ePGT_Sphere;
    geometry.Radius = transformEntry->GetAttributeFloat("radius");
  }
}


