
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditoroutliner.hh>
#include <shadergrapheditor/shadergrapheditorpreview.hh>
#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditortoolbox.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <editor/components/baseeditorwidget.hh>
#include <editor/basicdockitem.hh>
#include <editor/editor.hh>
#include <editor/glcontext.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <QUuid>

ShaderGraphEditor::ShaderGraphEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new ShaderGraphEditorWidget(this);

  m_toolbox = new ShaderGraphEditorToolbox();
  m_properties = new ShaderGraphEditorProperties();
  m_outliner = new ShaderGraphEditorOutliner();
  m_preview = new ShaderGraphEditorPreview();

  
  BaseEditorWidget *widget = new BaseEditorWidget(0,
    BaseEditorWidget::Description(m_preview, m_toolbox),
    m_widget,
    BaseEditorWidget::Description(m_outliner, m_properties));
    
  SetWidget(widget);
    
  QObject::connect(m_widget, SIGNAL(SelectionChanged(const QList<ShaderGraphEditorNode*>&)), m_properties, SLOT(SetNodes(const QList<ShaderGraphEditorNode*>&)));

  QObject::connect(m_widget, SIGNAL(SelectionChanged(const QList<ShaderGraphEditorNode*>&)), m_outliner, SLOT(SetSelectedNodes(const QList<ShaderGraphEditorNode*>&)));
  QObject::connect(m_widget, SIGNAL(ShaderGraphNodeAboutToAdd(csSGNode*)), m_outliner, SLOT(NodeAboutToAdd(csSGNode*)));
  QObject::connect(m_widget, SIGNAL(ShaderGraphNodeAdded(csSGNode*)), m_outliner, SLOT(NodeAdded(csSGNode*)));
  QObject::connect(m_widget, SIGNAL(ShaderGraphNodeAboutToRemove(csSGNode*)), m_outliner, SLOT(NodeAboutToRemove(csSGNode*)));
  QObject::connect(m_widget, SIGNAL(ShaderGraphNodeRemoved(csSGNode*)), m_outliner, SLOT(NodeRemoved(csSGNode*)));
  QObject::connect(m_properties, SIGNAL(NodeChanged(ShaderGraphEditorNode*)), m_outliner, SLOT(NodeChanged(ShaderGraphEditorNode*)));

  QObject::connect(m_properties, SIGNAL(NodeChanged(ShaderGraphEditorNode*)), m_widget, SLOT(RepaintGraph()));
  QObject::connect(m_widget, SIGNAL(ShaderGraphChanged()), m_preview, SLOT(ShaderGraphChanged()));
}

ShaderGraphEditor::~ShaderGraphEditor()
{
}


void ShaderGraphEditor::UpdateAsset()
{
  // GLContext::Get()->MakeCurrent();

	const csResourceLocator &locator = GetAsset()->GetResourceLocator();

  csMaterialDefWrapper *shaderGraphWrapper = csResourceManager::Get()->Aquire<csMaterialDefWrapper>(locator);
  csSGShaderGraph *shaderGraph = cs::QueryClass<csSGShaderGraph>(shaderGraphWrapper->Get());
  UpdateResourceNodeIDs(shaderGraph);

  csResourceLocator metaLocator = locator.WithResourceName("meta");
  ShaderGraphEditorMetaWrapper *metaWrapper = csResourceManager::Get()->Load<ShaderGraphEditorMetaWrapper>(metaLocator);
  ShaderGraphEditorMeta *meta = metaWrapper ? metaWrapper->Get() : nullptr;
  if (!meta)
  { 
    meta = new ShaderGraphEditorMeta();
  }

  csSGShaderGraph *graph = m_widget->SetShaderGraph(shaderGraph, meta);
  m_outliner->SetShaderGraph(graph);
  m_preview->SetShaderGraph(shaderGraphWrapper);

  CS_RELEASE(meta);

}

void ShaderGraphEditor::UpdateResourceNodeIDs(csSGShaderGraph *shaderGraph)
{
  for (size_t i = 0, in = shaderGraph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    csSGNode *node = shaderGraph->GetNode(i);
    csSGResourceNode *resourceNode = cs::QueryClass<csSGResourceNode>(node);
    if (resourceNode)
    {
      std::string resourceId = resourceNode->GetResourceId();
      if (resourceId.empty())
      {
        resourceId = QUuid::createUuid().toString().toLatin1().data();
        resourceNode->SetResourceId(resourceId);
      }
    }
  }
}

void ShaderGraphEditor::UpdatePreview()
{
  QImage preview = TakeScreenshot(64, 64);
  if (!preview.isNull())
  {
    ReplacePreviewIcon(preview);
  }
}

QImage ShaderGraphEditor::TakeScreenshot(unsigned width, unsigned height)
{
  return m_preview->TakeScreenshot(width, height);
}

