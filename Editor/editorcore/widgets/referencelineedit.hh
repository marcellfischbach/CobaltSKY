#pragma once

#include <editorcore/export.hh>
#include <QLineEdit>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcelocator.hh>
#include <vector>


namespace cs::editor::model
{
class AssetNode;
}

namespace cs::editor::core
{

class EDITOR_CORE_API ReferenceLineEdit : public QLineEdit
{
public:
  ReferenceLineEdit(QWidget * parent = nullptr);

  void SetResourceLocator (const cs::ResourceLocator &locator);
  const cs::ResourceLocator &GetResourceLocator() const;

  void SetValidClass(const cs::Class* cls);
  void AddValidClass(const cs::Class *cls);

  bool IsValidClass(const cs::Class* cls) const;


protected:
  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;

  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;

private:
  cs::ResourceLocator m_locator;
  std::vector<const cs::Class*> m_validClasses;

  cs::editor::model::AssetNode* GetAssetNode (QDropEvent* event) const;

  bool m_hover;
  bool m_ctrlDown;

};

}

