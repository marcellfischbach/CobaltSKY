
#include <editor/components/assetresourcewidget.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <editor/project/project.hh>
#include <editor/editor.hh>
#include <editor/eventbus.hh>
#include <editor/events/assetrenamedevent.hh>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>


void asset_resource_widget_asset_renamed(cs::Event &event, void *userData)
{
  AssetRenamedEvent &evt = static_cast<AssetRenamedEvent&>(event);
  AssetResourceWidget *widget = reinterpret_cast<AssetResourceWidget*>(userData);
  widget->Renamed(evt.GetFrom(), evt.GetTo());
}

AssetResourceWidget::AssetResourceWidget(QWidget *parent)
  : QWidget(parent)
{
  m_gui.setupUi(this);
  connect(
    m_gui.lineEdit, SIGNAL(ResourceChanged(const cs::ResourceLocator&)), 
    this, SLOT(ResourceChangedSlot(const cs::ResourceLocator &)));

  EventBus::Get().Register(AssetRenamedEvent::GetStaticClass(), asset_resource_widget_asset_renamed, this);
}

AssetResourceWidget::~AssetResourceWidget()
{
  EventBus::Get().Deregister(asset_resource_widget_asset_renamed, this);
}

void AssetResourceWidget::AddValidClass(const cs::Class *cls)
{
  m_gui.lineEdit->AddValidClass(cls);
}

void AssetResourceWidget::ResourceChangedSlot(const cs::ResourceLocator &locator)
{
  SetResourceLocator(locator);
  emit ResourceChanged(locator);
}

void AssetResourceWidget::SetResourceLocator(const cs::ResourceLocator &locator)
{
  m_locator = locator;
  m_gui.lineEdit->setText(QString(locator.GetResourceFile().c_str())); 
}

const cs::ResourceLocator &AssetResourceWidget::GetResourceLocator() const
{
  return m_locator;
}

void AssetResourceWidget::Renamed(const cs::ResourceLocator &from, const cs::ResourceLocator &to)
{
  if (m_locator.EqualsAnonymous(from))
  {
    SetResourceLocator(to.AsAnonymous());
  }
}