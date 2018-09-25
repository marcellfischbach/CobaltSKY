

#include <editor/assetmanager/assetmanagercontentitemdelegate.hh>
#include <editor/assetmanager/assetmanagercontentmodel.hh>
#include <editor/assetmanager/assetmanagercontentmodelentry.hh>
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
    grad.setColorAt(0.0, option.palette.highlight().color().lighter(120));
    grad.setColorAt(0.3, option.palette.highlight().color().lighter(120));
    grad.setColorAt(0.4, option.palette.highlight().color());
    grad.setColorAt(1.0, option.palette.highlight().color());
    painter->fillPath(path, QBrush(grad));
  }
  
  const AssetManagerContentModelEntry *entry = m_model->GetEntry(index);
  painter->drawPixmap(option.rect.topLeft() + QPoint(4, 4), entry->GetIcon());
  painter->drawText(QRect (
    option.rect.topLeft() + QPoint(64 + 8, 4), 
    QPoint(option.rect.right () - 4, option.rect.center().y() - 2)), 
    Qt::AlignLeft | Qt::AlignBottom,
    entry->GetEntryName());

  if (!entry->GetTypeName().isEmpty())
  {
    painter->drawText(QRect(
      QPoint(option.rect.left() +64 + 8, option.rect.center().y() + 2),
      option.rect.bottomRight() - QPoint(4, 4)),
      Qt::AlignLeft | Qt::AlignTop,
      entry->GetTypeName());
  }
  painter->restore();
}

QSize AssetManagerContentItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return QSize(256, 72);
}

