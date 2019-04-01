
#include <editor/editorresourcemanager.hh>
#include <cobalt/core/csvfs.hh>
#include <editor/eventbus.hh>
#include <editor/events/assetrenamedevent.hh>

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

csResourceWrapper *EditorResourceManager::Load(const csResourceLocator &locator)
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("Load: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::Load(fixedLocator);
}


const csClass *EditorResourceManager::EvalClass(const csResourceLocator &locator) const
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  // printf("EvalClass: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::EvalClass(fixedLocator);
}

void EditorResourceManager::RenameResource(const csResourceLocator &from, const csResourceLocator &to)
{
  // the resource is already changed on drive ... so we must check the to value
  if (!to.GetResourceEntry().empty() && IsAnonymousLocator(to))
  {
    csResourceLocator fixedFrom = csResourceLocator(
		csResourceFile(from.GetResourceFile()),
      csResourceName(from.GetResourceName())
    );
    csResourceLocator fixedTo = csResourceLocator(
      csResourceFile(to.GetResourceFile()),
      csResourceName(to.GetResourceName())
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
      csResourceFile(locator.GetResourceFile()),
      csResourceName(locator.GetResourceName())
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

bool EditorResourceManager::RegisterObject(const csResourceLocator &locator, csResourceWrapper *object)
{
  bool res = csResourceManager::RegisterObject(locator, object);
  if (locator.GetResourceEntry().empty())
  {

    const csVFS::Entry *entry = csVFS::Get()->FindEntryForFilename(locator.GetResourceFile());
    if (entry)
    {
      csResourceManager::RegisterObject(
        csResourceLocator(
			csResourceEntry(entry->GetName()),
			csResourceFile(locator.GetResourceFile()),
			csResourceName(locator.GetResourceName())
		),
        object
      );
    }
  }
  else if (IsAnonymousLocator(locator))
  {
    csResourceManager::RegisterObject(
      csResourceLocator(csResourceFile(locator.GetResourceFile()), csResourceName(locator.GetResourceName())),
      object
    );
  }
  return res;
}
