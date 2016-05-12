#pragma once

#include <qdialog.h>
#include <qlistview.h>

#include <ui_NodeSelector.h>
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
void selectIndex(const QModelIndex &index);
  void on_tvNodes_doubleClicked(const QModelIndex &index);
  void on_tvNodes_activated(const QModelIndex &index);
  void on_leFilter_textChanged(const QString &text);
signals:
  void addNode (const vkClass *clazz);

private:
  Ui::NodeSelector m_gui;
};

