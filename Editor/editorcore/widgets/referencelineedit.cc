#include <editorcore/widgets/referencelineedit.hh>
#include <editorcore/editor.hh>
#include <editormodel/model.hh>
#include <QDragEnterEvent>
#include <QMimeData>
#include <iostream>

namespace cs::editor::core {

ReferenceLineEdit::ReferenceLineEdit(QWidget *parent)
  : QLineEdit(parent)
{
  m_validClasses.clear();
}

void ReferenceLineEdit::SetValidClass(const cs::Class *cls)
{
  m_validClasses.clear();
  if (cls)
  {
    m_validClasses.push_back(cls);
  }
}

void ReferenceLineEdit::AddValidClass(const cs::Class *cls)
{
  if (cls)
  {
    m_validClasses.push_back(cls);
  }
}


bool ReferenceLineEdit::IsValidClass(const cs::Class *cls) const
{
  if (cls)
  {
    for (auto validClass : m_validClasses)
    {
      if (cls->IsInstanceOf(validClass))
      {
        return true;
      }
    }
  }
  return false;
}

void ReferenceLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
  const QMimeData *data = event->mimeData();
  if (data->hasFormat("text/cs::ResourceLocator"))
  {
    QByteArray encodedData = data->data("text/cs::ResourceLocator");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);
    QString encodedLocator;
    stream >> encodedLocator;
    if (!encodedLocator.isEmpty())
    {
      cs::ResourceLocator locator (encodedLocator.toLatin1().data());

      cs::editor::model::Model *model = cs::editor::core::Editor::Get()->GetModel();
      if (model)
      {
        cs::editor::model::Node* node = model->FindNode(locator);
        if (node && node->IsAssetNode())
        {
           const cs::Class* cls = node->AsAssetNode()->GetAssetClass();
           if (IsValidClass(cls))
           {
             event->accept();
             return;
           }
        }
      }
    }
  }
  event->ignore ();
  std::cout << "DragEnter: " << event->mimeData()->hasFormat("text/cs::ResourceLocator") << "\n";
  std::fflush(stdout);
}

} 
