
#include <components/assetresourcewidget.hh>
#include <valkyrie/core/csresourcelocator.hh>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>


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