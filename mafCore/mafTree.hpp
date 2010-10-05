#ifndef MAFTREE_H
#define MAFTREE_H
////////////////////////////////////////////////////////////////////////////////

//   Author:    Andy Rushton
//   Copyright: (c) Southampton University 1999-2004
//              (c) Andy Rushton           2004-2009
//   License:   BSD License, see ../docs/license.html

//   A templated n-ary tree data structure. STL-like but the definition of
//   iterators is really only applicable to one-dimensional structures. I use
//   iterators to access tree nodes, but there is no increment or decrement
//   operators for them. I also define prefix and postfix traversal iterators
//   which do have increment.

////////////////////////////////////////////////////////////////////////////////
#include "mafCore_global.h"
#include "mafContainersFixes.hpp"
#include "mafExceptions.hpp"
#include "mafSafeIterator.hpp"

namespace mafCore
{

  ////////////////////////////////////////////////////////////////////////////////
  // Internals

  template<typename T> class mafTreeNode;
  template<typename T> class mafTree;
  template<typename T, typename TRef, typename TPtr> class mafTreeIterator;
  template<typename T, typename TRef, typename TPtr> class mafTreePrefixIterator;
  template<typename T, typename TRef, typename TPtr> class mafTreePostfixIterator;

  ////////////////////////////////////////////////////////////////////////////////
  // Iterators

  // Simple iterators which are just used as pointers to tree nodes. These have
  // no increment or decrement operations defined. An uninitialised iterator is
  // null - similarly, if you ask for the root of an empty tree or the parent of
  // the root node then you get a null iterator.

  template<typename T, typename TRef, typename TPtr>
  class mafTreeIterator : public mafSafeIterator<mafTree<T>,mafTreeNode<T> >
  {
  public:
    // local type definitions
    // an iterator points to an object whilst a const_iterator points to a const object
    typedef mafTreeIterator<T,T&,T*> iterator;
    typedef mafTreeIterator<T,const T&,const T*> const_iterator;
    typedef mafTreeIterator<T,TRef,TPtr> this_iterator;
    typedef TRef reference;
    typedef TPtr pointer;

    // constructor to create a null iterator - you must assign a valid value to this iterator before using it
    mafTreeIterator(void);
    ~mafTreeIterator(void);

    // Type conversion methods allow const_iterator and iterator to be converted
    const_iterator constify(void) const;
    iterator deconstify(void) const;

    // tests useful for putting iterators into other STL structures and for testing whether iteration has completed
    bool operator == (const this_iterator& r) const;
    bool operator != (const this_iterator& r) const;
    bool operator < (const this_iterator& r) const;

    // access the node data - a const_iterator gives you a const element, an iterator a non-const element
    // it is illegal to dereference an invalid (i.e. null or end) iterator
    reference operator*(void) const
      throw(mafNullDereference,mafEndDereference);
    pointer operator->(void) const
      throw(mafNullDereference,mafEndDereference);

    friend class mafTree<T>;
    friend class mafTreePrefixIterator<T,TRef,TPtr>;
    friend class mafTreePostfixIterator<T,TRef,TPtr>;

  public:
    // Note: I had to make this public to get round a compiler problem - it should be private
    // you cannot create a valid iterator except by calling an mafTree method that returns one
    // constructor used by mafTree to create a non-null iterator
    explicit mafTreeIterator(mafTreeNode<T>* node);
    // constructor used by mafTree to create an end iterator
    explicit mafTreeIterator(const mafTree<T>* owner);
    // used to create an alias of an iterator
    explicit mafTreeIterator(const mafSafeIterator<mafTree<T>, mafTreeNode<T> >& iterator);
  };

  // Traversal iterators are like iterators but they have increment operators (++)
  // - prefix_iterator visits the nodes of the tree in prefix order
  // - postfix_iterator visits the nodes of the tree in postfix order.
  // There is no such thing as infix order for an n-ary tree and you cannot
  // traverse backwards with these iterators. These follow the STL convention in
  // that you iterate from a begin to an end - in this case mafTree exports
  // prefix_begin()/prefix_end() and postfix_begin()/postfix_end(). You can
  // simplify these iterators to the basic iterator above for functions that
  // require a simple iterator.

  template<typename T, typename TRef, typename TPtr>
  class mafTreePrefixIterator
  {
  public:
    typedef mafTreePrefixIterator<T,T&,T*>             iterator;
    typedef mafTreePrefixIterator<T,const T&,const T*> const_iterator;
    typedef mafTreePrefixIterator<T,TRef,TPtr>         this_iterator;
    typedef mafTreeIterator<T,TRef,TPtr>                simple_iterator;
    typedef TRef                                       reference;
    typedef TPtr                                       pointer;

    // constructor to create a null iterator - you must assign a valid value to this iterator before using it
    mafTreePrefixIterator(void);
    ~mafTreePrefixIterator(void);

    // tests
    // a null iterator is one that has not been initialised with a value yet
    // i.e. you just declared it but didn't assign to it
    bool null(void) const;
    // an end iterator is one that points to the end element of the list of nodes
    // in STL conventions this is one past the last valid element and must not be dereferenced
    bool end(void) const;
    // a valid iterator is one that can be dereferenced
    // i.e. non-null and non-end
    bool valid(void) const;

    // Type conversion methods allow const_iterator and iterator to be converted
    // convert an iterator/const_iterator to a const_iterator
    const_iterator constify(void) const;
    iterator deconstify(void) const;

    // generate a simple iterator from a traversal iterator
    mafTreeIterator<T,TRef,TPtr> simplify(void) const;

    // tests useful for putting iterators into other STL structures and for testing whether iteration has completed
    bool operator == (const this_iterator& r) const;
    bool operator != (const this_iterator& r) const;
    bool operator < (const this_iterator& r) const;

    // increment/decrement operators used to step through the set of all nodes in a graph
    // it is only legal to increment a valid iterator
    // pre-increment
    this_iterator& operator ++ (void)
      throw(mafNullDereference,mafEndDereference);
    // post-increment
    this_iterator operator ++ (int)
      throw(mafNullDereference,mafEndDereference);

    // access the node data - a const_iterator gives you a const element, an iterator a non-const element
    // it is illegal to dereference an invalid (i.e. null or end) iterator
    reference operator*(void) const
      throw(mafNullDereference,mafEndDereference);
    pointer operator->(void) const
      throw(mafNullDereference,mafEndDereference);

    friend class mafTree<T>;
    friend class mafTreeIterator<T,TRef,TPtr>;

  private:
    mafTreeIterator<T,TRef,TPtr> m_iterator;

    explicit mafTreePrefixIterator(const mafTreeIterator<T,TRef,TPtr>& i);
    const mafTreeIterator<T,TRef,TPtr>& get_iterator(void) const;
    mafTreeIterator<T,TRef,TPtr>& get_iterator(void);
  };

  ////////////////////////////////////////////////////////////////////////////////

  template<typename T, typename TRef, typename TPtr>
  class mafTreePostfixIterator
  {
  public:
    typedef mafTreePostfixIterator<T,T&,T*>             iterator;
    typedef mafTreePostfixIterator<T,const T&,const T*> const_iterator;
    typedef mafTreePostfixIterator<T,TRef,TPtr>         this_iterator;
    typedef mafTreeIterator<T,TRef,TPtr>                 simple_iterator;
    typedef TRef                                        reference;
    typedef TPtr                                        pointer;

    // constructor to create a null iterator - you must assign a valid value to this iterator before using it
    mafTreePostfixIterator(void);
    ~mafTreePostfixIterator(void);

    // tests
    // a null iterator is one that has not been initialised with a value yet
    // i.e. you just declared it but didn't assign to it
    bool null(void) const;
    // an end iterator is one that points to the end element of the list of nodes
    // in STL conventions this is one past the last valid element and must not be dereferenced
    bool end(void) const;
    // a valid iterator is one that can be dereferenced
    // i.e. non-null and non-end
    bool valid(void) const;

    // Type conversion methods allow const_iterator and iterator to be converted
    // convert an iterator/const_iterator to a const_iterator
    const_iterator constify(void) const;
    iterator deconstify(void) const;

    // generate a simple iterator from a traversal iterator
    mafTreeIterator<T,TRef,TPtr> simplify(void) const;

    // tests useful for putting iterators into other STL structures and for testing whether iteration has completed
    bool operator == (const this_iterator& r) const;
    bool operator != (const this_iterator& r) const;
    bool operator < (const this_iterator& r) const;

    // increment/decrement operators used to step through the set of all nodes in a graph
    // it is only legal to increment a valid iterator
    // pre-increment
    this_iterator& operator ++ (void)
      throw(mafNullDereference,mafEndDereference);
    // post-increment
    this_iterator operator ++ (int)
      throw(mafNullDereference,mafEndDereference);

    // access the node data - a const_iterator gives you a const element, an iterator a non-const element
    // it is illegal to dereference an invalid (i.e. null or end) iterator
    reference operator*(void) const
      throw(mafNullDereference,mafEndDereference);
    pointer operator->(void) const
      throw(mafNullDereference,mafEndDereference);

    friend class mafTree<T>;
    friend class mafTreeIterator<T,TRef,TPtr>;

  private:
    mafTreeIterator<T,TRef,TPtr> m_iterator;

    explicit mafTreePostfixIterator(const mafTreeIterator<T,TRef,TPtr>& i);
    const mafTreeIterator<T,TRef,TPtr>& get_iterator(void) const;
    mafTreeIterator<T,TRef,TPtr>& get_iterator(void);
  };

  ////////////////////////////////////////////////////////////////////////////////
  // The mafTree class
  ////////////////////////////////////////////////////////////////////////////////

  template<typename T>
  class mafTree
  {
  public:
    // STL-like typedefs for the types and iterators
    typedef T value_type;

    typedef mafTreeIterator<T,T&,T*> iterator;
    typedef mafTreeIterator<T,const T&,const T*> const_iterator;

    typedef mafTreePrefixIterator<T,T&,T*> prefix_iterator;
    typedef mafTreePrefixIterator<T,const T&,const T*> const_prefix_iterator;

    typedef mafTreePostfixIterator<T,T&,T*> postfix_iterator;
    typedef mafTreePostfixIterator<T,const T&,const T*> const_postfix_iterator;

    //////////////////////////////////////////////////////////////////////////////
    // Constructors, destructors and copies

    mafTree(void);
    ~mafTree(void);

    // copy constructor and assignment both copy the tree
    mafTree(const mafTree<T>&);
    mafTree<T>& operator=(const mafTree<T>&);

    //////////////////////////////////////////////////////////////////////////////
    // size tests

    // tests on whole tree
    bool empty(void) const;
    unsigned size(void) const;

    // tests for number of nodes in subtree starting at node
    unsigned size(const const_iterator& node) const
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    unsigned size(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    // test for depth of tree from root to node
    unsigned depth(const const_iterator& node) const
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    unsigned depth(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    //////////////////////////////////////////////////////////////////////////////
    // direct traversal

    const_iterator root(void) const;
    iterator root(void);

    unsigned children(const const_iterator& node) const
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    unsigned children(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    const_iterator child(const const_iterator& node, unsigned child) const
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);
    iterator child(const iterator& node, unsigned child)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);

    const_iterator parent(const const_iterator& node) const
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    iterator parent(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    //////////////////////////////////////////////////////////////////////////////
    // iterator traversal

    const_prefix_iterator prefix_begin(void) const;
    prefix_iterator prefix_begin(void);
    const_prefix_iterator prefix_end(void) const;
    prefix_iterator prefix_end(void);

    const_postfix_iterator postfix_begin(void) const;
    postfix_iterator postfix_begin(void);
    const_postfix_iterator postfix_end(void) const;
    postfix_iterator postfix_end(void);

    //////////////////////////////////////////////////////////////////////////////
    // modification

    iterator insert(const T&);

    iterator insert(const iterator& node, unsigned child, const T&)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);
    iterator append(const iterator& node, const T&) 
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    iterator insert(const iterator& node, unsigned child, const mafTree<T>&)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);
    iterator append(const iterator& node, const mafTree<T>&)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    iterator push(const iterator& node, const T&) 
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    void pop(const iterator& node, unsigned child) 
      throw(mafWrongObject,mafNullDereference,mafEndDereference);

    void erase(void);
    void erase(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    void erase(const iterator& node, unsigned child)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);

    mafTree<T> subtree(void);
    mafTree<T> subtree(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    mafTree<T> subtree(const iterator& node, unsigned child)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);

    mafTree<T> cut(void);
    mafTree<T> cut(const iterator& node)
      throw(mafWrongObject,mafNullDereference,mafEndDereference);
    mafTree<T> cut(const iterator& node, unsigned child)
      throw(mafWrongObject,mafNullDereference,mafEndDereference,std::out_of_range);

    //////////////////////////////////////////////////////////////////////////////

  private:
    mafTreeNode<T>* m_root;
  };

  ////////////////////////////////////////////////////////////////////////////////

} // end namespace mafCore

#include "mafTree.tpp"
#endif
