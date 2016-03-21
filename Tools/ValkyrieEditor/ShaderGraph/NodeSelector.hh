#pragma once

#include <qdialog.h>
#include <qlistview.h>

#include <ui_NodeSelector.h>
#include <Valkyrie/Core/Object.hh>

class NodeSelector : public QDialog
{
  Q_OBJECT
public:
  NodeSelector(QWidget *parent);

private:
  QListView *m_listView;

private slots:
  void timed ();

signals:
  void addNode (const vkClass *clazz);
};

