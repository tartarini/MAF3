////////////////////////////////////////////////////////////////////////////////

//   Author:    Andy Rushton
//   Copyright: (c) Southampton University 1999-2004
//              (c) Andy Rushton           2004-2009
//   License:   BSD License, see ../docs/license.html

////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>

namespace mafCore
{

  ////////////////////////////////////////////////////////////////////////////////
  // mafTreeNode

  template<typename T>
  class mafTreeNode
  {
  public:
    master_iterator<mafTree<T>, mafTreeNode<T> > m_master;
    T m_data;
    mafTreeNode<T>* m_parent;
    std::vector<mafTreeNode<T>*> m_children;

  public:
    mafTreeNode(const mafTree<T>* owner, const T& data = T()) :
      m_master(owner,this), m_data(data), m_parent(0)
      {
      }

    void change_owner(const mafTree<T>* owner)
      {
        m_master.change_owner(owner);
        for (TYPENAME std::vector<mafTreeNode<T>*>::iterator i = m_children.begin(); i != m_children.end(); i++)
          (*i)->change_owner(owner);
      }

    ~mafTreeNode(void)
      {
        m_parent = 0;
        for (TYPENAME std::vector<mafTreeNode<T>*>::iterator i = m_children.begin(); i != m_children.end(); i++)
          delete *i;
      }

  };

  template<typename T>
  static mafTreeNode<T>* mafTree_copy(const mafTree<T>* new_owner, mafTreeNode<T>* root)
  {
    if (!root) return 0;
    mafTreeNode<T>* new_tree = new mafTreeNode<T>(new_owner, root->m_data);
    for (TYPENAME std::vector<mafTreeNode<T>*>::iterator i = root->m_children.begin(); i != root->m_children.end(); i++)
    {
      mafTreeNode<T>* new_child = mafTree_copy(new_owner, *i);
      new_tree->m_children.push_back(new_child);
      new_child->m_parent = new_tree;
    }
    return new_tree;
  }

  template<typename T>
  static unsigned mafTree_size(mafTreeNode<T>* root)
  {
    if (!root) return 0;
    unsigned result = 1;
    for (TYPENAME std::vector<mafTreeNode<T>*>::iterator i = root->m_children.begin(); i != root->m_children.end(); i++)
      result += mafTree_size(*i);
    return result;
  }

  template<typename T>
  static unsigned mafTree_depth(mafTreeNode<T>* root)
  {
    unsigned depth = 0;
    for (mafTreeNode<T>* i = root; i; i = i->m_parent)
      depth++;
    return depth;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // mafTreeIterator

  // constructor to create a null iterator - you must assign a valid value to this iterator before using it
  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>::mafTreeIterator(void)
  {
  }

  // used to create an alias of an iterator
  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>::mafTreeIterator(const mafSafeIterator<mafTree<T>, mafTreeNode<T> >& iterator) :
    mafSafeIterator<mafTree<T>,mafTreeNode<T> >(iterator)
  {
  }

  // constructor used by mafTree to create a non-null iterator
  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>::mafTreeIterator(mafTreeNode<T>* node) :
    mafSafeIterator<mafTree<T>,mafTreeNode<T> >(node->m_master)
  {
  }

  // constructor used by mafTree to create an end iterator
  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>::mafTreeIterator(const mafTree<T>* owner) :
    mafSafeIterator<mafTree<T>,mafTreeNode<T> >(owner)
  {
  }

  // destructor
  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>::~mafTreeIterator(void)
  {
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreeIterator<T,TRef,TPtr>::const_iterator mafTreeIterator<T,TRef,TPtr>::constify(void) const
  {
    return mafTreeIterator<T,const T&,const T*>(*this);
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreeIterator<T,TRef,TPtr>::iterator mafTreeIterator<T,TRef,TPtr>::deconstify(void) const
  {
    return mafTreeIterator<T,T&,T*>(*this);
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreeIterator<T,TRef,TPtr>::operator == (const TYPENAME mafTreeIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return equal(r);
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreeIterator<T,TRef,TPtr>::operator != (const TYPENAME mafTreeIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return !operator==(r);
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreeIterator<T,TRef,TPtr>::operator < (const TYPENAME mafTreeIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return compare(r) < 0;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreeIterator<T,TRef,TPtr>::reference mafTreeIterator<T,TRef,TPtr>::operator*(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    this->assert_valid();
    return this->node()->m_data;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreeIterator<T,TRef,TPtr>::pointer mafTreeIterator<T,TRef,TPtr>::operator->(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    return &(operator*());
  }

  ////////////////////////////////////////////////////////////////////////////////
  // mafTreePrefixIterator

  template<typename T, typename TRef, typename TPtr>
  mafTreePrefixIterator<T,TRef,TPtr>::mafTreePrefixIterator(void)
  {
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreePrefixIterator<T,TRef,TPtr>::~mafTreePrefixIterator(void)
  {
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreePrefixIterator<T,TRef,TPtr>::mafTreePrefixIterator(const mafTreeIterator<T,TRef,TPtr>& i) :
    m_iterator(i)
  {
    // this is initialised with the root node
    // which is also the first node in prefix traversal order
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::null(void) const
  {
    return m_iterator.null();
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::end(void) const
  {
    return m_iterator.end();
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::valid(void) const
  {
    return m_iterator.valid();
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::const_iterator mafTreePrefixIterator<T,TRef,TPtr>::constify(void) const
  {
    return mafTreePrefixIterator<T,const T&,const T*>(m_iterator);
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::iterator mafTreePrefixIterator<T,TRef,TPtr>::deconstify(void) const
  {
    return mafTreePrefixIterator<T,T&,T*>(m_iterator);
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr> mafTreePrefixIterator<T,TRef,TPtr>::simplify(void) const
  {
    return m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::operator == (const TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator == r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::operator != (const TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator != r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePrefixIterator<T,TRef,TPtr>::operator < (const TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator < r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::this_iterator& mafTreePrefixIterator<T,TRef,TPtr>::operator ++ (void)
    throw(mafNullDereference,mafEndDereference)
  {
    // pre-increment operator
    // algorithm: if there are any children, visit child 0, otherwise, go to
    // parent and deduce which child the start node was of that parent - if
    // there are further children, go into the next one. Otherwise, go up the
    // tree and test again for further children. Return null if there are no
    // further nodes
    m_iterator.assert_valid();
    mafTreeNode<T>* old_node = m_iterator.node();
    if (!old_node->m_children.empty())
    {
      // simply take the first child of this node
      m_iterator.set(old_node->m_children[0]->m_master);
    }
    else
    {
      // this loop walks up the parent pointers
      // either it will walk off the top and exit or a new node will be found and the loop will exit
      for (;;)
      {
        // go up a level
        mafTreeNode<T>* parent = old_node->m_parent;
        if (!parent)
        {
          // we've walked off the top of the tree, so return end
          m_iterator.set_end();
          break;
        }
        else
        {
          // otherwise walk down the next child - if there is one
          // find which index the old node was relative to this node
          TYPENAME std::vector<mafTreeNode<T>*>::iterator found =
            std::find(parent->m_children.begin(), parent->m_children.end(), old_node);
          // if this was found, then see if there is another and if so return that
          found++;
          if (found != parent->m_children.end())
          {
            // visit the next child
            m_iterator.set((*found)->m_master);
            break;
          }
          else
          {
            // keep going up
            old_node = parent;
          }
        }
      }
    }
    return *this;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::this_iterator mafTreePrefixIterator<T,TRef,TPtr>::operator ++ (int)
    throw(mafNullDereference,mafEndDereference)
  {
    // post-increment is defined in terms of the pre-increment
    mafTreePrefixIterator<T,TRef,TPtr> result(*this);
    ++(*this);
    return result;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::reference mafTreePrefixIterator<T,TRef,TPtr>::operator*(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    return m_iterator.operator*();
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePrefixIterator<T,TRef,TPtr>::pointer mafTreePrefixIterator<T,TRef,TPtr>::operator->(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    return m_iterator.operator->();
  }

  template<typename T, typename TRef, typename TPtr>
  const mafTreeIterator<T,TRef,TPtr>& mafTreePrefixIterator<T,TRef,TPtr>::get_iterator(void) const
  {
    return m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>& mafTreePrefixIterator<T,TRef,TPtr>::get_iterator(void)
  {
    return m_iterator;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // mafTreePostfixIterator

  template<typename T, typename TRef, typename TPtr>
  mafTreePostfixIterator<T,TRef,TPtr>::mafTreePostfixIterator(void)
  {
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreePostfixIterator<T,TRef,TPtr>::~mafTreePostfixIterator(void)
  {
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreePostfixIterator<T,TRef,TPtr>::mafTreePostfixIterator(const mafTreeIterator<T,TRef,TPtr>& i) :
    m_iterator(i)
  {
    // this is initialised with the root node
    // initially traverse to the first node to be visited
    if (m_iterator.valid())
    {
      mafTreeNode<T>* node = m_iterator.node();
      while (!node->m_children.empty())
        node = node->m_children[0];
      m_iterator.set(node->m_master);
    }
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::null(void) const
  {
    return m_iterator.null();
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::end(void) const
  {
    return m_iterator.end();
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::valid(void) const
  {
    return m_iterator.valid();
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::const_iterator mafTreePostfixIterator<T,TRef,TPtr>::constify(void) const
  {
    return mafTreePostfixIterator<T,const T&,const T*>(m_iterator);
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::iterator mafTreePostfixIterator<T,TRef,TPtr>::deconstify(void) const
  {
    return mafTreePostfixIterator<T,T&,T*>(m_iterator);
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr> mafTreePostfixIterator<T,TRef,TPtr>::simplify(void) const
  {
    return m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::operator == (const TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator == r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::operator != (const TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator != r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  bool mafTreePostfixIterator<T,TRef,TPtr>::operator < (const TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::this_iterator& r) const
  {
    return m_iterator < r.m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::this_iterator& mafTreePostfixIterator<T,TRef,TPtr>::operator ++ (void)
    throw(mafNullDereference,mafEndDereference)
  {
    // pre-increment operator
    // algorithm: this node has been visited, therefore all children must have
    // already been visited. So go to parent. Return null if the parent is null.
    // Otherwise deduce which child the start node was of that parent - if there
    // are further children, go into the next one and then walk down any
    // subsequent first-child pointers to the bottom. Otherwise, if there are no
    // children then the parent node is the next in the traversal.
    m_iterator.assert_valid();
    // go up a level
    mafTreeNode<T>* old_node = m_iterator.node();
    mafTreeNode<T>* parent = old_node->m_parent;
    if (!parent)
    {
      // we've walked off the top of the tree, so return end
      m_iterator.set_end();
    }
    else
    {
      // otherwise find which index the old node was relative to this node
      TYPENAME std::vector<mafTreeNode<T>*>::iterator found =
        std::find(parent->m_children.begin(), parent->m_children.end(), old_node);
      // if this was found, then see if there is another
      found++;
      if (found != parent->m_children.end())
      {
        // if so traverse to it and walk down the leftmost child pointers to the bottom of the new sub-tree
        mafTreeNode<T>* new_node = *found;
        while (!new_node->m_children.empty())
          new_node = new_node->m_children[0];
        m_iterator.set(new_node->m_master);
      }
      else
      {
        // the parent's children have all been visited - so the parent is visited
        m_iterator.set(parent->m_master);
      }
    }
    return *this;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::this_iterator mafTreePostfixIterator<T,TRef,TPtr>::operator ++ (int)
    throw(mafNullDereference,mafEndDereference)
  {
    // post-increment is defined in terms of the pre-increment
    mafTreePostfixIterator<T,TRef,TPtr> result(*this);
    ++(*this);
    return result;
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::reference mafTreePostfixIterator<T,TRef,TPtr>::operator*(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    return m_iterator.operator*();
  }

  template<typename T, typename TRef, typename TPtr>
  TYPENAME mafTreePostfixIterator<T,TRef,TPtr>::pointer mafTreePostfixIterator<T,TRef,TPtr>::operator->(void) const
    throw(mafNullDereference,mafEndDereference)
  {
    return m_iterator.operator->();
  }

  template<typename T, typename TRef, typename TPtr>
  const mafTreeIterator<T,TRef,TPtr>& mafTreePostfixIterator<T,TRef,TPtr>::get_iterator(void) const
  {
    return m_iterator;
  }

  template<typename T, typename TRef, typename TPtr>
  mafTreeIterator<T,TRef,TPtr>& mafTreePostfixIterator<T,TRef,TPtr>::get_iterator(void)
  {
    return m_iterator;
  }

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // mafTree
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  mafTree<T>::mafTree(void) : m_root(0)
  {
  }

  template<typename T>
  mafTree<T>::~mafTree(void)
  {
    if (m_root) delete m_root;
  }

  template<typename T>
  mafTree<T>::mafTree(const mafTree<T>& r) : m_root(0)
  {
    *this = r;
  }

  template<typename T>
  mafTree<T>& mafTree<T>::operator=(const mafTree<T>& r)
  {
    if (m_root) delete m_root;
    m_root = mafTree_copy(this, r.m_root);
    return *this;
  }

  template<typename T>
  bool mafTree<T>::empty(void) const
  {
    return m_root == 0;
  }

  template<typename T>
  unsigned mafTree<T>::size(void) const
  {
    return mafTree_size(m_root);
  }

  template<typename T>
  unsigned mafTree<T>::size(const TYPENAME mafTree<T>::const_iterator& i) const
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return mafTree_size(i.node());
  }

  template<typename T>
  unsigned mafTree<T>::size(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return mafTree_size(i.node());
  }

  template<typename T>
  unsigned mafTree<T>::depth(const TYPENAME mafTree<T>::const_iterator& i) const
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return mafTree_depth(i.node());
  }

  template<typename T>
  unsigned mafTree<T>::depth(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return mafTree_depth(i.node());
  }

  template<typename T>
  TYPENAME mafTree<T>::const_iterator mafTree<T>::root(void) const
  {
    if (!m_root) return mafTreeIterator<T,const T&,const T*>(this);
    return mafTreeIterator<T,const T&,const T*>(m_root);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::root(void)
  {
    if (!m_root) return mafTreeIterator<T,T&,T*>(this);
    return mafTreeIterator<T,T&,T*>(m_root);
  }

  template<typename T>
  unsigned mafTree<T>::children(const TYPENAME mafTree<T>::const_iterator& i) const
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return i.node()->m_children.size();
  }

  template<typename T>
  unsigned mafTree<T>::children(const mafTreeIterator<T,T&,T*>& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    return i.node()->m_children.size();
  }

  template<typename T>
  TYPENAME mafTree<T>::const_iterator mafTree<T>::child(const TYPENAME mafTree<T>::const_iterator& i, unsigned child) const
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    i.assert_valid(this);
    if (child >= children(i)) throw std::out_of_range("mafCore::mafTree");
    return mafTreeIterator<T,const T&,const T*>(i.node()->m_children[child]);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::child(const TYPENAME mafTree<T>::iterator& i, unsigned child)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    i.assert_valid(this);
    if (child >= children(i)) throw std::out_of_range("mafCore::mafTree");
    return mafTreeIterator<T,T&,T*>(i.node()->m_children[child]);
  }

  template<typename T>
  TYPENAME mafTree<T>::const_iterator mafTree<T>::parent(const TYPENAME mafTree<T>::const_iterator& i) const
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    mafTreeNode<T>* parent = i.node()->m_parent;
    if (!parent) return mafTreeIterator<T,const T&,const T*>(this);
    return mafTreeIterator<T,const T&,const T*>(parent);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::parent(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    i.assert_valid(this);
    mafTreeNode<T>* parent = i.node()->m_parent;
    if (!parent) return mafTreeIterator<T,T&,T*>(this);
    return mafTreeIterator<T,T&,T*>(parent);
  }

  template<typename T>
  TYPENAME mafTree<T>::const_prefix_iterator mafTree<T>::prefix_begin(void) const
  {
    return mafTreePrefixIterator<T,const T&,const T*>(root());
  }

  template<typename T>
  TYPENAME mafTree<T>::prefix_iterator mafTree<T>::prefix_begin(void)
  {
    return mafTreePrefixIterator<T,T&,T*>(root());
  }

  template<typename T>
  TYPENAME mafTree<T>::const_prefix_iterator mafTree<T>::prefix_end(void) const
  {
    return mafTreePrefixIterator<T,const T&,const T*>(mafTreeIterator<T,const T&,const T*>(this));
  }

  template<typename T>
  TYPENAME mafTree<T>::prefix_iterator mafTree<T>::prefix_end(void)
  {
    return mafTreePrefixIterator<T,T&,T*>(mafTreeIterator<T,T&,T*>(this));
  }

  template<typename T>
  TYPENAME mafTree<T>::const_postfix_iterator mafTree<T>::postfix_begin(void) const
  {
    return mafTreePostfixIterator<T,const T&,const T*>(root());
  }

  template<typename T>
  TYPENAME mafTree<T>::postfix_iterator mafTree<T>::postfix_begin(void)
  {
    return mafTreePostfixIterator<T,T&,T*>(root());
  }

  template<typename T>
  TYPENAME mafTree<T>::const_postfix_iterator mafTree<T>::postfix_end(void) const
  {
    return mafTreePostfixIterator<T,const T&,const T*>(mafTreeIterator<T,const T&,const T*>(this));
  }

  template<typename T>
  TYPENAME mafTree<T>::postfix_iterator mafTree<T>::postfix_end(void)
  {
    return mafTreePostfixIterator<T,T&,T*>(mafTreeIterator<T,T&,T*>(this));
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::insert(const T& data)
  {
    // insert a new node as the root
    return insert(mafTreeIterator<T,T&,T*>(this), 0, data);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::insert(const TYPENAME mafTree<T>::iterator& i, unsigned offset, const T& data)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    // if i is the end iterator, this means insert a new root
    if (i.end())
      erase();
    else
    {
      i.assert_valid(this);
      if (offset > children(i)) throw std::out_of_range("mafCore::mafTree");
    }
    mafTreeNode<T>* new_node = new mafTreeNode<T>(this,data);
    if (i.end())
    {
      m_root = new_node;
    }
    else
    {
      i.node()->m_children.insert(i.node()->m_children.begin()+offset,new_node);
      new_node->m_parent = i.node();
    }
    return mafTreeIterator<T,T&,T*>(new_node);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::append(const TYPENAME mafTree<T>::iterator& i, const T& data)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    return insert(i, i.node()->m_children.size(), data);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::insert(const TYPENAME mafTree<T>::iterator& i, unsigned offset, const mafTree<T>& tree)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    // insert a whole tree as a child of i
    i.assert_valid(this);
    if (offset > children(i)) throw std::out_of_range("mafCore::mafTree");
    mafTreeNode<T>* new_node = mafTree_copy(this, tree.m_root);
    i.node()->m_children.insert(i.node()->m_children.begin()+offset,new_node);
    new_node->m_parent = i.node();
    return mafTreeIterator<T,T&,T*>(new_node);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::append(const TYPENAME mafTree<T>::iterator& i, const mafTree<T>& tree)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    return insert(i, children(i), tree);
  }

  template<typename T>
  TYPENAME mafTree<T>::iterator mafTree<T>::push(const TYPENAME mafTree<T>::iterator& node, const T& data)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    // insert a new node to replace the existing node in the tree
    // making the original node the child of the new node
    // i.e. (node) becomes (new)->(node)
    // afterwards, the iterator still points to the old node, now the child
    // returns the iterator to the new node
    node.assert_valid(this);
    mafTreeNode<T>* new_node = new mafTreeNode<T>(this,data);
    if (node.node() == m_root)
    {
      // pushing the root node
      m_root = new_node;
      new_node->m_parent = 0;
    }
    else
    {
      // pushing a sub-node
      *(std::find(node.node()->m_parent->m_children.begin(), node.node()->m_parent->m_children.end(), node.node())) = new_node;
      new_node->m_parent = node.node()->m_parent;
    }
    // link up the old node as the child of the new node
    new_node->m_children.insert(new_node->m_children.begin(),node.node());
    node.node()->m_parent = new_node;
    return mafTreeIterator<T,T&,T*>(new_node);
  }

  template<typename T>
  void mafTree<T>::pop(const TYPENAME mafTree<T>::iterator& parent, unsigned offset)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    // inverse of push
    // removes the specified child of the parent node, adding its children to the parent node at the same offset
    parent.assert_valid(this);
    mafTreeNode<T>* node = parent.node();
    if (offset >= node->m_children.size()) throw std::out_of_range("mafCore::mafTree");
    // move the grandchildren first
    mafTreeNode<T>* child = parent.node()->m_children[offset];
    while (!child->m_children.empty())
    {
      // remove the last grandchild and insert into node just after the child to be removed
      mafTreeNode<T>* grandchild = child->m_children[child->m_children.size()-1];
      child->m_children.pop_back();
      node->m_children.insert(node->m_children.begin()+offset+1, grandchild);
      grandchild->m_parent = node;
    }
    // now remove the child
    node->m_children.erase(node->m_children.begin()+offset);
    delete child;
  }

  template<typename T>
  void mafTree<T>::erase(void)
  {
    // erase the whole tree
    erase(root());
  }

  template<typename T>
  void mafTree<T>::erase(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    if (!i.end())
    {
      // erase this node and its subtree
      // do this by erasing this child of its parent
      // handle the case of erasing the root
      i.assert_valid(this);
      mafTreeNode<T>* node = i.node();
      if (node == m_root)
      {
        delete m_root;
        m_root = 0;
      }
      else
      {
        mafTreeNode<T>* parent = node->m_parent;
        // impossible for parent to be null - should assert this
        TYPENAME std::vector<mafTreeNode<T>*>::iterator found =
          std::find(parent->m_children.begin(), parent->m_children.end(), node);
        // impossible for find to fail - should assert this
        parent->m_children.erase(found);
        delete node;
      }
    }
  }

  template<typename T>
  void mafTree<T>::erase(const TYPENAME mafTree<T>::iterator& i, unsigned offset)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    erase(child(i, offset));
  }

  template<typename T>
  mafTree<T> mafTree<T>::subtree(void)
  {
    return subtree(root());
  }

  template<typename T>
  mafTree<T> mafTree<T>::subtree(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    mafTree<T> result;
    if (!i.end())
    {
      i.assert_valid(this);
      result.m_root = mafTree_copy(&result, i.node());
    }
    return result;
  }

  template<typename T>
  mafTree<T> mafTree<T>::subtree(const TYPENAME mafTree<T>::iterator& i, unsigned offset)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    return subtree(child(i, offset));
  }

  template<typename T>
  mafTree<T> mafTree<T>::cut(void)
  {
    return cut(root());
  }

  template<typename T>
  mafTree<T> mafTree<T>::cut(const TYPENAME mafTree<T>::iterator& i)
    throw(mafWrongObject,mafNullDereference,mafEndDereference)
  {
    mafTree<T> result;
    if (!i.end())
    {
      i.assert_valid(this);
      mafTreeNode<T>* node = i.node();
      if (node == m_root)
      {
        result.m_root = m_root;
        m_root = 0;
      }
      else
      {
        mafTreeNode<T>* parent = node->m_parent;
        // impossible for parent to be null - should assert this
        TYPENAME std::vector<mafTreeNode<T>*>::iterator found =
          std::find(parent->m_children.begin(), parent->m_children.end(), node);
        // impossible for find to fail - should assert this
        result.m_root = *found;
        parent->m_children.erase(found);
      }
      if (result.m_root)
      {
        result.m_root->m_parent = 0;
        result.m_root->change_owner(&result);
      }
    }
    return result;
  }

  template<typename T>
  mafTree<T> mafTree<T>::cut(const TYPENAME mafTree<T>::iterator& i, unsigned offset)
    throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range)
  {
    return cut(child(i, offset));
  }

  ////////////////////////////////////////////////////////////////////////////////

} // end namespace mafCore

