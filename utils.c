/* SPDX-License-Identifier: GPL-3.0 */
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "ifError.h"
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include <stdio.h>

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

const char *calculate_md5sum(const char *filename)
{
    uint8_t *result;
    FILE *in_file = fopen(filename, "r");

    result = md5File(in_file);

	for(unsigned int i = 0; i < 16; ++i){
		printf("%02x", result[i]);
	}
	free(result);
    fclose (in_file);
}

const char *calculate_sha1sum(const char *filename)
{
    uint8_t *result;
    FILE *in_file = fopen(filename, "r");

    result = sha1File(in_file);

    for(unsigned int i = 0; i < 20; ++i){
        printf("%02x", result[i]);
    }
    free(result);
    fclose (in_file);
}

const char *calculate_sha256sum(const char *filename)
{
    uint8_t *result;
    FILE *in_file = fopen(filename, "r");

    result = sha256File(in_file);

    for(unsigned int i = 0; i < 32; ++i){
        printf("%02x", result[i]);
    }
    free(result);
    fclose (in_file);
}