
#include <editorresourcemanager.hh>
#include <valkyrie/core/vkvfs.hh>


EditorResourceManager::EditorResourceManager()
  : vkResourceManager()
{

}

EditorResourceManager::~EditorResourceManager()
{

}

iObject *EditorResourceManager::Load(const vkResourceLocator &locator, iObject *userData)
{
  vkResourceLocator fixedLocator = FixResourceLocator(locator);
  printf("Load: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return vkResourceManager::Load(fixedLocator, userData);
}


const vkClass *EditorResourceManager::EvalClass(const vkResourceLocator &locator, iObject *userData) const
{
  vkResourceLocator fixedLocator = FixResourceLocator(locator);
  printf("EvalClass: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return vkResourceManager::EvalClass(fixedLocator, userData);
}


vkResourceLocator EditorResourceManager::FixResourceLocator(const vkResourceLocator &locator) const
{
  if (!locator.GetResourceEntry().empty() && IsAnonymousLocator(locator))
  {
    return vkResourceLocator(
      locator.GetResourceFile(),
      locator.GetResourceName()
    );
  }
  return locator;
}

bool EditorResourceManager::IsAnonymousLocator(const vkResourceLocator &locator) const
{
  const vkVFS::Entry *entry = vkVFS::Get()->FindEntryForFilename(locator.GetResourceFile());
  if (!entry)
  {
    return false;
  }
  return entry->GetName() == locator.GetResourceEntry();
}

bool EditorResourceManager::RegisterObject(const vkResourceLocator &locator, iObject *object)
{
  bool res = vkResourceManager::RegisterObject(locator, object);
  if (locator.GetResourceEntry().empty())
  {

    const vkVFS::Entry *entry = vkVFS::Get()->FindEntryForFilename(locator.GetResourceFile());
    if (entry)
    {
      vkResourceManager::RegisterObject(
        vkResourceLocator(locator.GetResourceFile(), locator.GetResourceName(), entry->GetName()),
        object
      );
    }
  }
  else if (IsAnonymousLocator(locator))
  {
    vkResourceManager::RegisterObject(
      vkResourceLocator(locator.GetResourceFile(), locator.GetResourceName()),
      object
    );
  }
  return res;
}
