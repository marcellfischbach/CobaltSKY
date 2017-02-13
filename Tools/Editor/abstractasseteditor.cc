

#include <abstractasseteditor.hh>

AbstractAssetEditor::AbstractAssetEditor()
  : iAssetEditor()
{
}

AbstractAssetEditor::~AbstractAssetEditor()
{

}

void AbstractAssetEditor::SetAssetDescriptor(const AssetDescriptor &descriptor)
{
  m_assetDescriptor = descriptor;
  m_name = descriptor.GetAssetFileName();

  size_t idx = m_name.find_last_of('/');
  if (idx == -1)
  {
    idx = m_name.find_last_of("\\");
  }
  if (idx == -1)
  {
    m_name = "Unknown";
  }
  else
  {
    m_name = m_name.substr(idx + 1);
  }

  OpenAsset();
}

const AssetDescriptor &AbstractAssetEditor::GetAssetDescriptor() const
{
  return m_assetDescriptor;
}



void AbstractAssetEditor::SetWidget(QWidget *widget)
{
  m_widget = widget;
}

QWidget *AbstractAssetEditor::GetWidget()
{
  return m_widget;
}

const vkString &AbstractAssetEditor::GetName() const
{
  return m_name;
}

void AbstractAssetEditor::AddDockItemName(const vkString &dockItemName)
{
  m_visibleDockItems.insert(dockItemName);
}

const std::set<vkString> &AbstractAssetEditor::GetVisibleDockItems() const
{
  return m_visibleDockItems;
}

