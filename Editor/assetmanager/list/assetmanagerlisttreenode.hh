

#pragma once

#include <vector>
#include <QString>

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class ListTreeNode
{
  friend class ListTreeModel;
public:
  ListTreeNode(model::Node* node);
  ~ListTreeNode();
  
  QString GetName() const;
  QString GetType() const;

  model::Node *GetNode() 
  {
    return m_node;
  }

  bool HasParent(const ListTreeNode*node) const;

  int GetIndexOfChild(const ListTreeNode*child, int ifOrfan = -1) const;
  int GetIndexWithinParent(int ifOrfan = -1) const;
  size_t GetNumberOfChildren() const;
  const ListTreeNode*GetChild(size_t idx) const;

  void Sort();
  
private:


  static bool less(ListTreeNode*node0, ListTreeNode*node1);

  model::Node *m_node;

  ListTreeNode*m_parent;
  std::vector<ListTreeNode*> m_children;

};


}
