

#pragma once

#include <vector>
#include <QString>

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class TreeNode
{
  friend class TreeModel;
public:
  TreeNode(const model::Node* node);
  
  QString GetName() const;
  QString GetType() const;

  int GetIndexOfChild(const TreeNode *child, int ifOrfan = -1) const;
  int GetIndexWithinParent(int ifOrfan = -1) const;
  size_t GetNumberOfChildren() const;
  const TreeNode *GetChild(size_t idx) const;
  
private:
  const model::Node *m_node;

  TreeNode *m_parent;
  std::vector<TreeNode *> m_children;

};


}
