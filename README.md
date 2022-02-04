# macrobber-ng
Collects MACB times from files and outputs to STDOUT in a format that the mactime program from The Sleuth Kit  reads.
This program uses system calls and therefore will modify the A-Time on directories that are not mounted readonly.

This program is based on the work of Author: Brian Carrier [carrier@sleuthkit.org] but upgraded to support btime/crtime and improve performance over python versions.

---- COMPILE ----
make
make install

---- USE ---
mactime-ng /path/to/process

--- NOTES ---

For extract crtimes from NTFS filesystems you will need to have a kernel 5.15 or higher and mount the drive to be processed with the new ntfs3 driver:

sudo mount -t ntfs3 -o ro,loop [DEVICE OR IMG] [MOUNT POINT]
