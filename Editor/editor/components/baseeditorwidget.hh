#pragma once

#include <editor/editorexport.hh>
#include <QWidget>


class EDITOR_API BaseEditorWidget : public QWidget
{
public:
  class EDITOR_API Description
  {
  public:
    Description(QWidget *part0 = 0, QWidget *part1 = 0);
    unsigned Count() const;
    QWidget *GetPart0() const;
    QWidget *GetPart1() const;
  private:
    QWidget *m_part0;
    QWidget *m_part1;
  };

public:
  BaseEditorWidget(QWidget *parent, const Description &descLeft, QWidget *center, const Description &descRight);
  QWidget *Create(const Description &desc);
};
