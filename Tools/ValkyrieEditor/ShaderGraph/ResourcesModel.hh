#pragma once

#include <Valkyrie/Graphics/IShader.hh>
#include <ShaderGraph/SGNode.hh>
#include <qstring.h>
#include <qabstractitemmodel.h>
#include <qlist.h>
#include <qmap.h>

namespace shadergraph
{


class ResourcesModel : public QAbstractItemModel
{
  Q_OBJECT;
public:
  ResourcesModel();
  virtual ~ResourcesModel();

  virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &child) const;
  virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
  virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  void AddNode(SGNode *node);
  void RemoveNode(SGNode *node);

public slots:
  void NodeChanged(SGNode *node);

private:
  QList<SGNode *> m_nodes;
  QString GetValue(SGNode *node) const;

};


}