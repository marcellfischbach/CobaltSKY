#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.refl.hh>
#include <Valkyrie/Defs.hh>

#include <Valkyrie/Core/String.hh>
#include <vector>

enum vkSGDataType
{
  eSGDT_Float = 0x01,
  eSGDT_Float2 = 0x02,
  eSGDT_Float3 = 0x04,
  eSGDT_Float4 = 0x08,
  eSGDT_Int    = 0x10,
  eSGDT_Int2 = 0x20,
  eSGDT_Int3 = 0x40,
  eSGDT_Int4 = 0x80,
};

class vkSGNode;


class VKE_API vkSGOutput
{
  friend class vkSGNode;
public:
  vkSGOutput(vkSGDataType type, const vkString &name, const vkString &attr);
  virtual ~vkSGOutput();

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  vkSGDataType GetType() const;
  const vkString &GetName() const;
  const vkString &GetAttr() const;

private:
  vkSGDataType m_dataType;
  vkString m_name;
  vkString m_attr;

  vkSGNode *m_node;

};


class VKE_API vkSGInput
{
  friend class vkSGNode;
public:
  vkSGInput(vkSGDataType type, const vkString &name);
  vkSGInput(vkUInt32 types, const vkString &name, bool canConst);
  virtual ~vkSGInput();

  const vkSGNode *GetNode() const;
  vkSGNode *GetNode();

  vkSGDataType GetType() const;
  const vkString &GetName() const;

  void SetOutput(vkSGOutput* output);
  vkSGOutput* GetOutput();
  const vkSGOutput* GetOutput() const;

  vkSGNode *GetInputNode();

  void SetConstDataPossible(bool constData);
  void SetConstData(unsigned i0);
  void SetConstData(unsigned i0, unsigned i1);
  void SetConstData(unsigned i0, unsigned i1, unsigned i2);
  void SetConstData(unsigned i0, unsigned i1, unsigned i2, unsigned i3);
  void SetConstData(float i0);
  void SetConstData(float i0, float i1);
  void SetConstData(float i0, float i1, float i2);
  void SetConstData(float i0, float i1, float i2, float i3);

  bool CanConstData() const;
  vkSGDataType GetConstDataType() const;
  unsigned GetInt(vkSize idx) const;
  float GetFloat(vkSize idx) const;
  
private:
  vkSGDataType m_dataType;
  vkString m_name;

  vkSGNode *m_node;

  vkSGOutput *m_output;

  bool m_canConstData;
  union
  {
    float f4[4];
    unsigned u4[4];
  } m_constData;
  vkSGDataType m_constDataType;
  vkUInt32 m_dataTypes;

};


class vkDataTypeValidationMapping
{

};


VK_CLASS()
class VKE_API vkSGNode : public vkObject
{
  VK_CLASS_GEN;
public:
  vkSGNode();
  virtual ~vkSGNode();

  const vkString &GetName() const;

  vkSize GetNumberOfInputs() const;
  vkSGInput *GetInput(vkSize idx);
  vkSGInput *GetInput(const vkString &name);

  vkSGNode *GetInputNode(vkSize idx);
  vkSGNode *GetInputNode(const vkString &name);


  vkSize GetNumberOfOutputs() const;
  vkSGOutput *GetOutput(vkSize idx);
  vkSGOutput *GetOutput(const vkString &name);


protected:
  void SetName(const vkString &name);

  void AddInput(vkSGInput *input);
  void AddOutput(vkSGOutput *output);

private:
  vkString m_name;

  std::vector<vkSGInput*> m_inputs;
  std::vector<vkSGOutput*> m_outputs;
};


VK_CLASS()
class VKE_API vkSGConstFloat : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat();
  virtual ~vkSGConstFloat();

  void SetValue(float value);
  float GetValue() const;

private:
  float m_value;
};


VK_CLASS()
class VKE_API vkSGConstFloat3 : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3();
  virtual ~vkSGConstFloat3();

  void SetValue(float x, float y, float z);
  float GetValueX() const;
  float GetValueY() const;
  float GetValueZ() const;

private:
  float m_valueX;
  float m_valueY;
  float m_valueZ;
};



VK_CLASS()
class VKE_API vkSGFloat3 : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGFloat3();
  virtual ~vkSGFloat3();

};

VK_CLASS()
class VKE_API vkSGAddFloat : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGAddFloat();
  virtual ~vkSGAddFloat();

};


VK_CLASS()
class VKE_API vkSGAddFloat3 : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGAddFloat3();
  virtual ~vkSGAddFloat3();

};


VK_CLASS()
class VKE_API vkSGSplitFloat3 : public vkSGNode
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat3();
  virtual ~vkSGSplitFloat3();

};




VK_FORCEINLINE const vkString &vkSGNode::GetName() const
{
  return m_name;
}





VK_FORCEINLINE vkSGDataType vkSGOutput::GetType() const
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






VK_FORCEINLINE vkSGDataType vkSGInput::GetType() const
{
  return m_dataType;
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

VK_FORCEINLINE void vkSGInput::SetOutput(vkSGOutput *output)
{
  m_output = output;
}

VK_FORCEINLINE vkSGOutput *vkSGInput::GetOutput()
{
  return m_output;
}

VK_FORCEINLINE const vkSGOutput *vkSGInput::GetOutput() const
{
  return m_output;
}



VK_FORCEINLINE void vkSGConstFloat::SetValue(float value)
{
  m_value = value;
}

VK_FORCEINLINE float vkSGConstFloat::GetValue() const
{
  return m_value;
}




VK_FORCEINLINE void vkSGConstFloat3::SetValue(float x, float y, float z)
{
  m_valueX = x;
  m_valueY = y;
  m_valueZ = z;
}

VK_FORCEINLINE float vkSGConstFloat3::GetValueX() const
{
  return m_valueX;
}

VK_FORCEINLINE float vkSGConstFloat3::GetValueY() const
{
  return m_valueY;
}

VK_FORCEINLINE float vkSGConstFloat3::GetValueZ() const
{
  return m_valueZ;
}



VK_FORCEINLINE bool vkSGInput::CanConstData() const
{
  return m_canConstData;
}

VK_FORCEINLINE vkSGDataType vkSGInput::GetConstDataType() const
{
  return m_constDataType;
}

VK_FORCEINLINE unsigned vkSGInput::GetInt(vkSize idx) const
{
  return m_constData.u4[idx];
}

VK_FORCEINLINE float vkSGInput::GetFloat(vkSize idx) const
{
  return m_constData.f4[idx];
}
