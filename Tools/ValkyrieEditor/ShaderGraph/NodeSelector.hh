#pragma once

#include <qdialog.h>
#include <qlistview.h>

// #include <ui_NodeSelector.h>
#include <Valkyrie/Core/Object.hh>

class NodesModel;
class NodeSelector : public QDialog
{
  Q_OBJECT
public:
  NodeSelector(QWidget *parent);

private:
  QListView *m_listView;
  NodesModel *m_nodesModel;

private slots:
  void on_listView_doubleClicked(const QModelIndex &index);
signals:
  void addNode (const vkClass *clazz);
};

