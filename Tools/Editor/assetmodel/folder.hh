
#pragma once

#include <assetmodel/entry.hh>

namespace asset::model
{
  class Model;
  class Folder : public Entry
  {
  public:
    Folder(Model *model, const std::string &folderName);
    virtual ~Folder();

    virtual void Add(Entry *entry);

    const std::string &GetFolderName() const;

    virtual csResourceLocator GetResourceLocator() const;

  protected:
    Folder(Model *mode, Entry::Type type);

    void SetFolderName(const std::string &folderName);

  private:
    std::string m_folderName;
  };
}