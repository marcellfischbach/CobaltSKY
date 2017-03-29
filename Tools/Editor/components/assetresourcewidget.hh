#pragma once

#include <QWidget>
#include <ui_assetresourcewidget.h>
#include <valkyrie/core/vkresourcelocator.hh>


class AssetResourceWidget : public QWidget
{
public:
  AssetResourceWidget(QWidget *parent = 0);
  ~AssetResourceWidget();

  void SetResourceLocator(const vkResourceLocator &locator);
public:
  Ui::AssetResourceUI m_gui;

  vkResourceLocator m_locator;
};