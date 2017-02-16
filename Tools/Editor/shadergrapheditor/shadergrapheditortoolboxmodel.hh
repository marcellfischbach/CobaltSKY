#pragma once

#include <QAbstractItemModel>
#include <vector>
#include <valkyrie/core/vkstring.hh>

#define SHADER_GRAPH_EDITO_TOOLBOX_MODEL_CLASS_MIME "application/vkClassName"

class vkClass;



class ShaderGraphEditorToolboxModel : public QAbstractItemModel
{
public:
  ShaderGraphEditorToolboxModel();
  virtual ~ShaderGraphEditorToolboxModel();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  virtual QStringList mimeTypes() const;
  virtual QMimeData *ShaderGraphEditorToolboxModel::mimeData(const QModelIndexList &indexes) const;
  void CreateModelData(const QString &filter);
private:
  struct Data
  {
    vkString name;
    const vkClass *cls;
    QList<Data*> children;
    Data* parent;
    Data(const vkString &name)
      : name(name)
      , cls(0)
      , parent(0)
    {

    }
    ~Data()
    {
      for (Data *child : children)
      {
        delete child;
      }
      children.clear();
    }
  };

  Data *m_root;

  Data *fromString(const vkString &name);
};