#pragma once

#include <editor/editorexport.hh>
#include <QWidget>
#include <editor/ui_assetresourcewidget.h>
#include <cobalt/core/csresourcelocator.hh>


class EDITOR_API AssetResourceWidget : public QWidget
{
  Q_OBJECT
public:
  AssetResourceWidget(QWidget *parent = 0);
  ~AssetResourceWidget();

  void SetResourceLocator(const cs::ResourceLocator &locator);
  const cs::ResourceLocator &GetResourceLocator() const;
  void AddValidClass(const cs::Class *cls);

  void Renamed(const cs::ResourceLocator &from, const cs::ResourceLocator &resourceLocator);

signals:
  void ResourceChanged(const cs::ResourceLocator &locator);

  private slots:
  void ResourceChangedSlot(const cs::ResourceLocator &locator);
private:
  Ui::AssetResourceUI m_gui;

  cs::ResourceLocator m_locator;
};