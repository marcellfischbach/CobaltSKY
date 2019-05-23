
#include <editor/editorresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <editor/eventbus.hh>
#include <editor/events/assetrenamedevent.hh>

void editor_resource_manager_asset_renamed(cs::Event &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);

  EditorResourceManager *resourceManager = reinterpret_cast<EditorResourceManager*>(userData);
  resourceManager->RenameResource(evt.GetFrom(), evt.GetTo());

}

EditorResourceManager::EditorResourceManager()
  : cs::ResourceManager()
{
  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), editor_resource_manager_asset_renamed, this);
}

EditorResourceManager::~EditorResourceManager()
{
  EventBus::Get().Deregister(editor_resource_manager_asset_renamed, this);
}

cs::ResourceWrapper *EditorResourceManager::Load(const cs::ResourceLocator &locator)
{
  cs::ResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("Load: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return cs::ResourceManager::Load(fixedLocator);
}


const cs::Class *EditorResourceManager::EvalClass(const cs::ResourceLocator &locator) const
{
  cs::ResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("EvalClass: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return cs::ResourceManager::EvalClass(fixedLocator);
}

void EditorResourceManager::RenameResource(const cs::ResourceLocator &from, const cs::ResourceLocator &to)
{
  // the resource is already changed on drive ... so we must check the to value
  if (!to.GetResourceEntry().empty() && IsAnonymousLocator(to))
  {
    cs::ResourceLocator fixedFrom = cs::ResourceLocator(
		cs::ResourceFile(from.GetResourceFile()),
      cs::ResourceName(from.GetResourceName())
    );
    cs::ResourceLocator fixedTo = cs::ResourceLocator(
      cs::ResourceFile(to.GetResourceFile()),
      cs::ResourceName(to.GetResourceName())
    );
    cs::ResourceManager::RenameResource(fixedFrom, fixedTo);
  }
  else
  {
    cs::ResourceManager::RenameResource(from, to);
  }
}

cs::ResourceLocator EditorResourceManager::FixResourceLocator(const cs::ResourceLocator &locator) const
{
  if (!locator.GetResourceEntry().empty() && IsAnonymousLocator(locator))
  {
    return cs::ResourceLocator(
      cs::ResourceFile(locator.GetResourceFile()),
      cs::ResourceName(locator.GetResourceName())
    );
  }
  return locator;
}

bool EditorResourceManager::IsAnonymousLocator(const cs::ResourceLocator &locator) const
{
  const cs::VFS::Entry *entry = cs::VFS::Get()->FindEntryForFilename(locator.GetResourceFile());
  if (!entry)
  {
    return false;
  }
  return entry->GetName() == locator.GetResourceEntry();
}

bool EditorResourceManager::RegisterObject(const cs::ResourceLocator &locator, cs::ResourceWrapper *object)
{
  bool res = cs::ResourceManager::RegisterObject(locator, object);
  if (locator.GetResourceEntry().empty())
  {

    const cs::VFS::Entry *entry = cs::VFS::Get()->FindEntryForFilename(locator.GetResourceFile());
    if (entry)
    {
      cs::ResourceManager::RegisterObject(
        cs::ResourceLocator(
			cs::ResourceEntry(entry->GetName()),
			cs::ResourceFile(locator.GetResourceFile()),
			cs::ResourceName(locator.GetResourceName())
		),
        object
      );
    }
  }
  else if (IsAnonymousLocator(locator))
  {
    cs::ResourceManager::RegisterObject(
      cs::ResourceLocator(cs::ResourceFile(locator.GetResourceFile()), cs::ResourceName(locator.GetResourceName())),
      object
    );
  }
  return res;
}
