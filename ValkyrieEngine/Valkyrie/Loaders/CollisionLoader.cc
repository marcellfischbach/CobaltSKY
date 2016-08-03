

#include <Valkyrie/Loaders/CollisionLoader.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Enums.hh>

vkCollisionAssetXMLLoader::vkCollisionAssetXMLLoader()
  : vkBaseXMLLoader()
{
}

vkCollisionAssetXMLLoader::~vkCollisionAssetXMLLoader()
{

}


bool vkCollisionAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("collision");
}

const vkClass *vkCollisionAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkPhysicsShapeContainer::GetStaticClass();
}

IObject *vkCollisionAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkPhysicsShapeContainer *container = new vkPhysicsShapeContainer();

  TiXmlElement *shapesElement = element->FirstChildElement("shapes");
  if (!shapesElement)
  {
    return container;
  }

  IPhysicsSystem *phys = vkEngine::Get()->GetPhysicsSystem();



  for (TiXmlElement *shapeElement = shapesElement->FirstChildElement("shape");
       shapeElement;
       shapeElement = shapeElement->NextSiblingElement("shape"))
  {

    vkMatrix4f localTransform;
    localTransform.SetIdentity();
    vkPhysGeometry geometryDesc;
    memset(&geometryDesc, 0, sizeof(geometryDesc));
    geometryDesc.Type = (vkPhysGeometryType)-1;

    for (TiXmlElement *child = shapeElement->FirstChildElement();
         child;
         child = child->NextSiblingElement())
    {
      vkString tag(child->Value());
      if (tag == vkString("transform"))
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
    IPhysicsShape *shape = phys->CreateShape(geometryDesc);
    shape->SetLocalTransform(localTransform);
    container->AddShape(shape);
  }

  return container;
}


void vkCollisionAssetXMLLoader::LoadTransform(TiXmlElement *transformElement, vkMatrix4f &localTransform) const
{
  TiXmlElement *matrixElement = transformElement->FirstChildElement("matrix");
  if (!matrixElement)
  {
    return;
  }


  vkVector4f row0, row1, row2, row3;
  TiXmlElement *row = matrixElement->FirstChildElement("row");
  if (row)
  {
    row0 = LoadVector4f(row->GetText());
    row = row->NextSiblingElement("row");
  }
  if (row)
  {
    row1 = LoadVector4f(row->GetText());
    row = row->NextSiblingElement("row");
  }
  if (row)
  {
    row2 = LoadVector4f(row->GetText());
    row = row->NextSiblingElement("row");
  }
  if (row)
  {
    row3 = LoadVector4f(row->GetText());
    row = row->NextSiblingElement("row");
  }

  localTransform.Set(
    row0.x, row0.y, row0.z, row0.w,
    row1.x, row1.y, row1.z, row1.w,
    row2.x, row2.y, row2.z, row2.w,
    row3.x, row3.y, row3.z, row3.w
  );
}

void vkCollisionAssetXMLLoader::LoadBox(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("halfX") &&
      transformElement->Attribute("halfY") &&
      transformElement->Attribute("halfZ"))
  {
    geometry.Type = ePGT_Box;
    geometry.Dimensions.x = atof(transformElement->Attribute("halfX"));
    geometry.Dimensions.y = atof(transformElement->Attribute("halfY"));
    geometry.Dimensions.z = atof(transformElement->Attribute("halfZ"));
  }
}


void vkCollisionAssetXMLLoader::LoadCylinder(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius") &&
      transformElement->Attribute("halfHeight"))
  {
    geometry.Type = ePGT_CapsuleZ;
    geometry.Radius = atof(transformElement->Attribute("radius"));
    geometry.Height = atof(transformElement->Attribute("halfHeight")) * 2.0f;
  }
}

void vkCollisionAssetXMLLoader::LoadSphere(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius"))
  {
    geometry.Type = ePGT_Sphere;
    geometry.Radius = atof(transformElement->Attribute("radius"));
  }
}


