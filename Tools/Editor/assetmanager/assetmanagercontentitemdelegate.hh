#pragma once


#include <QAbstractItemDelegate>

class AssetManagerContentModel;
class AssetManagerContentItemDelegate : public QAbstractItemDelegate
{
public:
  AssetManagerContentItemDelegate(AssetManagerContentModel *model, QObject *parent = 0);
  virtual ~AssetManagerContentItemDelegate();

  virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
  AssetManagerContentModel *m_model;
};