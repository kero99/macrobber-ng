/*
 * macrobber-ng.c
 *
 * (c) Author: Miguel Quero (Based in statx work of David Quiroga. thx you bro :*
 * e-mail: motrilwireless@gmail.com
 *
 ***************************************************************
  *
 * The license below covers all files distributed with infofile unless 
 * otherwise noted in the file itself.
 *
 * This program is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, version 3.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <https://www.gnu.org/licenses/>. 
 *
*/

#define _GNU_SOURCE
#include <sys/stat.h>
#include <locale.h>
#include <dirent.h> 

#include "ifError.h"
#include "utils.h"


static void
printStatx(const struct statx *sbx, const char *pathfile)
{
   __s32 fmode = sbx->stx_mode;
	unsigned long flags = 0UL;

   char hash[40] = "0"; // Hardcoded hash to 0 by the moment 
   char *filetype;
   size_t bufsize = 255;
   ssize_t result;
   char* buf = malloc(bufsize);
   char linkedfile[1024] = "";

   if (sbx->stx_mask & STATX_MODE) {
      switch (fmode & S_IFMT) {
         case S_IFDIR:  filetype = "d";                                 break;
         case S_IFCHR:  filetype = "c";                                 break;
         case S_IFBLK:  filetype = "b";                                 break;
         case S_IFLNK:  filetype = "l";                                 break; 
         case S_IFIFO:  filetype = "p";                                 break;
         case S_IFSOCK: filetype = "s";                                 break;
         default: filetype = "-";                                       break;
      }
   }

   if ( filetype == "l" ){
      while ((result = readlink(pathfile, buf, bufsize)) >= bufsize) {
      /* We double the buffer size, so next call should succeed ! */
         bufsize *= 2;
         buf = realloc(buf, bufsize);
      }
      buf[result] = '\0';
      strcpy(linkedfile, " -> ");
      strcat(linkedfile, buf);
   }

   printf("%s|%s|%llu|%s%s%s|%d|%d|%llu|%llu.%llu|%llu.%llu|%llu.%llu|%llu.%llu \n", hash,
                                                                                    pathfile,
                                                                                    (unsigned long long) sbx->stx_ino,
                                                                                    filetype,
                                                                                    str_perm(fmode),
                                                                                    linkedfile,
                                                                                    sbx->stx_uid,
                                                                                    sbx->stx_gid,
                                                                                    sbx->stx_size,
                                                                                    sbx->stx_atime.tv_sec,
                                                                                    sbx->stx_atime.tv_nsec,
                                                                                    sbx->stx_mtime.tv_sec,
                                                                                    sbx->stx_mtime.tv_nsec,
                                                                                    sbx->stx_ctime.tv_sec,
                                                                                    sbx->stx_ctime.tv_nsec,
                                                                                    sbx->stx_btime.tv_sec,
                                                                                    sbx->stx_btime.tv_nsec);

}

void listdir(const char *name)
{
   DIR *dir;
   struct dirent *entry;
   struct statx sbx;

   if (!(dir = opendir(name)))
      return;

   while ((entry = readdir(dir)) != NULL) {
      char path[1024];
      if (entry->d_type == DT_DIR) {
         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
               continue;
         
         snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
         statx(AT_FDCWD , path, AT_SYMLINK_NOFOLLOW, STATX_ALL, &sbx);
         printStatx(&sbx, path);
         listdir(path);
      } else {
         snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
         statx(AT_FDCWD , path, AT_SYMLINK_NOFOLLOW, STATX_ALL, &sbx);
         printStatx(&sbx, path);
      }
   }
   closedir(dir);
}

void main(int argc, char *argv[])
{

   setlocale(LC_ALL, "");

   // check to see if user entered a directory name */
   if (argc < 2)
   {
            printf("Usage: %s <directory>\n", argv[0]);
            return 0;
   }

   listdir(argv[1]);

	exit(EXIT_SUCCESS);
}
