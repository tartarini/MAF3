#ifndef MAF_EXCEPTIONS
#define MAF_EXCEPTIONS
////////////////////////////////////////////////////////////////////////////////

//   Author: Andy Rushton
//   Copyright: (c) Southampton University 1999-2004
//              (c) Andy Rushton           2004-2009
//   License:   BSD License, see ../docs/license.html

//   The set of general exceptions thrown by STLplus components

////////////////////////////////////////////////////////////////////////////////
#include "mafCore_global.h"
#include "mafContainersFixes.hpp"
#include <stdexcept>
#include <string>

namespace mafCore
{

  ////////////////////////////////////////////////////////////////////////////////
  // Thrown if a pointer or an iterator is dereferenced when it is null

  class MAFCORESHARED_EXPORT mafNullDereference : public std::logic_error
  {
  public:
    mafNullDereference(const std::string& description) throw() :
      std::logic_error(std::string("mafCore::mafNullDereference: ") + description) {}
    ~mafNullDereference(void) throw() {}
  };

  ////////////////////////////////////////////////////////////////////////////////
  // Thrown if an iterator is dereferenced when it is pointing to the end element

  class MAFCORESHARED_EXPORT mafEndDereference : public std::logic_error
  {
  public:
    mafEndDereference(const std::string& description) throw() :
      std::logic_error("mafCore::mafEndDereference: " + description) {}
    ~mafEndDereference(void) throw() {}
  };

  ////////////////////////////////////////////////////////////////////////////////
  // Thrown if an iterator is used with the wrong container. In other words, an
  // iterator is created as a pointer to a sub-object within a container. If
  // that iterator is then used with a different container, this exception is
  // thrown.

  class MAFCORESHARED_EXPORT mafWrongObject : public std::logic_error
  {
  public:
    mafWrongObject(const std::string& description) throw() :
      std::logic_error("mafCore::mafWrongObject: " + description) {}
    ~mafWrongObject(void) throw() {}
  };

  ////////////////////////////////////////////////////////////////////////////////
  // Thrown if an attempt is made to copy an object that is uncopyable

  class MAFCORESHARED_EXPORT mafIllegalCopy : public std::logic_error
  {
  public:
    mafIllegalCopy(const std::string& description) throw() :
      std::logic_error("mafCore::mafIllegalCopy: " + description) {}
    ~mafIllegalCopy(void) throw() {}
  };

  ////////////////////////////////////////////////////////////////////////////////

} // end namespace mafCore

#endif
