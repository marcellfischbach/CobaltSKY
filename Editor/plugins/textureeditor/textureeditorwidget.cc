
#include <plugins/textureeditor/textureeditorwidget.hh>

#include <plugins/textureeditor/textureeditorpreview.hh>

#include <QGridLayout>

namespace cs::editor::plugin::textureeditor
{

TextureEditorWidget::TextureEditorWidget(TextureEditor *editor, iTexture* texture)
    : QWidget()
{
    QGridLayout *gridLayout = new QGridLayout(this);

    TextureEditorPreview *preview = new TextureEditorPreview(this);

    gridLayout->addWidget(preview, 0, 0, 1, 1);
}

} // namespace cs

