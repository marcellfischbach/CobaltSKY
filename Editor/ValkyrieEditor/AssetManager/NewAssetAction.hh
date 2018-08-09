#ifndef NEWASSETACTION_HH
#define NEWASSETACTION_HH

#include <QAction>
#include <Valkyrie/Core/ResourceManager.hh>


namespace assetmanager
{

class NewAssetAction : public QAction
{
public:

  void SetLocator (const vkResourceLocator &locator);
  const vkResourceLocator &GetLocator () const;

  virtual bool activate ();

protected:
  NewAssetAction(const QString &name, QObject *parent);

private:
  vkResourceLocator m_locator;

};

}


#endif // NEWASSETACTION_HH
