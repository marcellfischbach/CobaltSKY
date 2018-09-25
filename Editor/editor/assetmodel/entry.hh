
#pragma once

#include <editor/editorexport.hh>
#include <vector>
#include <cobalt/core/csresourcelocator.hh>

namespace asset::model
{
	class Asset;
	class Model;
	class ModelTransaction;
	class Folder;
	class Root;
  class VFSEntry;
  class EDITOR_API Entry
  {
		friend class Model;
  public:
    enum Type
    {
      eT_Root,
      eT_VFSEntry,
      eT_Folder,
      eT_Asset
    };

  public:
    virtual ~Entry();
    Entry *GetParent() { return m_parent;  }
    const Entry *GetParent() const { return m_parent; }

		bool IsDescendentOf(const Entry *entry) const;

		virtual void Add(Entry *entry, ModelTransaction &tr);
		virtual void Remove(Entry *entry, ModelTransaction &tr);
		virtual void MoveTo(Entry *newParent, ModelTransaction &tr);
		void RemoveFromParent(ModelTransaction &tr);
		virtual void Delete(ModelTransaction &tr);
		virtual void Rename(const std::string &newName, ModelTransaction &tr);

    Type GetType() const { return m_type; }

		virtual void SetName(const std::string &name);
		const std::string &GetName() const;
		
		virtual void SetDisplayName(const std::string &displayName);
		virtual const std::string &GetDisplayName() const;

		virtual const std::string FakeName(const std::string &name) const;


		virtual bool IsFolder() const;
		virtual Folder *AsFolder();
		virtual const Folder *AsFolder() const;

		virtual bool IsAsset() const;
		virtual Asset *AsAsset();
		virtual const Asset*AsAsset() const;

    virtual bool IsVFSEntry() const;
    virtual VFSEntry *AsVFSEntry();
    virtual const VFSEntry *AsVFSEntry() const;

		virtual bool IsRoot() const;
		virtual Root *AsRoot();
		virtual const Root *AsRoot() const;

		virtual bool IsAttached() const;
    virtual csResourceLocator GetResourceLocator() const = 0;
		virtual csResourceLocator CreateResourceLocator(const std::string &fileName) const;

		bool ContainsChild(const std::string &name) const;
    const std::vector<Entry*> &GetChildren() const;
    Entry* GetChildByName(const std::string &name);
    const Entry* GetChildByName(const std::string &name) const;

		Folder *FindFolder();
		const Folder *FindFolder() const;
    virtual const VFSEntry *GetVFSEntry() const;
    int GetVFSEntryPriority() const;

		Model *GetModel();
		const Model *GetModel() const;

  protected:
    Entry(Model *model, Type type);

  private:
		bool AddChild(Entry *entry);
		bool RemoveChild(Entry *entry);

    Model * m_model;
    Type m_type;
    std::string m_name;
		csResourceLocator m_locator;

    Entry *m_parent;
    std::vector<Entry*> m_children;
  };

}

inline void asset::model::Entry::SetName(const std::string &name)
{
  m_name = name;
}

inline const std::string &asset::model::Entry::GetName() const
{
  return m_name;
}

inline const std::vector<asset::model::Entry*> &asset::model::Entry::GetChildren() const
{
  return m_children;
}

inline asset::model::Model *asset::model::Entry::GetModel()
{
	return m_model;
}

inline const asset::model::Model *asset::model::Entry::GetModel() const
{
	return m_model;
}
