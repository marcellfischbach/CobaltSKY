#pragma once

#include <sstream>

namespace
{
vkString eval(vkString *src, vkSGDataType *dataTypes, vkSGNode *node, vkShaderGraphCtx &ctx, int start, int num)
{
  if (start >= num)
  {
    return "";
  }

  int next = start;
  std::ostringstream ss;
  if (src[start].length() != 0)
  {

    int numEquals = 1;
    if (dataTypes[start] != eSGDT_Float && dataTypes[start] != eSGDT_Int)
    {
      for (int i = start + 1; i < num; ++i)
      {
        if (src[start] == src[i] && node->GetInput(i)->GetInput()->GetAttr().length() != 0)
        {
          numEquals++;
        }
        else
        {
          break;
        }
      }
    }
    if (numEquals == 1)
    {
      ss << ctx.GetFullInputValue(node->GetInput(start));
    }
    else
    {
      ss << src[start] << ".";
      for (int i = 0; i < numEquals; ++i)
      {
        ss << node->GetInput(start + i)->GetInput()->GetAttr();
      }
    }
    next += numEquals;
  }
  else
  {
    ss << std::to_string(node->GetInput(start)->GetConst());
    next++;
  }

  vkString trail = ::eval(src, dataTypes, node, ctx, next, num);
  if (trail.length() != 0)
  {
    ss << ", " << trail;
  }
  return ss.str();
}
}

