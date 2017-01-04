

#include <Valkyrie/Loaders/CollisionLoader.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>
#include <Valkyrie/vkengine.hh>
#include <Valkyrie/vkenums.hh>

vkCollisionAssetXMLLoader::vkCollisionAssetXMLLoader()
  : vkBaseXMLLoader()
{
}

vkCollisionAssetXMLLoader::~vkCollisionAssetXMLLoader()
{

}


bool vkCollisionAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("collider") ||
    vkString(element->Value()) == vkString("shapes");
}

const vkClass *vkCollisionAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkPhysicsShapeContainer::GetStaticClass();
}

IObject *vkCollisionAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkPhysicsShapeContainer *container = new vkPhysicsShapeContainer();

  TiXmlElement *shapesElement = 0;
  if (vkString(element->Value()) == vkString("shapes"))
  {
    shapesElement = element;
  }
  else
  {
    element->FirstChildElement("shapes");
  }
  if (!shapesElement)
  {
    return container;
  }



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
    IPhysicsShape *shape = vkEng->CreateShape(geometryDesc);
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


  vkVector4f r0, r1, r2, r3;
  TiXmlElement *row0 = matrixElement->FirstChildElement("row0");
  TiXmlElement *row1 = matrixElement->FirstChildElement("row1");
  TiXmlElement *row2 = matrixElement->FirstChildElement("row2");
  TiXmlElement *row3 = matrixElement->FirstChildElement("row3");
  if (!row0 || !row1 || !row2 || !row3)
  {
    return;
  }
  r0 = LoadVector4f(row0->GetText());
  r1 = LoadVector4f(row1->GetText());
  r2 = LoadVector4f(row2->GetText());
  r3 = LoadVector4f(row3->GetText());
  localTransform.Set(
    r0.x, r0.y, r0.z, r0.w,
    r1.x, r1.y, r1.z, r1.w,
    r2.x, r2.y, r2.z, r2.w,
    r3.x, r3.y, r3.z, r3.w
  );
}

void vkCollisionAssetXMLLoader::LoadBox(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("halfX") &&
      transformElement->Attribute("halfY") &&
      transformElement->Attribute("halfZ"))
  {
    geometry.Type = ePGT_Box;
    geometry.Dimensions.x = (float)atof(transformElement->Attribute("halfX")) * 2.0f;
    geometry.Dimensions.y = (float)atof(transformElement->Attribute("halfY")) * 2.0f;
    geometry.Dimensions.z = (float)atof(transformElement->Attribute("halfZ")) * 2.0f;
  }
}


void vkCollisionAssetXMLLoader::LoadCylinder(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius") &&
      transformElement->Attribute("halfHeight"))
  {
    geometry.Type = ePGT_CylinderZ;
    geometry.Radius = (float)atof(transformElement->Attribute("radius"));
    geometry.Height = (float)atof(transformElement->Attribute("halfHeight")) * 2.0f;
  }
}

void vkCollisionAssetXMLLoader::LoadSphere(TiXmlElement *transformElement, vkPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius"))
  {
    geometry.Type = ePGT_Sphere;
    geometry.Radius = (float)atof(transformElement->Attribute("radius"));
  }
}


