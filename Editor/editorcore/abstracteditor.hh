

#pragma once

#include <editorcore/export.hh>
#include <cobalt/core/cssignalslot.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <string>

class QWidget;

namespace cs::editor::model
{
class AssetNode;
class Node;
}

namespace cs::editor::core
{


class AbstractEditor;
class EDITOR_CORE_API AbstractEditor 
{
public:
  virtual ~AbstractEditor();
  cs::Signal<AbstractEditor*>& OnTitleChanged();

  const std::string& GetName() const;
  bool HasPendingChanges() const;
  QWidget* GetWidget();
  void SetAsset(cs::editor::model::AssetNode* asset);
  cs::editor::model::AssetNode* GetAsset();
  const cs::editor::model::AssetNode* GetAsset() const;

protected:
  AbstractEditor(QWidget *parent = nullptr);

  void SetName(const std::string& name);
  void SetPendingChanges(bool pendingChanges);
  void SetWidget(QWidget* widget);


private:
  void NodeRenamed(model::Node* node);
  bool m_pendingChanges;
  std::string m_name;
  QWidget* m_widget;
  cs::editor::model::AssetNode* m_asset;

  cs::Slot<model::Node*, cs::ResourceLocator, cs::ResourceLocator> m_slotModelNodeRenamed;
  cs::Signal<AbstractEditor*> m_titleChangedSignal;
};

}