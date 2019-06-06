

#include <editorcore/abstracteditor.hh>
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
    m_titleChangedSignel.Call(this);
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
    m_titleChangedSignel.Call(this);
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
  m_asset = asset;
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
  return m_titleChangedSignel;
}

}