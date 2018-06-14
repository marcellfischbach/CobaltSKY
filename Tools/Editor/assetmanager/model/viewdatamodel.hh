
#pragma once

#include <QObject>
#include <map>

namespace asset::model
{
  class Model;
  class Entry;
  class ViewEntry;
  class ViewRoot;
  class ViewDataModel : public QObject
  {
    Q_OBJECT
  public:
    ViewDataModel();
    ~ViewDataModel();

    void SetModel(Model *model);
    void Rebuild();

    ViewRoot *GetRoot();

	signals:
		void EntryAboutToAdd(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);
		void EntryAdded(asset::model::ViewEntry *parent, asset::model::ViewEntry *child);

  private:
    void Cleanup();
    ViewEntry *Build(Entry *entry);
    ViewEntry * Create(Entry* entry);

	private slots:
		void EntryAdded(asset::model::Entry *parent, asset::model::Entry *child);

  private:
		void Connect();
		void Disconnect();

    Model * m_model;

    ViewRoot *m_root;



    std::map<Entry*, ViewEntry*> m_data;


  };
}

inline asset::model::ViewRoot *asset::model::ViewDataModel::GetRoot()
{
  return m_root;
}