
#include <components/assetresourcewidget.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <eventbus.hh>
#include <events/assetrenamedevent.hh>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>


void asset_resource_widget_asset_renamed(csEvent &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);
  AssetResourceWidget *widget = reinterpret_cast<AssetResourceWidget*>(userData);

}

AssetResourceWidget::AssetResourceWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
  connect(
    m_gui.lineEdit, SIGNAL(ResourceChanged(const csResourceLocator&)), 
    this, SLOT(ResourceChangedSlot(const csResourceLocator &)));


}

AssetResourceWidget::~AssetResourceWidget()
{

}

void AssetResourceWidget::AddValidClass(const csClass *cls)
{
  m_gui.lineEdit->AddValidClass(cls);
}

void AssetResourceWidget::ResourceChangedSlot(const csResourceLocator &locator)
{
  SetResourceLocator(locator);
  emit ResourceChanged(locator);
}

void AssetResourceWidget::SetResourceLocator(const csResourceLocator &locator)
{
  m_locator = locator;
  m_gui.lineEdit->setText(QString(locator.GetResourceFile().c_str())); 
}

const csResourceLocator &AssetResourceWidget::GetResourceLocator() const
{
  return m_locator;
}