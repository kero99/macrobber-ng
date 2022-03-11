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
#define VERSION "0.2.1"
#define PATH_LEN	2048

#include <sys/stat.h>
#include <locale.h>
#include <dirent.h>

#include <unistd.h>

#include "ifError.h"
#include "utils.h"


static void
printStatx(const struct statx *sbx, const char *pathfile, const int hashfiles)
{
   __s32 fmode = sbx->stx_mode;
	unsigned long flags = 0UL;

   char *hash = "";
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


   if ( sbx->stx_size == 0)  {
      hash = "d41d8cd98f00b204e9800998ecf8427e";
   }

   if (( filetype == "d") || (hashfiles == 0)) {
      hash = "0";
   }

   if ( (hashfiles == 1) && (filetype == "-") && (sbx->stx_size >= 0) ) {
      calculate_md5sum(pathfile);
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

   free(buf);
}

void listdir(const char *name, const int hashfiles)
{
   DIR *dir;
   struct dirent *entry;
   struct statx sbx;
   char path[PATH_LEN];

   if (!(dir = opendir(name)))
      return;

   while ((entry = readdir(dir)) != NULL) {
      if (entry->d_type == DT_DIR) {
         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
               continue;
         
         snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
         statx(AT_FDCWD , path, AT_SYMLINK_NOFOLLOW, STATX_ALL, &sbx);
         printStatx(&sbx, path, hashfiles);
         listdir(path, hashfiles);
      } else {
         snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
         statx(AT_FDCWD , path, AT_SYMLINK_NOFOLLOW, STATX_ALL, &sbx);
         printStatx(&sbx, path, hashfiles);
      }
   }
   closedir(dir);
}

static void help(const char *argv0)
{
   printf("macrobber-ng v%s \n", VERSION);
   printf("USAGE: %s <-5> <-v> <-h> [DIRECTORY] \n", argv0);
   printf("\t -5 do MD5 calculation (disabled by default)\n");
   printf("\t -v Show version\n");
   printf("\t -h This help\n");
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   int opt;
   int hashfiles = 0;
   char *processpath;

   setlocale(LC_ALL, "");

   // check to see if user entered a directory name */
   //if (argc < 2)
   //{
   //         printf("Usage: %s <directory>\n", argv[0]);
   //         return 0;
   //}

   while ((opt = getopt(argc, argv, "vh::5")) != -1)
   {
      switch (opt)
      {
      case '5':
         hashfiles = 1;
         break;
      case 'v':
         printf("macrobber-ng v%s \n", VERSION);
         exit(EXIT_SUCCESS);
      case 'h':
         help(argv[0]);
         break;
      default:
         help(argv[0]);
      }
   }


   if (argc - optind < 1)
   {
      fprintf(stderr, "%s: too few arguments\n", argv[0]);
      help(argv[0]);
   }

   processpath = argv[optind++];
   
   listdir(processpath, hashfiles);

	exit(EXIT_SUCCESS);
}
