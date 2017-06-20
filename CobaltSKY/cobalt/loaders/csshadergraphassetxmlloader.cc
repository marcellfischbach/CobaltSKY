
#include <cobalt/loaders/csshadergraphassetxmlloader.hh>
#include <cobalt/core/csclassregistry.hh>
#include <cobalt/graphics/shadergraph/cssgnodes.hh>
#include <cobalt/graphics/shadergraph/cssgshadergraph.hh>
#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <map>





csShaderGraphAssetXMLLoader::csShaderGraphAssetXMLLoader()
  : csBaseXMLLoader()
{

}


csShaderGraphAssetXMLLoader::~csShaderGraphAssetXMLLoader()
{

}

bool csShaderGraphAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csString tagName(element->Value());
  return tagName == csString("shaderGraph");
}

const csClass *csShaderGraphAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csSGShaderGraph::GetStaticClass();
}

iObject *csShaderGraphAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csSGShaderGraph *shaderGraph = new csSGShaderGraph();

  TiXmlElement *nodesElement = element->FirstChildElement("nodes");
  TiXmlElement *inputsElement = element->FirstChildElement("inputs");
  TiXmlElement *attributesElement = element->FirstChildElement("attributes");

  std::map<csUInt32, csSGNode*> nodes;
  if (nodesElement)
  {
    for (TiXmlElement *nodeElement = nodesElement->FirstChildElement("node"); nodeElement; nodeElement = nodeElement->NextSiblingElement("node"))
    {
      if (!nodeElement->Attribute("id") || !nodeElement->Attribute("class"))
      {
        continue;
      }

      csUInt32 id = atoi(nodeElement->Attribute("id"));
      csString className(nodeElement->Attribute("class"));

      const csClass *cls = csClassRegistry::Get()->GetClass(className);
      if (!cls)
      {
        continue;
      }

      csSGNode *node = cls->CreateInstance<csSGNode>();
      if (!node)
      {
        continue;
      }

      // 
      // add the node may be exiting later, but the node must be stored.
      shaderGraph->AddNode(node);
      nodes[id] = node;
      node->Release();


      csSGResourceNode *resource = csQueryClass<csSGResourceNode>(node);
      TiXmlElement *resourceElement = nodeElement->FirstChildElement("resource");
      if (resource && resourceElement)
      {

        if (resourceElement->Attribute("name"))
        {
          csString resourceName = csString(resourceElement->Attribute("name"));
          resource->SetResourceName(resourceName);
        }


        TiXmlElement *valueElement = resourceElement->FirstChildElement();
        if (valueElement)
        {
          csString tagName(valueElement->Value());
          if (tagName == csString("float"))
          {
            resource->SetDefault(LoadFloat(valueElement->GetText()));
          }
          else if (tagName == csString("float2"))
          {
            resource->SetDefault(LoadVector2f(valueElement->GetText()));
          }
          else if (tagName == csString("float3"))
          {
            resource->SetDefault(LoadVector3f(valueElement->GetText()));
          }
          else if (tagName == csString("float4"))
          {
            resource->SetDefault(LoadVector4f(valueElement->GetText()));
          }
          else if (tagName == csString("color4"))
          {
            resource->SetDefault(LoadColor4f(valueElement->GetText()));
          }
          else if (tagName == csString("locator"))
          {
            resource->SetDefaultTextureResource(csResourceLocator(csString(valueElement->GetText())));
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
      csSGOutput *output = 0;
      float fl = 0.0f;
      bool validInput = false;
      TiXmlElement *nodeElement = inputElement->FirstChildElement("node");
      if (nodeElement && nodeElement->Attribute("id") && nodeElement->Attribute("output"))
      {
        csUInt32 id = atoi(nodeElement->Attribute("id"));
        csUInt32 outputID = atoi(nodeElement->Attribute("output"));
        std::map<csUInt32, csSGNode*>::iterator it = nodes.find(id);
        if (it == nodes.end())
        {
          continue;
        }
        csSGNode *outputNode = it->second;
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

      csString tagName(inputElement->Value());
      if (tagName == csString("shaderGraph") && inputElement->Attribute("input"))
      {
        if (!output)
        {
          continue;
        }
        csString input(inputElement->Attribute("input"));
        csSGShaderGraph::InputType it;
        if (input == csString("Diffuse"))
        {
          it = csSGShaderGraph::eIT_Diffuse;
        }
        else if (input == csString("Alpha"))
        {
          it = csSGShaderGraph::eIT_Alpha;
        }
        else if (input == csString("Roughness"))
        {
          it = csSGShaderGraph::eIT_Roughness;
        }
        else if (input == csString("Normal"))
        {
          it = csSGShaderGraph::eIT_Normal;
        }
        else
        {
          continue;
        }
        shaderGraph->SetInput(it, output);
      }
      else if (tagName == csString("node") && inputElement->Attribute("id") && inputElement->Attribute("input"))
      {
        csUInt32 nodeID = atoi(inputElement->Attribute("id"));
        csUInt32 inputID = atoi(inputElement->Attribute("input"));

        std::map<csUInt32, csSGNode*>::iterator it = nodes.find(nodeID);
        if (it == nodes.end())
        {
          continue;
        }

        csSGNode *inputNode = it->second;
        if (!inputNode)
        {
          continue;
        }

        csSGInput *input = inputNode->GetInput(inputID);
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
        csCompareMode compareMode = eCM_Less;
        if (thresholdElement)
        {
          threshold = LoadFloat(thresholdElement->GetText());
        }
        if (modeElement && modeElement->GetText())
        {
          csString cmpStr(modeElement->GetText());
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


  iGraphics *graphics = csEng->GetRenderer();
  if (graphics)
  {
    graphics->GetShaderGraphFactory()->GenerateShaderGraph(shaderGraph);
  }


  return shaderGraph;
}
