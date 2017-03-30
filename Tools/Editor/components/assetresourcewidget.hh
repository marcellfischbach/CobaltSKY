#pragma once

#include <QWidget>
#include <ui_assetresourcewidget.h>
#include <valkyrie/core/vkresourcelocator.hh>


class AssetResourceWidget : public QWidget
{
  Q_OBJECT
public:
  AssetResourceWidget(QWidget *parent = 0);
  ~AssetResourceWidget();

  void SetResourceLocator(const vkResourceLocator &locator);
  void AddValidClass(const vkClass *cls);
signals:
  void ResourceChanged(const vkResourceLocator &locator);

  private slots:
  void ResourceChangedSlot(const vkResourceLocator &locator);
private:
  Ui::AssetResourceUI m_gui;

  vkResourceLocator m_locator;
};