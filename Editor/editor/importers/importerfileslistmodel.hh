
#pragma once

#include <editor/editorexport.hh>
#include <QAbstractItemModel>
#include <vector>
#include <string>

class ImporterFilesListModel : public QAbstractItemModel
{
public:
  ImporterFilesListModel();

  void add(const std::string &data);

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;

  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
  std::vector<std::string> m_data;
};

