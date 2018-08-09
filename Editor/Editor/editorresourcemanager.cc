
#include <editorresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <eventbus.hh>
#include <events/assetrenamedevent.hh>

void editor_resource_manager_asset_renamed(csEvent &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);

  EditorResourceManager *resourceManager = reinterpret_cast<EditorResourceManager*>(userData);
  resourceManager->RenameResource(evt.GetFrom(), evt.GetTo());

}

EditorResourceManager::EditorResourceManager()
  : csResourceManager()
{
  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), editor_resource_manager_asset_renamed, this);
}

EditorResourceManager::~EditorResourceManager()
{
  EventBus::Get().Deregister(editor_resource_manager_asset_renamed, this);
}

iObject *EditorResourceManager::Load(const csResourceLocator &locator, iObject *userData)
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("Load: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::Load(fixedLocator, userData);
}


const csClass *EditorResourceManager::EvalClass(const csResourceLocator &locator, iObject *userData) const
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("EvalClass: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::EvalClass(fixedLocator, userData);
}

void EditorResourceManager::RenameResource(const csResourceLocator &from, const csResourceLocator &to)
{
  // the resource is already changed on drive ... so we must check the to value
  if (!to.GetResourceEntry().empty() && IsAnonymousLocator(to))
  {
    csResourceLocator fixedFrom = csResourceLocator(
      from.GetResourceFile(),
      from.GetResourceName()
    );
    csResourceLocator fixedTo = csResourceLocator(
      to.GetResourceFile(),
      to.GetResourceName()
    );
    csResourceManager::RenameResource(fixedFrom, fixedTo);
  }
  else
  {
    csResourceManager::RenameResource(from, to);
  }
}

csResourceLocator EditorResourceManager::FixResourceLocator(const csResourceLocator &locator) const
{
  if (!locator.GetResourceEntry().empty() && IsAnonymousLocator(locator))
  {
    return csResourceLocator(
      locator.GetResourceFile(),
      locator.GetResourceName()
    );
  }
  return locator;
}

bool EditorResourceManager::IsAnonymousLocator(const csResourceLocator &locator) const
{
  const csVFS::Entry *entry = csVFS::Get()->FindEntryForFilename(locator.GetResourceFile());
  if (!entry)
  {
    return false;
  }
  return entry->GetName() == locator.GetResourceEntry();
}

bool EditorResourceManager::RegisterObject(const csResourceLocator &locator, iObject *object)
{
  bool res = csResourceManager::RegisterObject(locator, object);
  if (locator.GetResourceEntry().empty())
  {

    const csVFS::Entry *entry = csVFS::Get()->FindEntryForFilename(locator.GetResourceFile());
    if (entry)
    {
      csResourceManager::RegisterObject(
        csResourceLocator(locator.GetResourceFile(), locator.GetResourceName(), entry->GetName()),
        object
      );
    }
  }
  else if (IsAnonymousLocator(locator))
  {
    csResourceManager::RegisterObject(
      csResourceLocator(locator.GetResourceFile(), locator.GetResourceName()),
      object
    );
  }
  return res;
}
