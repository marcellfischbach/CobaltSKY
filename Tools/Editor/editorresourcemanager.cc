
#include <editorresourcemanager.hh>
#include <valkyrie/core/csvfs.hh>


EditorResourceManager::EditorResourceManager()
  : csResourceManager()
{

}

EditorResourceManager::~EditorResourceManager()
{

}

iObject *EditorResourceManager::Load(const csResourceLocator &locator, iObject *userData)
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  printf("Load: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::Load(fixedLocator, userData);
}


const csClass *EditorResourceManager::EvalClass(const csResourceLocator &locator, iObject *userData) const
{
  csResourceLocator fixedLocator = FixResourceLocator(locator);
  printf("EvalClass: %s => %s\n", locator.GetDebugName().c_str(), fixedLocator.GetDebugName().c_str());
  return csResourceManager::EvalClass(fixedLocator, userData);
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
