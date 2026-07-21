// Native APIs
#ifndef BF_NATIVE_H
#define BF_NATIVE_H
#ifdef _WIN32
#include <windows.h>

inline void bf_exit(int code) { ExitProcess((UINT)code); }
#elif defined(__linux__)
#include <sys/stat.h>

typedef int fd_t;
#define BF_STAT_BAD_FD -1
#define BF_STAT_BAD_MMAP ((void *)-1)

void bf_exit(int code);
void *bf_mmap(size_t size, fd_t fd);
void bf_munmap(void *addr, size_t size);
void bf_fstat(fd_t fd, struct stat *buf);
fd_t bf_open(const char *path);
void bf_close(fd_t fd);

int bf_read();                  // 0-255 if successful, -1 if error
int bf_write(unsigned char *c); // 1 if successful

size_t bf_fsize(fd_t fd) {
  struct stat buf;
  bf_fstat(fd, &buf);
  return buf.st_size;
}

#endif
#endif // BF_NATIVE_H
