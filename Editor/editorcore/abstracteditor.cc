

#include <editorcore/abstracteditor.hh>
#include <editormodel/nodes/assetnode.hh>
#include <editormodel/model.hh>
#include <QWidget>


namespace cs::editor::core
{


AbstractEditor::AbstractEditor(QWidget *parent)
  : m_pendingChanges(false)
  , m_name ("untitled")
  , m_widget(nullptr)
  , m_asset(nullptr)
{

}

AbstractEditor::~AbstractEditor()
{
  if (m_widget)
  {
    m_widget->deleteLater();
  }
}

void AbstractEditor::SetPendingChanges(bool pendingChanges)
{
  if (m_pendingChanges != pendingChanges)
  {
    m_pendingChanges = pendingChanges;
    m_titleChangedSignal.Call(this);
  }
}

bool AbstractEditor::HasPendingChanges() const
{
  return m_pendingChanges;
}

void AbstractEditor::SetName(const std::string& name)
{
  if (m_name != name)
  {
    m_name = name;
    m_titleChangedSignal.Call(this);
  }
}

const std::string& AbstractEditor::GetName() const
{
  return m_name;
}

void AbstractEditor::SetWidget(QWidget* widget)
{
  m_widget = widget;
}

QWidget* AbstractEditor::GetWidget()
{
  return m_widget;
}

void AbstractEditor::SetAsset( cs::editor::model::AssetNode* asset)
{
  if (m_asset == asset)
  {
    return;
  }
  if (m_asset)
  {
    m_asset->GetModel()->OnNamedNodeRenamed().Disconnect(m_slotModelNodeRenamed);
  }
  m_asset = asset;
  if (m_asset)
  {
    SetName(m_asset->GetName());
    m_slotModelNodeRenamed = m_asset->GetModel()->OnNamedNodeRenamed().Connect([this](model::Node* node, cs::ResourceLocator, cs::ResourceLocator) { NodeRenamed(node); });
  }
}

void AbstractEditor::NodeRenamed(model::Node* node)
{
  if (m_asset == node)
  {
    SetName(m_asset->GetName());
  }
}

cs::editor::model::AssetNode* AbstractEditor::GetAsset() 
{
  return m_asset;
}

const cs::editor::model::AssetNode* AbstractEditor::GetAsset() const
{
  return m_asset;
}

cs::Signal<AbstractEditor*>& AbstractEditor::OnTitleChanged()
{
  return m_titleChangedSignal;
}

}