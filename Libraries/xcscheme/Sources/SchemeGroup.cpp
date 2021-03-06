/**
 Copyright (c) 2015-present, Facebook, Inc.
 All rights reserved.

 This source code is licensed under the BSD-style license found in the
 LICENSE file in the root directory of this source tree. An additional grant
 of patent rights can be found in the PATENTS file in the same directory.
 */

#include <xcscheme/SchemeGroup.h>

#include <libutil/Filesystem.h>
#include <libutil/FSUtil.h>
#include <libutil/SysUtil.h>

using xcscheme::SchemeGroup;
using xcscheme::XC::Scheme;
using libutil::Filesystem;
using libutil::FSUtil;
using libutil::SysUtil;

SchemeGroup::
SchemeGroup()
{
}

Scheme::shared_ptr SchemeGroup::
scheme(std::string const &name) const
{
    for (Scheme::shared_ptr const &scheme : _schemes) {
        if (scheme->name() == name) {
            return scheme;
        }
    }

    return nullptr;
}

SchemeGroup::shared_ptr SchemeGroup::
Open(Filesystem const *filesystem, std::string const &basePath, std::string const &path, std::string const &name)
{
    if (path.empty() || basePath.empty()) {
        return nullptr;
    }

    if (!filesystem->isDirectory(basePath) || !filesystem->isDirectory(path)) {
        return nullptr;
    }

    std::string realPath = filesystem->resolvePath(path);
    if (realPath.empty()) {
        return nullptr;
    }

    SchemeGroup::shared_ptr group = std::make_shared <SchemeGroup> ();
    group->_basePath = basePath;
    group->_path = path;
    group->_name = name;

    std::string schemePath;

    schemePath = path + "/xcshareddata/xcschemes";
    filesystem->enumerateDirectory(schemePath, [&](std::string const &filename) -> bool {
        if (FSUtil::GetFileExtension(filename) != "xcscheme") {
            return true;
        }

        std::string name = filename.substr(0, filename.find('.'));
        auto scheme = Scheme::Open(filesystem, name, std::string(), schemePath + "/" + filename);
        if (!scheme) {
            fprintf(stderr, "warning: failed parsing shared scheme '%s'\n", name.c_str());
        } else {
            group->_schemes.push_back(scheme);
        }

        if (!group->_defaultScheme && name == group->name()) {
            group->_defaultScheme = scheme;
        }
        return true;
    });

    std::string userName = SysUtil::GetUserName();
    if (!userName.empty()) {
        schemePath = path + "/xcuserdata/" + userName + ".xcuserdatad/xcschemes";
        filesystem->enumerateDirectory(schemePath, [&](std::string const &filename) -> bool {
            if (FSUtil::GetFileExtension(filename) != "xcscheme") {
                return true;
            }

            std::string name = filename.substr(0, filename.find('.'));
            auto scheme = Scheme::Open(filesystem, name, userName, schemePath + "/" + filename);
            if (!scheme) {
                fprintf(stderr, "warning: failed parsing user scheme '%s'\n", name.c_str());
            } else {
                group->_schemes.push_back(scheme);
            }

            if (!group->_defaultScheme && name == group->name()) {
                group->_defaultScheme = scheme;
            }
            return true;
        });
    }

    if (!group->_schemes.empty() && !group->_defaultScheme) {
        group->_defaultScheme = group->_schemes[0];
    }

    return group;
}
