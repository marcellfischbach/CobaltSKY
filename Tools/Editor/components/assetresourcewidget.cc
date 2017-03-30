
#include <components/assetresourcewidget.hh>
#include <valkyrie/core/vkresourcelocator.hh>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>


AssetResourceWidget::AssetResourceWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
  connect(
    m_gui.lineEdit, SIGNAL(ResourceChanged(const vkResourceLocator&)), 
    this, SLOT(ResourceChangedSlot(const vkResourceLocator &)));
}

AssetResourceWidget::~AssetResourceWidget()
{

}

void AssetResourceWidget::AddValidClass(const vkClass *cls)
{
  m_gui.lineEdit->AddValidClass(cls);
}

void AssetResourceWidget::ResourceChangedSlot(const vkResourceLocator &locator)
{
  SetResourceLocator(locator);
  emit ResourceChanged(locator);
}

void AssetResourceWidget::SetResourceLocator(const vkResourceLocator &locator)
{
  m_locator = locator;
  m_gui.lineEdit->setText(QString(locator.GetResourceFile().c_str())); 
}