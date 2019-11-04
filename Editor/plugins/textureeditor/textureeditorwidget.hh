#pragma once

#include <QWidget>

namespace cs
{
struct iTexture;
}

namespace cs::editor::plugin::textureeditor
{

class TextureEditor;

class TextureEditorWidget : public QWidget
{
public:
  TextureEditorWidget(TextureEditor *editor, iTexture* texture);
};

}

