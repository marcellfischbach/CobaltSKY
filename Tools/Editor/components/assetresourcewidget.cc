
#include <components/assetresourcewidget.hh>

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>


AssetResourceWidget::AssetResourceWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
}

AssetResourceWidget::~AssetResourceWidget()
{

}

void AssetResourceWidget::SetResourceLocator(const vkResourceLocator &locator)
{
  m_locator = locator;
  m_gui.lineEdit->setText(QString(locator.GetResourceFile().c_str()));
}