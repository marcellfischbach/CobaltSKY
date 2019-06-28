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
  setReadOnly(true);
  setFocusPolicy(Qt::StrongFocus);
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

void ReferenceLineEdit::SetResourceLocator(const cs::ResourceLocator &locator)
{
  setText(QString(locator.Encode().c_str()));
  m_locator = locator;
}

const cs::ResourceLocator &ReferenceLineEdit::GetResourceLocator() const
{
  return m_locator;
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

cs::editor::model::AssetNode* ReferenceLineEdit::GetAssetNode(QDropEvent *event) const
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
        if (node)
        {
          return node->AsAssetNode();
        }
      }
    }
  }
  return nullptr;
}

void ReferenceLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
  cs::editor::model::AssetNode* assetNode = GetAssetNode(event);
  if (assetNode)
  {
    const cs::Class* cls = assetNode->GetAssetClass();
    if (IsValidClass(cls))
    {
      event->accept();
      return;
    }
  }
  event->ignore ();
}

void ReferenceLineEdit::dropEvent(QDropEvent *event)
{
  cs::editor::model::AssetNode* assetNode = GetAssetNode(event);
  if (assetNode)
  {
    const cs::Class* cls = assetNode->GetAssetClass();
    if (IsValidClass(cls))
    {
      SetResourceLocator(assetNode->GetResourceLocator());
      event->accept();
      return;
    }
  }
  event->ignore ();
}

void ReferenceLineEdit::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && event->modifiers() == Qt::CTRL && m_locator.IsValid())
  {
    cs::editor::core::Editor::Get()->OpenEditor(m_locator);
  }
}

void ReferenceLineEdit::mouseMoveEvent(QMouseEvent *event)
{
  if (event->modifiers() == Qt::CTRL && m_locator.IsValid())
  {
    setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
  }
  else
  {
    setCursor(QCursor(Qt::CursorShape::ArrowCursor));
  }
}


} 
