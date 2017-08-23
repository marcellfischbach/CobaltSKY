#include <cobalt/loaders/entity/csentityxmlloader.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/csentitystate.hh>


// ***********************************************************************************************
// ***********************************************************************************************
//
//                   csEntityLoader
//
// ***********************************************************************************************
// ***********************************************************************************************


csEntityXMLLoader::csEntityXMLLoader()
  : csBaseXMLLoader()
{

}


csEntityXMLLoader::~csEntityXMLLoader()
{

}


bool csEntityXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return userData != 0;
}

const csClass *csEntityXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csEntity::GetStaticClass();
}

iObject *csEntityXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csEntity *entity = csQueryClass<csEntity>(userData);
  if (!entity)
  {
    userData->Release();
    return 0;
  }

  if (element->Attribute("name"))
  {
    entity->SetName(std::string(element->Attribute("name")));
  }

  std::map<std::string, csSpatialState*> spatialStates;
  TiXmlElement *statesElement = element->FirstChildElement("entityStates");
  if (statesElement)
  {
    for (TiXmlElement *stateElement = statesElement->FirstChildElement("entityState");
         stateElement;
         stateElement = stateElement->NextSiblingElement("entityState"))
    {

      csEntityState *entityState = csResourceManager::Get()->Load<csEntityState>(stateElement, locator, entity);
      if (entityState)
      {
        bool addedToEntity = false;
        csSpatialState *spatialState = entityState->ToSpatialState();
        if (spatialState)
        {
          if (stateElement->Attribute("id"))
          {
            spatialStates[std::string(stateElement->Attribute("id"))] = spatialState;
          }

          if (stateElement->Attribute("parentID"))
          {
            csSpatialState *parentState = spatialStates[std::string(stateElement->Attribute("parentID"))];
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
    csSpatialState *rootState = spatialStates[std::string(rootElement->Attribute("id"))];
    if (rootState)
    {
      entity->SetRootState(rootState);
    }
  }

  return entity;
}

const csClass *csEntityXMLLoader::GetLoadingClass() const
{
  return csEntity::GetStaticClass();
}



