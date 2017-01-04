#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Core/vkclass.hh>
#include <Valkyrie/Core/vkresourcemanager.hh>
#include <Valkyrie/vkdefs.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Math/Vector.hh>

#include <Valkyrie/Core/vkstring.hh>
#include <vector>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.refl.hh>


enum vkSGDataType
{
  eSGDT_Inval = 0x00,
  eSGDT_Int = 0x01,
  eSGDT_Int2 = 0x02,
  eSGDT_Int3 = 0x04,
  eSGDT_Int4 = 0x08,
  eSGDT_Float = 0x10,
  eSGDT_Float2 = 0x20,
  eSGDT_Float3 = 0x40,
  eSGDT_Float4 = 0x80,
  eSGDT_IntTypes = 0x0f,
  eSGDT_FloatTypes = 0xf0,
  eSGDT_ScalarTypes = 0xff,

  eSGDT_Texture1D = 0x100,
  eSGDT_Texture1DArray = 0x200,
  eSGDT_Texture2D = 0x400,
  eSGDT_Texture2DArray = 0x800,
  eSGDT_Texture3D = 0x1000,
  eSGDT_TextureCube = 0x2000,
  eSGDT_TextureCubeArray = 0x4000,
  eSGDT_Textures = 0x7f00,
};



class vkSGNode;


class VKE_API vkSGOutput
{
  friend class vkSGNode;
public:
  vkSGOutput(vkSGDataType type, const vkString &name, const vkString &attr = "");
  vkSGOutput(const vkString &name, const vkString &attr = "");
  virtual ~vkSGOutput();

  void SetIdx(vkUInt32 idx);
  vkUInt32 GetIdx() const;

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  void SetDataType(vkSGDataType dataType);
  vkSGDataType GetDataType() const;
  const vkString &GetName() const;
  const vkString &GetAttr() const;

private:
  vkSGDataType m_dataType;
  vkString m_name;
  vkString m_attr;

  vkSGNode *m_node;

  vkUInt32 m_idx;

};


class VKE_API vkSGInput
{
  friend class vkSGNode;
public:
  vkSGInput(const vkString &name, bool canConst, bool canInputNode);
  virtual ~vkSGInput();

  void SetIdx(vkUInt32 idx);
  vkUInt32 GetIdx() const;

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  const vkString &GetName() const;

  void SetInput(vkSGOutput* output);
  vkSGOutput* GetInput();
  const vkSGOutput* GetInput() const;

  bool CanInputNode() const;
  vkSGNode *GetInputNode();

  bool CanInputConst() const;
  void SetConst(float fl);
  float GetConst() const;

  vkSGDataType GetDataType()const;
  
private:
  vkString m_name;

  vkSGNode *m_node;

  bool m_canInputNode;
  vkSGOutput *m_input;

  bool m_canInputConst;
  float m_constFloat;

  vkUInt32 m_idx;

};

VK_CLASS()
class VKE_API vkSGNode : public VK_SUPER(vkObject)
{

  VK_CLASS_GEN;
public:
  vkSGNode();
  virtual ~vkSGNode();

  const vkString &GetName() const;

  //void SetBindingName(const vkString &bindingName);
  //const vkString &GetBindingName() const;

  vkSize GetNumberOfInputs() const;
  vkSGInput *GetInput(vkSize idx) const;
  vkSGInput *GetInput(const vkString &name) const;
  vkSGDataType GetInputDataType(vkSize idx) const;
  vkSGDataType GetInputDataType(const vkString &name) const;

  void SetOutputDataType(vkSize idx, vkSGDataType dataType);
  void SetOutputDataType(const vkString &name, vkSGDataType dataType);

  void SetInput(int inputIdx, vkSGNode* node, int outputIdx = 0);
  void SetInput(const vkString &inputName, vkSGNode *node, int outputIdx = 0);
  void SetInput(int inputIdx, vkSGNode* node, const vkString &outputName);
  void SetInput(const vkString &inputName, vkSGNode *node, const vkString &outputName);
  void SetInput(int inputIdx, float constFloat);
  void SetInput(const vkString &inputName, float constFloat);

  vkSGNode *GetInputNode(vkSize idx);
  vkSGNode *GetInputNode(const vkString &name);


  vkSize GetNumberOfOutputs() const;
  vkSGOutput *GetOutput(vkSize idx);
  vkSGOutput *GetOutput(const vkString &name);


  bool NotInvalid(vkSGDataType dt) const;
  bool ScalarType(vkSGDataType dt) const;
  bool FloatType(vkSGDataType dt) const;
  bool IntType(vkSGDataType dt) const;


  bool NotInvalid(vkSGDataType dtA, vkSGDataType dtB) const;
  bool ScalarType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool FloatType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool IntType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameScalarType(vkSGDataType dtA, vkSGDataType dtB) const;
  bool SameTypeOrOne(vkSGDataType dtA, vkSGDataType dtB) const;
  vkSGDataType HighOrderType(vkSGDataType dtA, vkSGDataType dtB) const;

  virtual bool Validate();

//protected:
  void SetName(const vkString &name);

  void AddInput(vkSGInput *input);
  void AddOutput(vkSGOutput *output);

  void SetValidationMessage(const vkString &validationMessage);
  const vkString &GetValidationMessage() const;

#ifdef VK_BUILD_EDITOR
  const vkVector2f &GetPosition() const
  {
    return m_position;
  }
  void SetPosition(const vkVector2f &position)
  {
    m_position = position;
  }
#endif

private:
  vkString m_name;
  //vkString m_bindingName;
  vkString m_validationMessage;

  std::vector<vkSGInput*> m_inputs;
  std::vector<vkSGOutput*> m_outputs;

#ifdef VK_BUILD_EDITOR
  vkVector2f m_position;
#endif
};


class VKE_API vkSGNodes
{
public:
  struct Entry
  {
    vkString name;
    const vkClass *clazz;
    Entry (const vkString &name, const vkClass *clazz);
  };

public:
  static vkSGNodes *Get();

  const std::vector<Entry> &GetEntries () const;

private:
  vkSGNodes ();

  std::vector<Entry> m_entries;
};

VK_CLASS()
class VKE_API vkSGResourceNode : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGResourceNode();
  virtual ~vkSGResourceNode();

  void SetResourceName(const vkString &resourceName);
  const vkString &GetResourceName() const;

  vkShaderParameterType GetResourceType() const;

  void SetDefault(float value);
  void SetDefault(const vkVector2f &value);
  void SetDefault(const vkVector3f &value);
  void SetDefault(const vkVector4f &value);
  void SetDefault(const vkColor4f &value);
  void SetDefault(const vkMatrix3f &value);
  void SetDefault(const vkMatrix4f &value);
  float *GetDefaultFloats();
  int *GetDefaultInts();

  void SetDefaultTextureResource(const vkResourceLocator &locator);
  vkResourceLocator &GetDefaultTextureResource();

  const float *GetDefaultFloats() const;
  const int *GetDefaultInts() const;
  const vkResourceLocator &GetDefaultTextureResource() const;

protected:
  void SetResourceType(vkShaderParameterType type);

private:
  vkString m_resourceName;
  vkShaderParameterType m_resourceType;

  float m_defaultFloats[16];
  int m_defaultInts[16];
  vkResourceLocator m_defaultTextureResource;

};

VK_CLASS()
class VKE_API vkSGVarFloat : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat();
  virtual ~vkSGVarFloat() { }
};

VK_CLASS()
class VKE_API vkSGVarFloat2 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat2();
  virtual ~vkSGVarFloat2() { }
};

VK_CLASS()
class VKE_API vkSGVarFloat3 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat3();
  virtual ~vkSGVarFloat3() { }
};


VK_CLASS()
class VKE_API vkSGVarFloat4 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGVarFloat4();
  virtual ~vkSGVarFloat4() { }
};

VK_CLASS()
class VKE_API vkSGConstFloat : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat();
  virtual ~vkSGConstFloat() { };
};

VK_CLASS()
class VKE_API vkSGConstFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat2();
  virtual ~vkSGConstFloat2() { };
};

VK_CLASS()
class VKE_API vkSGConstFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3();
  virtual ~vkSGConstFloat3() { };
};

VK_CLASS()
class VKE_API vkSGConstFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat4();
  virtual ~vkSGConstFloat4() { };
};


VK_CLASS()
class VKE_API vkSGFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat2();
  virtual ~vkSGFloat2() { };
};

VK_CLASS()
class VKE_API vkSGFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat3();
  virtual ~vkSGFloat3() { };
};

VK_CLASS()
class VKE_API vkSGFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat4();
  virtual ~vkSGFloat4() { };
};


VK_CLASS()
class VKE_API vkSGSplitFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat2();
  virtual ~vkSGSplitFloat2() { }

  virtual bool Validate();
};

VK_CLASS()
class VKE_API vkSGSplitFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat3();
  virtual ~vkSGSplitFloat3() { }

  virtual bool Validate();
};

VK_CLASS()
class VKE_API vkSGSplitFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat4();
  virtual ~vkSGSplitFloat4() { }

  virtual bool Validate();
};


VK_CLASS()
class VKE_API vkSGAdd : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGAdd();
  virtual ~vkSGAdd() { }

  virtual bool Validate();

};

VK_CLASS()
class VKE_API vkSGSub : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSub();
  virtual ~vkSGSub() { }

  virtual bool Validate();
};


VK_CLASS()
class VKE_API vkSGMul : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGMul();
  virtual ~vkSGMul() { }

  virtual bool Validate();
};


VK_CLASS()
class VKE_API vkSGDiv : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDiv();
  virtual ~vkSGDiv() { }

  virtual bool Validate();
};

VK_CLASS()
class VKE_API vkSGDot : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDot();
  virtual ~vkSGDot() { }

  virtual bool Validate();
};

VK_CLASS()
class VKE_API vkSGCross : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGCross();
  virtual ~vkSGCross() { }

  virtual bool Validate();
};


VK_CLASS()
class VKE_API vkSGLerp : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGLerp();
  virtual ~vkSGLerp() { }

  virtual bool Validate();
};
VK_CLASS()
class VKE_API vkSGDefaultTextureCoordinate : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDefaultTextureCoordinate();
  virtual ~vkSGDefaultTextureCoordinate() { }

};

VK_CLASS()
class VKE_API vkSGTexture2D : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2D();
  virtual ~vkSGTexture2D() { }

};




VK_FORCEINLINE const vkString &vkSGNode::GetName() const
{
  return m_name;
}

/*
VK_FORCEINLINE void vkSGNode::SetBindingName(const vkString &bindingName)
{
  m_bindingName = bindingName;
}

VK_FORCEINLINE const vkString &vkSGNode::GetBindingName() const
{
  return m_bindingName;
}
*/

VK_FORCEINLINE void vkSGNode::SetName(const vkString &name)
{
  m_name = name;
}

VK_FORCEINLINE void vkSGNode::SetValidationMessage(const vkString &validationMessage)
{
  m_validationMessage = validationMessage;
}

VK_FORCEINLINE const vkString &vkSGNode::GetValidationMessage() const
{
  return m_validationMessage;
}


VK_FORCEINLINE void vkSGOutput::SetIdx(vkUInt32 idx)
{
  m_idx = idx;
}
VK_FORCEINLINE vkUInt32 vkSGOutput::GetIdx() const
{
  return m_idx;
}


VK_FORCEINLINE void vkSGOutput::SetDataType(vkSGDataType dataType)
{
  m_dataType = dataType;
}

VK_FORCEINLINE vkSGDataType vkSGOutput::GetDataType() const
{
  return m_dataType;
}

VK_FORCEINLINE const vkString &vkSGOutput::GetName() const
{
  return m_name;
}

VK_FORCEINLINE const vkString &vkSGOutput::GetAttr() const
{
  return m_attr;
}

VK_FORCEINLINE vkSGNode *vkSGOutput::GetNode()
{
  return m_node;
}

VK_FORCEINLINE const vkSGNode *vkSGOutput::GetNode() const
{
  return m_node;
}

VK_FORCEINLINE void vkSGInput::SetIdx(vkUInt32 idx)
{
  m_idx = idx;
}
VK_FORCEINLINE vkUInt32 vkSGInput::GetIdx() const
{
  return m_idx;
}


VK_FORCEINLINE const vkString &vkSGInput::GetName() const
{
  return m_name;
}

VK_FORCEINLINE vkSGNode *vkSGInput::GetNode()
{
  return m_node;
}

VK_FORCEINLINE const vkSGNode *vkSGInput::GetNode() const
{
  return m_node;
}


VK_FORCEINLINE vkSGOutput *vkSGInput::GetInput()
{
  return m_input;
}

VK_FORCEINLINE const vkSGOutput *vkSGInput::GetInput() const
{
  return m_input;
}

VK_FORCEINLINE bool vkSGInput::CanInputNode() const
{
  return m_canInputNode;
}

VK_FORCEINLINE bool vkSGInput::CanInputConst() const
{
  return m_canInputConst;
}

VK_FORCEINLINE void vkSGInput::SetConst(float fl)
{
  m_constFloat = fl;
}

VK_FORCEINLINE float vkSGInput::GetConst() const
{
  return m_constFloat;
}
