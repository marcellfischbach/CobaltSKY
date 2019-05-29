

#pragma once

#include <editorcore/export.hh>
#include <cobalt/core/cssignalslot.hh>
#include <string>

class QWidget;

namespace cs::editor::model
{
class AssetNode;
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
  bool m_pendingChanges;
  std::string m_name;
  QWidget* m_widget;
  cs::editor::model::AssetNode* m_asset;

  cs::Signal<AbstractEditor*> m_titleChangedSignel;
};

}