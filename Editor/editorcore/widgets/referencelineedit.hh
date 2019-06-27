#pragma once

#include <editorcore/export.hh>
#include <QLineEdit>
#include <csrefl/class.hh>
#include <vector>

namespace cs::editor::core {

class EDITOR_CORE_API ReferenceLineEdit : public QLineEdit
{
public:
  ReferenceLineEdit(QWidget * parent = nullptr);

  void SetValidClass(const cs::Class* cls);
  void AddValidClass(const cs::Class *cls);

  bool IsValidClass(const cs::Class* cls) const;


protected:
  virtual void dragEnterEvent(QDragEnterEvent *event) override;

private:
  std::vector<const cs::Class*> m_validClasses;
};

}

