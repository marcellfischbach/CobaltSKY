
#include <Valkyrie/Loaders/ShaderGraphLoader.hh>
#include <Valkyrie/Core/ClassRegistry.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <Valkyrie/Engine.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <map>





vkShaderGraphAssetXMLLoader::vkShaderGraphAssetXMLLoader()
  : vkBaseXMLLoader()
{

}


vkShaderGraphAssetXMLLoader::~vkShaderGraphAssetXMLLoader()
{

}

bool vkShaderGraphAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkString tagName(element->Value());
  return tagName == vkString("shadergraph");
}

IObject *vkShaderGraphAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();

  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  TiXmlElement *inputsElement = element->FirstChildElement("inputs");
  if (!nodesElement || !inputsElement)
  {
    shaderGraph->Release();
    return 0;
  }

  std::map<vkUInt32, vkSGNode*> nodes;
  for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node"); nodeElement; nodeElement = nodeElement->NextSiblingElement("node"))
  {
    if (!nodeElement->Attribute("id") || !nodeElement->Attribute("class"))
    {
      shaderGraph->Release();
      return 0;
    }

    vkUInt32 id = atoi(nodeElement->Attribute("id"));
    vkString className(nodeElement->Attribute("class"));

    const vkClass *cls = vkClassRegistry::Get()->GetClass(className);
    if (!cls)
    { 
      shaderGraph->Release();
      return 0;
    }

    vkSGNode *node = cls->CreateInstance<vkSGNode>();
    if (!node)
    {
      shaderGraph->Release();
      return 0;
    }

    vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
    TiXmlElement *resourceElement = nodeElement->FirstChildElement("resource");
    if (resource && !resourceElement)
    {
      shaderGraph->Release();
      return 0;
    }

    if (resource && resourceElement)
    {
      
      if (!resourceElement->Attribute("name"))
      {
        shaderGraph->Release();
        return 0;
      }

      vkString resourceName = vkString(resourceElement->Attribute("name"));
      resource->SetResourceName(resourceName);

      TiXmlElement *valueElement = resourceElement->FirstChildElement();
      if (valueElement)
      {
        vkString tagName(valueElement->Value());
        if (tagName == vkString("float"))
        {
          resource->SetDefault(LoadFloat(valueElement->Value()));
        }
        else if (tagName == vkString("float2"))
        {
          resource->SetDefault(LoadVector2f(valueElement->Value()));
        }
        else if (tagName == vkString("float3"))
        {
          resource->SetDefault(LoadVector3f(valueElement->Value()));
        }
        else if (tagName == vkString("float4"))
        {
          resource->SetDefault(LoadVector4f(valueElement->Value()));
        }
        else if (tagName == vkString("color4"))
        {
          resource->SetDefault(LoadColor4f(valueElement->Value()));
        }
        else if (tagName == vkString("locator"))
        {
          resource->SetDefaultTextureResource(vkResourceLocator(vkString(valueElement->GetText())));
        }
      }
    }

    shaderGraph->AddNode(node);
    nodes[id] = node;
    node->Release();
  }




  // now read the inputs for the nodes
  for (TiXmlElement *inputElement = inputsElement->FirstChildElement(); inputElement; inputElement = inputElement->NextSiblingElement())
  {
    vkSGOutput *output = 0;
    float fl = 0.0f;
    bool validInput = false;
    TiXmlElement *nodeElement = inputElement->FirstChildElement("node");
    if (nodeElement && nodeElement->Attribute("id") && nodeElement->Attribute("output"))
    {
      vkUInt32 id = atoi(nodeElement->Attribute("id"));
      vkUInt32 outputID = atoi(nodeElement->Attribute("output"));
      vkSGNode *outputNode = nodes[id];
      if (!outputNode)
      {
        shaderGraph->Release();
        return 0;
      }
      output = outputNode->GetOutput(outputID);
      if (!output)
      {
        shaderGraph->Release();
        return 0;
      }
      validInput = true;
    }
    TiXmlElement *floatElement = inputElement->FirstChildElement("float");
    if (floatElement && floatElement->GetText())
    {
      fl = (float)atof(floatElement->GetText());
      validInput = true;
    }

    if (!validInput)
    {
      shaderGraph->Release();
      return 0;
    }

    vkString tagName(inputElement->Value());
    if (tagName == vkString("shadergraph") && inputElement->Attribute("input"))
    {
      if (!output)
      {
        shaderGraph->Release();
        return 0;
      }
      vkString input(inputElement->Attribute("input"));
      vkSGShaderGraph::InputType it;
      if (input == vkString("Diffuse"))
      {
        it = vkSGShaderGraph::eIT_Diffuse;
      }
      else if (input == vkString("Alpha"))
      {
        it = vkSGShaderGraph::eIT_Alpha;
      }
      else if (input == vkString("Roughness"))
      {
        it = vkSGShaderGraph::eIT_Roughness;
      }
      else if (input == vkString("Normal"))
      {
        it = vkSGShaderGraph::eIT_Normal;
      }
      else 
      {
        shaderGraph->Release();
        return 0;
      }
      shaderGraph->SetInput(it, output);
    }
    else if (tagName == vkString("node") && inputElement->Attribute("id") && inputElement->Attribute("input"))
    {
      vkUInt32 nodeID = atoi(inputElement->Attribute("id"));
      vkUInt32 inputID = atoi(inputElement->Attribute("input"));
      vkSGNode *inputNode = nodes[nodeID];
      if (!inputNode)
      {
        shaderGraph->Release();
        return 0;
      }

      vkSGInput *input = inputNode->GetInput(inputID);
      if (!input)
      {
        shaderGraph->Release();
        return 0;
      }
      if (output)
      {
        input->SetInput(output);
      }
      else
      {
        input->SetConst(fl);
      }
    }
    else
    {
      shaderGraph->Release();
      return 0;
    }

  }

  IGraphics *graphics = vkEngine::Get()->GetRenderer();
  if (graphics && !graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph))
  {
    shaderGraph->Release();
    return 0;
  }


  return shaderGraph;
}
