

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
  TreeNode(model::Node* node);
  
  QString GetName() const;
  QString GetType() const;

  model::Node *GetNode() 
  {
    return m_node;
  }

  bool HasParent(const TreeNode *node) const;

  int GetIndexOfChild(const TreeNode *child, int ifOrfan = -1) const;
  int GetIndexWithinParent(int ifOrfan = -1) const;
  size_t GetNumberOfChildren() const;
  const TreeNode *GetChild(size_t idx) const;

  void Sort();
  
private:


  static bool less(TreeNode *node0, TreeNode *node1);

  model::Node *m_node;

  TreeNode *m_parent;
  std::vector<TreeNode *> m_children;

};


}
