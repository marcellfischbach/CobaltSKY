

#include <ShaderGraph/EditResourceDefaultDialog.hh>
#include <ShaderGraph/SGNode.hh>



namespace shadergraph
{


EditResourceDefaultDialog::EditResourceDefaultDialog(QWidget *parent)
  : QDialog(parent)
{
  setModal(false);
  setAttribute(Qt::WA_DeleteOnClose, true);

  m_gui.setupUi(this);
}

EditResourceDefaultDialog::~EditResourceDefaultDialog()
{

}

static QString type_name_map[] = {
  QString("Float"),
  QString("Vec2"),
  QString("Vec3"),
  QString("Vec4"),
  QString("Int"),
  QString("IVec2"),
  QString("IVec3"),
  QString("IVec4"),
  QString("Color4"),
  QString("Mat3"),
  QString("Mat4"),
  QString("Texture"),
};

void EditResourceDefaultDialog::SetNode(SGNode *node)
{
  m_node = node;

  if (!node->IsResources())
  {
    setVisible(false);
    return;
  }

  m_gui.lblName->setText(node->GetName());
  m_gui.lblType->setText(type_name_map[node->GetResourceType()]);
  m_gui.stackedWidget->setCurrentIndex(node->GetResourceType());


  m_gui.sbFloat->setValue(node->GetDefaultFloat()[0]);

  m_gui.sbVec2x->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbVec2y->setValue(node->GetDefaultFloat()[1]);

  m_gui.sbVec3x->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbVec3y->setValue(node->GetDefaultFloat()[1]);
  m_gui.sbVec3z->setValue(node->GetDefaultFloat()[2]);

  m_gui.sbVec4x->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbVec4y->setValue(node->GetDefaultFloat()[1]);
  m_gui.sbVec4z->setValue(node->GetDefaultFloat()[2]);
  m_gui.sbVec4w->setValue(node->GetDefaultFloat()[3]);

  m_gui.sbInt->setValue(node->GetDefaultInt()[0]);

  m_gui.sbIVec2x->setValue(node->GetDefaultInt()[0]);
  m_gui.sbIVec2y->setValue(node->GetDefaultInt()[1]);

  m_gui.sbIVec3x->setValue(node->GetDefaultInt()[0]);
  m_gui.sbIVec3y->setValue(node->GetDefaultInt()[1]);
  m_gui.sbIVec3z->setValue(node->GetDefaultInt()[2]);

  m_gui.sbIVec4x->setValue(node->GetDefaultInt()[0]);
  m_gui.sbIVec4y->setValue(node->GetDefaultInt()[1]);
  m_gui.sbIVec4z->setValue(node->GetDefaultInt()[2]);
  m_gui.sbIVec4w->setValue(node->GetDefaultInt()[3]);

  m_gui.sbCol4r->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbCol4g->setValue(node->GetDefaultFloat()[1]);
  m_gui.sbCol4b->setValue(node->GetDefaultFloat()[2]);
  m_gui.sbCol4a->setValue(node->GetDefaultFloat()[3]);

  m_gui.sbMat300->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbMat301->setValue(node->GetDefaultFloat()[1]);
  m_gui.sbMat302->setValue(node->GetDefaultFloat()[2]);
  m_gui.sbMat310->setValue(node->GetDefaultFloat()[3]);
  m_gui.sbMat311->setValue(node->GetDefaultFloat()[4]);
  m_gui.sbMat312->setValue(node->GetDefaultFloat()[5]);
  m_gui.sbMat320->setValue(node->GetDefaultFloat()[6]);
  m_gui.sbMat321->setValue(node->GetDefaultFloat()[7]);
  m_gui.sbMat322->setValue(node->GetDefaultFloat()[8]);


  m_gui.sbMat400->setValue(node->GetDefaultFloat()[0]);
  m_gui.sbMat401->setValue(node->GetDefaultFloat()[1]);
  m_gui.sbMat402->setValue(node->GetDefaultFloat()[2]);
  m_gui.sbMat403->setValue(node->GetDefaultFloat()[3]);
  m_gui.sbMat410->setValue(node->GetDefaultFloat()[4]);
  m_gui.sbMat411->setValue(node->GetDefaultFloat()[5]);
  m_gui.sbMat412->setValue(node->GetDefaultFloat()[6]);
  m_gui.sbMat413->setValue(node->GetDefaultFloat()[7]);
  m_gui.sbMat420->setValue(node->GetDefaultFloat()[8]);
  m_gui.sbMat421->setValue(node->GetDefaultFloat()[9]);
  m_gui.sbMat422->setValue(node->GetDefaultFloat()[10]);
  m_gui.sbMat423->setValue(node->GetDefaultFloat()[11]);
  m_gui.sbMat430->setValue(node->GetDefaultFloat()[12]);
  m_gui.sbMat431->setValue(node->GetDefaultFloat()[13]);
  m_gui.sbMat432->setValue(node->GetDefaultFloat()[14]);
  m_gui.sbMat433->setValue(node->GetDefaultFloat()[15]);

  m_gui.leTextureRefFile->setText(QString(node->GetDefaultTexture().GetResourceFile().c_str()));
  m_gui.leTextureRefName->setText(QString(node->GetDefaultTexture().GetResourceName().c_str()));

}


void EditResourceDefaultDialog::on_pbOK_clicked(bool)
{
  if (!m_node || !m_node->IsResources())
  {
    setVisible(false);
    return;
  }

  float *floats = m_node->GetDefaultFloat();
  int *ints = m_node->GetDefaultInt();
  vkResourceLocator &texture = m_node->GetDefaultTexture();
  switch (m_node->GetResourceType())
  {
  case eSPT_Float:
    floats[0] = m_gui.sbFloat->value();
    break;
  case eSPT_Vector2:
    floats[0] = m_gui.sbVec2x->value();
    floats[1] = m_gui.sbVec2y->value();
    break;
  case eSPT_Vector3:
    floats[0] = m_gui.sbVec3x->value();
    floats[1] = m_gui.sbVec3y->value();
    floats[2] = m_gui.sbVec3z->value();
    break;
  case eSPT_Vector4:
    floats[0] = m_gui.sbVec4x->value();
    floats[1] = m_gui.sbVec4y->value();
    floats[2] = m_gui.sbVec4z->value();
    floats[3] = m_gui.sbVec4w->value();
    break;
  case eSPT_Int:
    ints[0] = m_gui.sbInt->value();
    break;
  case eSPT_IVector2:
    ints[0] = m_gui.sbIVec2x->value();
    ints[1] = m_gui.sbIVec2y->value();
    break;
  case eSPT_IVector3:
    ints[0] = m_gui.sbIVec3x->value();
    ints[1] = m_gui.sbIVec3y->value();
    ints[2] = m_gui.sbIVec3z->value();
    break;
  case eSPT_IVector4:
    ints[0] = m_gui.sbIVec4x->value();
    ints[1] = m_gui.sbIVec4y->value();
    ints[2] = m_gui.sbIVec4z->value();
    ints[3] = m_gui.sbIVec4w->value();
    break;
  case eSPT_Color4:
    floats[0] = m_gui.sbCol4r->value();
    floats[1] = m_gui.sbCol4g->value();
    floats[2] = m_gui.sbCol4b->value();
    floats[3] = m_gui.sbCol4a->value();
    break;
  case eSPT_Matrix3:
    floats[0] = m_gui.sbMat300->value();
    floats[1] = m_gui.sbMat301->value();
    floats[2] = m_gui.sbMat302->value();
    floats[3] = m_gui.sbMat310->value();
    floats[4] = m_gui.sbMat311->value();
    floats[5] = m_gui.sbMat312->value();
    floats[6] = m_gui.sbMat320->value();
    floats[7] = m_gui.sbMat321->value();
    floats[8] = m_gui.sbMat322->value();
    break;
  case eSPT_Matrix4:
    floats[0] = m_gui.sbMat400->value();
    floats[1] = m_gui.sbMat401->value();
    floats[2] = m_gui.sbMat402->value();
    floats[3] = m_gui.sbMat403->value();
    floats[4] = m_gui.sbMat410->value();
    floats[5] = m_gui.sbMat411->value();
    floats[6] = m_gui.sbMat412->value();
    floats[7] = m_gui.sbMat413->value();
    floats[8] = m_gui.sbMat420->value();
    floats[9] = m_gui.sbMat421->value();
    floats[10] = m_gui.sbMat422->value();
    floats[11] = m_gui.sbMat423->value();
    floats[12] = m_gui.sbMat430->value();
    floats[13] = m_gui.sbMat431->value();
    floats[14] = m_gui.sbMat432->value();
    floats[15] = m_gui.sbMat433->value();
    break;
  case eSPT_Texture:
    texture = vkResourceLocator(
      vkString((const char*)m_gui.leTextureRefFile->text().toLatin1()),
      vkString((const char*)m_gui.leTextureRefName->text().toLatin1())
      );
    break;
  }

  emit Changed(m_node);
  setVisible(false);
}


void EditResourceDefaultDialog::on_pbCancel_clicked(bool)
{
  setVisible(false);
}

}