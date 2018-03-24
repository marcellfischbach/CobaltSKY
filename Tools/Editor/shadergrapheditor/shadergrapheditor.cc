
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditoroutliner.hh>
#include <shadergrapheditor/shadergrapheditorpreview.hh>
#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditortoolbox.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <components/baseeditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>
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
  const AssetDescriptor &descriptor = GetAssetDescriptor();


  csSGShaderGraph *shaderGraph = csResourceManager::Get()->Aquire<csSGShaderGraph>(descriptor.GetLocator());

  csResourceLocator metaLocator(descriptor.GetLocator(), "meta");
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

