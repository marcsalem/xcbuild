/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree. An additional grant
 of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef __builtin_productPackagingUtility_Driver_h
#define __builtin_productPackagingUtility_Driver_h

#include <builtin/Driver.h>

namespace builtin {
namespace productPackagingUtility {

class Driver : public builtin::Driver {
public:
    Driver();
    ~Driver();

public:
    virtual std::string name();

public:
    virtual int run(std::vector<std::string> const &args, std::unordered_map<std::string, std::string> const &environment, libutil::Filesystem *filesystem, std::string const &workingDirectory);
};

}
}

#endif // !__builtin_productPackagingUtility_Driver_h
