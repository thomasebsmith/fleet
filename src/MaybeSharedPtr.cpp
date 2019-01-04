// File: src/MaybeSharedPtr.cpp
// Purpose: Source file for MaybeSharedPtrs, which are pointers that can either
//  be shared or raw. If shared, this class is simply a wrapper around
//  std::shared_ptr. If raw, the object associated with the pointer will *not*
//  be destroyed when this class is destroyed.

#include <memory>
#include <variant>
#include "MaybeSharedPtr.hpp"
