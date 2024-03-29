/* SPDX-License-Identifier: GPL-3.0 */
#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#ifndef STATX_ATTR_HAS_ACL
#define STATX_ATTR_HAS_ACL	0x00000200
#endif
#ifndef STATX_ATTR_VERITY
#define STATX_ATTR_VERITY 	0x00100000
#endif

const char *str_perm(unsigned int perm);
const char *calculate_md5sum(const char *filename);
const char *calculate_sha1sum(const char *filename);
const char *calculate_sha256sum(const char *filename);

#endif
