// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef QRB_ROS_SYSTEM_MONITOR__FILESYSTEM_INFO_HPP_
#define QRB_ROS_SYSTEM_MONITOR__FILESYSTEM_INFO_HPP_

#include <mntent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>

#include <cstdint>
#include <set>
#include <string>
#include <vector>

namespace qrb_ros_system_monitor
{

struct FilesystemInfo
{
  std::string fs;
  std::string type;
  std::string mount;
  uint32_t total = 0;  // MB
  uint32_t used = 0;   // MB
  uint32_t avail = 0;  // MB
  uint32_t use_rate = 0;
};

inline bool is_network_filesystem(const std::string & type)
{
  static const std::set<std::string> network_types = {
      "nfs", "nfs4", "cifs", "smbfs", "smb3", "afs", "ncpfs"};
  return network_types.count(type) > 0;
}

// Round up to MB, the same way as "df -BM"
inline uint32_t bytes_to_mb(uint64_t bytes)
{
  return static_cast<uint32_t>((bytes + (1024 * 1024 - 1)) / (1024 * 1024));
}

// List mounted filesystems with the sizes in MB.
// Pseudo filesystems (no blocks), devtmpfs (hidden by df as well) and mounts of an
// already listed device are skipped.
inline std::vector<FilesystemInfo> get_filesystem_list()
{
  std::vector<FilesystemInfo> list;

  FILE * mounts = setmntent("/proc/self/mounts", "r");
  if (mounts == nullptr) {
    mounts = setmntent("/proc/mounts", "r");
  }
  if (mounts == nullptr) {
    return list;
  }

  std::set<dev_t> seen_devices;
  struct mntent * entry;
  while ((entry = getmntent(mounts)) != nullptr) {
    if (std::string(entry->mnt_type) == "devtmpfs") {
      continue;
    }

    struct statvfs vfs;
    if (statvfs(entry->mnt_dir, &vfs) != 0 || vfs.f_blocks == 0) {
      continue;
    }

    struct stat st;
    if (stat(entry->mnt_dir, &st) == 0 && !seen_devices.insert(st.st_dev).second) {
      continue;
    }

    uint64_t block_size = vfs.f_frsize != 0 ? vfs.f_frsize : vfs.f_bsize;
    uint64_t total = vfs.f_blocks * block_size;
    uint64_t used = (vfs.f_blocks - vfs.f_bfree) * block_size;
    uint64_t avail = vfs.f_bavail * block_size;

    FilesystemInfo info;
    info.fs = entry->mnt_fsname;
    info.type = entry->mnt_type;
    info.mount = entry->mnt_dir;
    info.total = bytes_to_mb(total);
    info.used = bytes_to_mb(used);
    info.avail = bytes_to_mb(avail);

    // used / (used + avail), rounded up, the same way as df
    uint64_t non_reserved = used + avail;
    info.use_rate = non_reserved == 0 ?
                        0 :
                        static_cast<uint32_t>((used * 100 + non_reserved - 1) / non_reserved);
    list.emplace_back(std::move(info));
  }
  endmntent(mounts);

  return list;
}

}  // namespace qrb_ros_system_monitor

#endif  // QRB_ROS_SYSTEM_MONITOR__FILESYSTEM_INFO_HPP_
