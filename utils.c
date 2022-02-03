/* SPDX-License-Identifier: GPL-3.0 */
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "ifError.h"

const char *str_perm(unsigned int perm)
{
    static char strperm[sizeof("rwx-rwxrwx")];

    snprintf(strperm, sizeof(strperm), "%c%c%c%c%c%c%c%c%c",
        (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
        (perm & S_IXUSR) ? ((perm & S_ISUID) ? 's' : 'x') :
                           ((perm & S_ISUID) ? 'S' : '-'),
        (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
        (perm & S_IXGRP) ? ((perm & S_ISGID) ? 's' : 'x') :
                           ((perm & S_ISGID) ? 'S' : '-'),
        (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
        (perm & S_IXOTH) ? ((perm & S_ISVTX) ? 't' : 'x') :
                           ((perm & S_ISVTX) ? 'T' : '-'));

    return strperm;
}
