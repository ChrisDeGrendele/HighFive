/*
 *  Copyright (c), 2017-2018, Adrien Devresse <adrien.devresse@epfl.ch>
 *                            Juan Hernando <juan.hernando@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5FILEDRIVER_MISC_HPP
#define H5FILEDRIVER_MISC_HPP

#include <H5Ppublic.h>

#ifdef H5_HAVE_PARALLEL
#include <H5FDmpi.h>
#endif

namespace HighFive {

namespace {

template <typename Comm, typename Info>
class MPIOFileAccess
{
public:
  MPIOFileAccess(Comm comm, Info info)
      : _comm(comm)
      , _info(info)
  {}

  void apply(const hid_t list) const {
    if (H5Pset_fapl_mpio(list, _comm, _info) < 0) {
        HDF5ErrMapper::ToException<FileException>(
            "Unable to set-up MPIO Driver configuration");
    }
  }
private:
  Comm _comm;
  Info _info;
};

}  //namespace

template <typename Comm, typename Info>
inline MPIOFileDriver::MPIOFileDriver(Comm comm, Info info) {
    add(MPIOFileAccess<Comm, Info>(comm, info));
}

inline void FileVersionBounds::apply(const hid_t list) const {
    if (H5Pset_libver_bounds(list, _low, _high) < 0) {
        HDF5ErrMapper::ToException<PropertyException>(
            "Error setting file version bounds");
    }
}

inline void MetadataBlockSize::apply(const hid_t list) const {
    if (H5Pset_meta_block_size(list, _size) < 0) {
        HDF5ErrMapper::ToException<PropertyException>(
            "Error setting metadata block size");
    }
}

} // namespace HighFive

#endif // H5FILEDRIVER_MISC_HPP
