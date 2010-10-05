#ifndef SAFE_ITERATOR_H
#define SAFE_ITERATOR_H
////////////////////////////////////////////////////////////////////////////////

//   Author:    Andy Rushton
//   Copyright: (c) Southampton University 1999-2004
//              (c) Andy Rushton           2004-2009
//   License:   BSD License, see ../docs/license.html

//   The STLplus safe_iterator superclasses. This implements the STLplus safe
//   iterator principles. Data structures can then be built using subclasses
//   of safe_iterator for their iterator objects and they will inherit the
//   safe iterator behaviour.

//   The data structure must contain a master iterator for each node in the
//   structure. When an iterator is returned to the user, it must be created
//   by the master iterator. When a node is removed from the data structure,
//   its master iterator is destroyed. This sets all iterators pointing to the
//   master iterator to end iterators.

////////////////////////////////////////////////////////////////////////////////
#include "mafContainersFixes.hpp"
#include "mafExceptions.hpp"

namespace mafCore
{

  ////////////////////////////////////////////////////////////////////////////////
  // internals

  template<typename O, typename N>
  class mafSafeIterator_body;

  template<typename O, typename N>
  class mafSafeIterator;

  ////////////////////////////////////////////////////////////////////////////////
  // Master Iterator
  // Create one of these in each node in the data structure
  // Generate iterators by obtaining a safe-iterator object from the master iterator
  ////////////////////////////////////////////////////////////////////////////////

  template<typename O, typename N>
  class master_iterator
  {
  public:

    // construct a valid master iterator connected to the node
    master_iterator(const O* owner, N* node) throw();

    // destructor - disconnects all iterators from the node
    ~master_iterator(void) throw();

    // dereference
    N* node(void) const throw();
    const O* owner(void) const throw();

    // when you move a node from one owner to another, call this on the node's master iterator
    // this effectively moves all other iterators to the node so that they are owned by the new owner too
    void change_owner(const O* owner) throw();

    friend class mafSafeIterator<O,N>;
  private:
    master_iterator(const master_iterator&) throw();
    master_iterator& operator=(const master_iterator&) throw();
    mafSafeIterator_body<O,N>* m_body;
  };

  ////////////////////////////////////////////////////////////////////////////////
  // Safe Iterator
  ////////////////////////////////////////////////////////////////////////////////

  template<typename O, typename N>
  class mafSafeIterator
  {
  public:

    // construct a null iterator
    mafSafeIterator(void) throw();

    // construct a valid iterator by aliasing from the owner node's master iterator
    mafSafeIterator(const master_iterator<O,N>&) throw();

    // copy constructor does aliasing
    mafSafeIterator(const mafSafeIterator<O,N>&) throw();

    // alias an iterator by assignment
    mafSafeIterator<O,N>& operator=(const mafSafeIterator<O,N>&) throw();

    // destructor
    ~mafSafeIterator(void) throw();

    // reassignment to another node used in increment/decrement operation
    void set(const master_iterator<O,N>&) throw();

    // dereference
    N* node(void) const throw();
    const O* owner(void) const throw();

    // change to a null iterator - i.e. one that does not belong to any object
    // this does not affect any other iterators pointing to the same node
    void set_null(void) throw();

    ////////////////////////////////////////////////////////////////////////////////
    // operations for clients that do not have a master end iterator
    // alternatively, have a master end iterator as part of the container
    // and call constructor(master_end) or set(master_end)

    // construct an end iterator
    mafSafeIterator(const O* owner) throw();

    // change to an end iterator - e.g. as a result of incrementing off the end
    void set_end(void) throw();

    ////////////////////////////////////////////////////////////////////////////////
    // tests

    // comparison
    bool equal(const mafSafeIterator<O,N>& right) const throw();
    int compare(const mafSafeIterator<O,N>& right) const throw();

    // a null iterator is one that has not been initialised with a value yet
    // i.e. you just declared it but didn't assign to it
    bool null(void) const throw();

    // an end iterator is one that points to the end element of the list of nodes
    // in STL conventions this is one past the last valid element and must not be dereferenced
    bool end(void) const throw();

    // a valid iterator is one that can be dereferenced
    // i.e. non-null and non-end
    bool valid(void) const throw();

    // check the rules for a valid iterator that can be dereferenced
    // optionally also check that the iterator is owned by the owner
    void assert_valid(void) const throw(mafNullDereference,mafEndDereference);
    void assert_valid(const O* owner) const throw(mafWrongObject,mafNullDereference,mafEndDereference);
    // assert the rules for a non-null iterator - i.e. valid or end, values that occur in increment operations
    void assert_non_null(void) const throw(mafNullDereference);
    // assert that this iterator is owned by this container
    void assert_owner(const O* owner) const throw(mafWrongObject);

    ////////////////////////////////////////////////////////////////////////////////

    friend class master_iterator<O,N>;
  private:
    mafSafeIterator_body<O,N>* m_body;
  };

  ////////////////////////////////////////////////////////////////////////////////

} // end namespace mafCore

#include "mafSafeIterator.tpp"
#endif
