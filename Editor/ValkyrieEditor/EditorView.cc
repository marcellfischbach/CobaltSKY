

#include <EditorView.hh>
#include <Valkyrie/Core/ClassRegistry.hh>


EditorView::EditorView()
  : m_editorObject(0)
  , m_widget(0)
  , m_canClose(false)
{

}

EditorView::~EditorView()
{
  VK_RELEASE(m_editorObject);
  m_widget = 0;
}


void EditorView::SetEditorObject(IObject *editorObject)
{
  VK_SET(m_editorObject, editorObject);
}

const IObject *EditorView::GetEditorObject() const
{
  return m_editorObject;
}


void EditorView::SetResourceLocator(const vkResourceLocator &locator)
{
  m_resourceLocator = locator;
}

const vkResourceLocator &EditorView::GetResourceLocator() const
{
  return m_resourceLocator;
}

void EditorView::SetWidget(QWidget *widget)
{
  m_widget = widget;
}

QWidget *EditorView::GetWidget()
{
  return m_widget;
}

void EditorView::SetName(const QString &name)
{
  m_name = name;
}

const QString &EditorView::GetName() const
{
  return m_name;
}

void EditorView::SetCanClose(bool canClose)
{
  m_canClose = canClose;
}

bool EditorView::CanClose() const
{
  return m_canClose;
}

bool EditorView::Close()
{
  return false;
}



EditorViewFactoryManager::EditorViewFactoryManager()
{

}

EditorViewFactoryManager *EditorViewFactoryManager::Get()
{
  static EditorViewFactoryManager manager;
  return &manager;
}


IEditorViewFactory *EditorViewFactoryManager::GetFactory(const vkResourceLocator &resourceLocator, IObject *obj)
{
  for (IEditorViewFactory *factory : m_factories)
  {
    if (factory->CanEdit(resourceLocator, obj))
    {
      return factory;
    }
  }

  std::vector<const vkClass*> classes = vkClassRegistry::Get()->GetAllClasses();
  for (const vkClass *clazz : classes)
  {
    // find a class that inherits from IEditorViewFactory
    if (clazz->IsInstanceOf<IEditorViewFactory>())
    {
      bool has = false;
      for (IEditorViewFactory *factory : m_factories)
      {
        if (factory->GetClass() == clazz)
        {
          has = true;
          break;
        }
      }
      if (!has)
      {
        IEditorViewFactory *factory = clazz->CreateInstance<IEditorViewFactory>();
        if (factory)
        {
          m_factories.append(factory);
          if (factory->CanEdit(resourceLocator, obj))
          {
            return factory;
          }
        }
      }
    }
  }

  return 0;
}


