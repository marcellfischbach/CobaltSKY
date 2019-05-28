

#pragma once

#include <vector>
#include <QString>

namespace cs::editor::model
{
class Node;
}

namespace cs::editor::assetmanager
{

class ListListNode
{
  friend class ListListModel;
public:
  ListListNode(model::Node* node);
  ~ListListNode();
  
  QString GetName() const;
  QString GetType() const;

  model::Node *GetNode() 
  {
    return m_node;
  }

  bool HasParent(const ListListNode*node) const;

  int GetIndexOfChild(const ListListNode*child, int ifOrfan = -1) const;
  int GetIndexWithinParent(int ifOrfan = -1) const;
  size_t GetNumberOfChildren() const;
  const ListListNode*GetChild(size_t idx) const;

  void Sort();
  
private:


  static bool less(ListListNode*node0, ListListNode*node1);

  model::Node *m_node;

  ListListNode*m_parent;
  std::vector<ListListNode*> m_children;

};


}
