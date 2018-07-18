
#include <editors/shadergrapheditor/shadergrapheditor.hh>
#include <editors/shadergrapheditor/shadergrapheditormeta.hh>
#include <editors/shadergrapheditor/shadergrapheditoroutliner.hh>
#include <editors/shadergrapheditor/shadergrapheditorpreview.hh>
#include <editors/shadergrapheditor/shadergrapheditorproperties.hh>
#include <editors/shadergrapheditor/shadergrapheditortoolbox.hh>
#include <editors/shadergrapheditor/shadergrapheditorwidget.hh>
#include <components/baseeditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>
#include <glcontext.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>


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
    
  QObject::connect(m_widget, SIGNAL(SelectionChanged(const QList<ShaderGraphEditorNode*>&)),
    m_properties, SLOT(SetNodes(const QList<ShaderGraphEditorNode*>&)));

  QObject::connect(m_widget, SIGNAL(SelectionChanged(const QList<ShaderGraphEditorNode*>&)),
    m_outliner, SLOT(SetSelectedNodes(const QList<ShaderGraphEditorNode*>&)));

  QObject::connect(m_properties, SIGNAL(NodeChanged()), m_widget, SLOT(RepaintGraph()));
  QObject::connect(m_widget, SIGNAL(ShaderGraphChanged()), m_preview, SLOT(ShaderGraphChanged()));
}

ShaderGraphEditor::~ShaderGraphEditor()
{
}


void ShaderGraphEditor::UpdateAsset()
{
  // GLContext::Get()->MakeCurrent();

	const csResourceLocator &locator = GetAsset()->GetResourceLocator();

  csSGShaderGraph *shaderGraph = csResourceManager::Get()->Aquire<csSGShaderGraph>(locator);

  csResourceLocator metaLocator(locator, "meta");
  ShaderGraphEditorMeta *meta = csResourceManager::Get()->Load<ShaderGraphEditorMeta>(metaLocator);
  if (!meta)
  { 
    meta = new ShaderGraphEditorMeta();
  }
  csSGShaderGraph *graph = m_widget->SetShaderGraph(shaderGraph, meta);
  m_outliner->SetShaderGraph(graph);
  m_preview->SetShaderGraph(shaderGraph);

  CS_RELEASE(meta);

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

