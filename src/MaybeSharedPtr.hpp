// File: src/MaybeSharedPtr.hpp
// Purpose: Header file for MaybeSharedPtrs, which are pointers that can either
//  be shared or raw. If shared, this class is simply a wrapper around
//  std::shared_ptr. If raw, the object associated with the pointer will *not*
//  be destroyed when this class is destroyed.

#ifndef MAYBESHAREDPTR_HPP
#define MAYBESHAREDPTR_HPP

#include <memory>
#include <variant>

template <typename T>
class MaybeSharedPtr {
private:
  typedef std::variant<std::shared_ptr<T>, T*> InternalVariant;
  InternalVariant internalPtr;
public:
  // Constructor(isRawPtr) - Creates an empty MaybeSharedPtr that is raw iff
  //  isRawPtr is true.
  MaybeSharedPtr(bool isRawPtr = false): internalPtr {
    isRawPtr ? InternalVariant { nullptr } : InternalVariant {
      std::shared_ptr<T> {}
    }
  } {}

  // Constructor(ptr, isRawPtr) - Creates a MaybeSharedPtr that is raw iff
  //  isRawPtr is true.
  MaybeSharedPtr(T* ptr, bool isRawPtr = false): internalPtr {
    isRawPtr ? InternalVariant { ptr } : InternalVariant {
      std::shared_ptr<T> { ptr }
    }
  } {}

  T& operator* () const {
    if (std::holds_alternative<T*>(internalPtr)) {
      return **std::get_if<T*>(&internalPtr);
    }
    return **std::get_if<std::shared_ptr<T>>(&internalPtr);
  }
  T* operator-> () const {
    if (std::holds_alternative<T*>(internalPtr)) {
      return *std::get_if<T*>(&internalPtr);
    }
    return std::get_if<std::shared_ptr<T>>(&internalPtr)->get();
  }
  operator bool() const {
    if (std::holds_alternative<T*>(internalPtr)) {
      return *std::get_if<T*>(&internalPtr) != nullptr;
    }
    return !!(*std::get_if<std::shared_ptr<T>>(&internalPtr));
  }
};

#endif
