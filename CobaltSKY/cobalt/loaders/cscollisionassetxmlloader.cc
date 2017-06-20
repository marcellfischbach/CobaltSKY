

#include <cobalt/loaders/cscollisionassetxmlloader.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/csphysicsshapecontainer.hh>
#include <cobalt/csengine.hh>
#include <cobalt/csenums.hh>

csCollisionAssetXMLLoader::csCollisionAssetXMLLoader()
  : csBaseXMLLoader()
{
}

csCollisionAssetXMLLoader::~csCollisionAssetXMLLoader()
{

}


bool csCollisionAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csString(element->Value()) == csString("collider") ||
    csString(element->Value()) == csString("shapes");
}

const csClass *csCollisionAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csPhysicsShapeContainer::GetStaticClass();
}

iObject *csCollisionAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csPhysicsShapeContainer *container = new csPhysicsShapeContainer();

  TiXmlElement *shapesElement = 0;
  if (csString(element->Value()) == csString("shapes"))
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

    csMatrix4f localTransform;
    localTransform.SetIdentity();
    csPhysGeometry geometryDesc;
    memset(&geometryDesc, 0, sizeof(geometryDesc));
    geometryDesc.Type = (csPhysGeometryType)-1;

    for (TiXmlElement *child = shapeElement->FirstChildElement();
         child;
         child = child->NextSiblingElement())
    {
      csString tag(child->Value());
      if (tag == csString("transform"))
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

  return container;
}


void csCollisionAssetXMLLoader::LoadTransform(TiXmlElement *transformElement, csMatrix4f &localTransform) const
{
  TiXmlElement *matrixElement = transformElement->FirstChildElement("matrix");
  if (!matrixElement)
  {
    return;
  }


  csVector4f r0, r1, r2, r3;
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

void csCollisionAssetXMLLoader::LoadBox(TiXmlElement *transformElement, csPhysGeometry &geometry) const
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


void csCollisionAssetXMLLoader::LoadCylinder(TiXmlElement *transformElement, csPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius") &&
      transformElement->Attribute("halfHeight"))
  {
    geometry.Type = ePGT_CylinderZ;
    geometry.Radius = (float)atof(transformElement->Attribute("radius"));
    geometry.Height = (float)atof(transformElement->Attribute("halfHeight")) * 2.0f;
  }
}

void csCollisionAssetXMLLoader::LoadSphere(TiXmlElement *transformElement, csPhysGeometry &geometry) const
{
  if (transformElement->Attribute("radius"))
  {
    geometry.Type = ePGT_Sphere;
    geometry.Radius = (float)atof(transformElement->Attribute("radius"));
  }
}


