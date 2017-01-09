#include <valkyrie/loaders/entity/vkentityxmlloader.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vkentitystate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   vkEntityLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


vkEntityXMLLoader::vkEntityXMLLoader()
  : vkBaseXMLLoader()
{

}


vkEntityXMLLoader::~vkEntityXMLLoader()
{

}


bool vkEntityXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return userData != 0;
}

const vkClass *vkEntityXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkEntity::GetStaticClass();
}

iObject *vkEntityXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkEntity *entity = vkQueryClass<vkEntity>(userData);
  if (!entity)
  {
    userData->Release();
    return 0;
  }

  if (element->Attribute("name"))
  {
    entity->SetName(vkString(element->Attribute("name")));
  }

  std::map<vkString, vkSpatialState*> spatialStates;
  TiXmlElement *statesElement = element->FirstChildElement("entityStates");
  if (statesElement)
  {
    for (TiXmlElement *stateElement = statesElement->FirstChildElement("entityState");
         stateElement;
         stateElement = stateElement->NextSiblingElement("entityState"))
    {

      vkEntityState *entityState = vkResourceManager::Get()->Load<vkEntityState>(stateElement, locator, entity);
      if (entityState)
      {
        bool addedToEntity = false;
        vkSpatialState *spatialState = entityState->ToSpatialState();
        if (spatialState)
        {
          if (stateElement->Attribute("id"))
          {
            spatialStates[vkString(stateElement->Attribute("id"))] = spatialState;
          }

          if (stateElement->Attribute("parentID"))
          {
            vkSpatialState *parentState = spatialStates[vkString(stateElement->Attribute("parentID"))];
            if (parentState)
            {
              entity->AddState(spatialState, parentState);
              addedToEntity = true;
            }
          }
        }

        if (!addedToEntity)
        {
          entity->AddState(entityState);
        }
      }
    }
  }

  TiXmlElement *rootElement = element->FirstChildElement("rootState");
  if (rootElement && rootElement->Attribute("id"))
  {
    vkSpatialState *rootState = spatialStates[vkString(rootElement->Attribute("id"))];
    if (rootState)
    {
      entity->SetRootState(rootState);
    }
  }

  return entity;
}

const vkClass *vkEntityXMLLoader::GetLoadingClass() const
{
  return vkEntity::GetStaticClass();
}



