
#include <assetmanager/assetmanager.hh>
#include <assetmanager/assetmanagertoolview.hh>
#include <editorcore/editor.hh>



namespace cs::editor::assetmanager
{

AssetManagerToolView::AssetManagerToolView()
  : cs::editor::core::iToolView()
  , m_assetManager(nullptr)
{
  m_assetManager = new AssetManager(
    cs::editor::core::Editor::Get()->GetModel(),
    AssetManager::eS_Automatic);

}

Qt::DockWidgetAreas AssetManagerToolView::GetAllowedAreas() const
{
  return Qt::AllDockWidgetAreas;
}

Qt::DockWidgetArea AssetManagerToolView::GetDefaultArea() const
{
  return Qt::LeftDockWidgetArea;
}

QString AssetManagerToolView::GetTitle() const
{
  return QObject::tr("Asset manager");
}

QWidget* AssetManagerToolView::GetWidget() const
{
  return m_assetManager;
}

}