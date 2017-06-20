#pragma once

#include <QWidget>
#include <ui_assetresourcewidget.h>
#include <cobalt/core/csresourcelocator.hh>


class AssetResourceWidget : public QWidget
{
  Q_OBJECT
public:
  AssetResourceWidget(QWidget *parent = 0);
  ~AssetResourceWidget();

  void SetResourceLocator(const csResourceLocator &locator);
  void AddValidClass(const csClass *cls);
signals:
  void ResourceChanged(const csResourceLocator &locator);

  private slots:
  void ResourceChangedSlot(const csResourceLocator &locator);
private:
  Ui::AssetResourceUI m_gui;

  csResourceLocator m_locator;
};