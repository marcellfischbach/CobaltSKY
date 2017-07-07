

#include <assetmanager/assetmanagercontentitemdelegate.hh>
#include <assetmanager/assetmanagercontentmodel.hh>
#include <assetmanager/assetmanagercontentmodelentry.hh>
#include <QPainter>
#include <QApplication>
#include <QLayout>


AssetManagerContentItemDelegate::AssetManagerContentItemDelegate(AssetManagerContentModel *model, QObject *parent)
  : QAbstractItemDelegate(parent)
  , m_model(model)
{
}

AssetManagerContentItemDelegate::~AssetManagerContentItemDelegate()
{

}


void AssetManagerContentItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  painter->save();

  if (option.state & QStyle::State_Selected)
  {
    QPainterPath path;
    path.addRoundedRect(option.rect, 16.0, 16.0);

    QLinearGradient grad(option.rect.topLeft(), option.rect.bottomLeft());
    grad.setColorAt(0.0, option.palette.background().color());
    grad.setColorAt(1.0, option.palette.highlight().color());
    painter->fillPath(path, QBrush(grad));
  }
  
  const AssetManagerContentModelEntry *entry = m_model->GetEntry(index);
  painter->drawPixmap(option.rect.topLeft() + QPoint(4, 4), entry->GetIcon());
  painter->drawText(QRect (
    option.rect.topLeft() + QPoint(64 + 8, 4), 
    QPoint(option.rect.right () - 4, option.rect.center().y() - 4)), 
    Qt::AlignLeft | Qt::AlignBottom,
    entry->GetEntryName());

  painter->restore();
}

QSize AssetManagerContentItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return QSize(256, 72);
}

