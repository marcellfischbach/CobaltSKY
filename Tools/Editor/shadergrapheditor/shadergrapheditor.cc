
#include <shadergrapheditor/shadergrapheditor.hh>
#include <shadergrapheditor/shadergrapheditormeta.hh>
#include <shadergrapheditor/shadergrapheditoroutliner.hh>
#include <shadergrapheditor/shadergrapheditorpreview.hh>
#include <shadergrapheditor/shadergrapheditorproperties.hh>
#include <shadergrapheditor/shadergrapheditortoolbox.hh>
#include <shadergrapheditor/shadergrapheditorwidget.hh>
#include <basicdockitem.hh>
#include <editor.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>


ShaderGraphEditor::ShaderGraphEditor()
  : AbstractAssetEditor()
{
  CS_CLASS_GEN_CONSTR;
  m_widget = new ShaderGraphEditorWidget(this);
  SetWidget(m_widget);

  m_toolbox = new ShaderGraphEditorToolbox();
  AddDockItemName(TOOLBOX_DOCK_NAME);

  m_properties = new ShaderGraphEditorProperties();
  AddDockItemName(PROPERTIES_DOCK_NAME);

  m_outliner = new ShaderGraphEditorOutliner();
  AddDockItemName(OUTLINER_DOCK_NAME);

  m_preview = new ShaderGraphEditorPreview();
  AddDockItemName(PREVIEW_DOCK_NAME);

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

void ShaderGraphEditor::PopulateDockItems()
{
  BasicDockItem *toolbox = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(TOOLBOX_DOCK_NAME));
  toolbox->SetContent(m_toolbox);

  BasicDockItem *properties = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(PROPERTIES_DOCK_NAME));
  properties->SetContent(m_properties);

  BasicDockItem *preview = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(PREVIEW_DOCK_NAME));
  preview->SetContent(m_preview);

  BasicDockItem *outliner = static_cast<BasicDockItem*>(Editor::Get()->GetDockItem(OUTLINER_DOCK_NAME));
  outliner->SetContent(m_outliner);

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

