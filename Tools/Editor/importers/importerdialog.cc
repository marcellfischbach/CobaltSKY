#include <importers/importerdialog.hh>
#include <importers/iimporter.hh>
#include <QGridLayout>


ImporterDialog::ImporterDialog(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent, f)
{

}



void ImporterDialog::AddImporter(iImporter *importer)
{
  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(importer->GetWidget(), 0, 0);
}
