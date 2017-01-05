
#include <Valkyrie/Loaders/vkshadergraphassetxmlloader.hh>
#include <Valkyrie/core/vkclassregistry.hh>
#include <Valkyrie/graphics/shadergraph/vksgnodes.hh>
#include <Valkyrie/graphics/shadergraph/vksgshadergraph.hh>
#include <Valkyrie/vkengine.hh>
#include <Valkyrie/graphics/igraphics.hh>
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
  return tagName == vkString("shaderGraph");
}

const vkClass *vkShaderGraphAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkSGShaderGraph::GetStaticClass();
}

IObject *vkShaderGraphAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkSGShaderGraph *shaderGraph = new vkSGShaderGraph();

  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  TiXmlElement *inputsElement = element->FirstChildElement("inputs");
  TiXmlElement *attributesElement = element->FirstChildElement("attributes");

  std::map<vkUInt32, vkSGNode*> nodes;
  if (nodesElement)
  {
    for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node"); nodeElement; nodeElement = nodeElement->NextSiblingElement("node"))
    {
      if (!nodeElement->Attribute("id") || !nodeElement->Attribute("class"))
      {
        continue;
      }

      vkUInt32 id = atoi(nodeElement->Attribute("id"));
      vkString className(nodeElement->Attribute("class"));

      const vkClass *cls = vkClassRegistry::Get()->GetClass(className);
      if (!cls)
      {
        continue;
      }

      vkSGNode *node = cls->CreateInstance<vkSGNode>();
      if (!node)
      {
        continue;
      }

      // 
      // add the node may be exiting later, but the node must be stored.
      shaderGraph->AddNode(node);
      nodes[id] = node;
      node->Release();


      vkSGResourceNode *resource = vkQueryClass<vkSGResourceNode>(node);
      TiXmlElement *resourceElement = nodeElement->FirstChildElement("resource");
      if (resource && resourceElement)
      {

        if (resourceElement->Attribute("name"))
        {
          vkString resourceName = vkString(resourceElement->Attribute("name"));
          resource->SetResourceName(resourceName);
        }


        TiXmlElement *valueElement = resourceElement->FirstChildElement();
        if (valueElement)
        {
          vkString tagName(valueElement->Value());
          if (tagName == vkString("float"))
          {
            resource->SetDefault(LoadFloat(valueElement->GetText()));
          }
          else if (tagName == vkString("float2"))
          {
            resource->SetDefault(LoadVector2f(valueElement->GetText()));
          }
          else if (tagName == vkString("float3"))
          {
            resource->SetDefault(LoadVector3f(valueElement->GetText()));
          }
          else if (tagName == vkString("float4"))
          {
            resource->SetDefault(LoadVector4f(valueElement->GetText()));
          }
          else if (tagName == vkString("color4"))
          {
            resource->SetDefault(LoadColor4f(valueElement->GetText()));
          }
          else if (tagName == vkString("locator"))
          {
            resource->SetDefaultTextureResource(vkResourceLocator(vkString(valueElement->GetText())));
          }
        }
      }

    }
  }




  // now read the inputs for the nodes
  if (inputsElement)
  {
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
        std::map<vkUInt32, vkSGNode*>::iterator it = nodes.find(id);
        if (it == nodes.end())
        {
          continue;
        }
        vkSGNode *outputNode = it->second;
        if (!outputNode)
        {
          continue;
        }
        output = outputNode->GetOutput(outputID);
        if (!output)
        {
          continue;
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
        continue;
      }

      vkString tagName(inputElement->Value());
      if (tagName == vkString("shaderGraph") && inputElement->Attribute("input"))
      {
        if (!output)
        {
          continue;
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
          continue;
        }
        shaderGraph->SetInput(it, output);
      }
      else if (tagName == vkString("node") && inputElement->Attribute("id") && inputElement->Attribute("input"))
      {
        vkUInt32 nodeID = atoi(inputElement->Attribute("id"));
        vkUInt32 inputID = atoi(inputElement->Attribute("input"));

        std::map<vkUInt32, vkSGNode*>::iterator it = nodes.find(nodeID);
        if (it == nodes.end())
        {
          continue;
        }

        vkSGNode *inputNode = it->second;
        if (!inputNode)
        {
          continue;
        }

        vkSGInput *input = inputNode->GetInput(inputID);
        if (!input)
        {
          continue;
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
        continue;
      }

    }
  }

  if (attributesElement)
  {
    //
    // load the attributes
    TiXmlElement *blendOutWithBinaryGradientElement = attributesElement->FirstChildElement("blendOutWithBinaryGradient");
    if (blendOutWithBinaryGradientElement)
    {
      if (blendOutWithBinaryGradientElement->Attribute("enabled"))
      {
        shaderGraph->SetBlendOutWithBinaryGradient(LoadBool(blendOutWithBinaryGradientElement->Attribute("enabled")));
      }
      else
      {
        shaderGraph->SetBlendOutWithBinaryGradient(false);
      }
    }
    TiXmlElement *discardAlphaElement = attributesElement->FirstChildElement("discardAlpha");
    if (discardAlphaElement)
    {
      if (discardAlphaElement->Attribute("enabled"))
      {
        shaderGraph->SetDiscardAlpha(LoadBool(discardAlphaElement->Attribute("enabled")));
        TiXmlElement *thresholdElement = discardAlphaElement->FirstChildElement("threshold");
        TiXmlElement *modeElement = discardAlphaElement->FirstChildElement("mode");
        float threshold = 0.0f;
        vkCompareMode compareMode = eCM_Less;
        if (thresholdElement)
        {
          threshold = LoadFloat(thresholdElement->GetText());
        }
        if (modeElement)
        {
          vkString cmpStr(modeElement->GetText());
#define CMP(cmp)  (cmpStr == #cmp)  compareMode = eCM_##cmp
          if CMP(LessOrEqual);
          else if CMP(GreaterOrEqual);
          else if CMP(Less);
          else if CMP(Greater);
          else if CMP(Equal);
          else if CMP(NotEqual);
          else if CMP(Never);
          else if CMP(Always);
#undef CMP
        }
        shaderGraph->SetDiscardAlpha(threshold, compareMode);
      }
    }
  }


  IGraphics *graphics = vkEng->GetRenderer();
  if (graphics)
  {
    graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph);
  }


  return shaderGraph;
}
