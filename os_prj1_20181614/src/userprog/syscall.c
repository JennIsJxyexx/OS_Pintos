#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "userprog/gdt.h"
#include "userprog/pagedir.h"
#include "userprog/tss.h"
#include "filesys/directory.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "threads/flags.h"
#include "threads/init.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include "userprog/process.h"

static void syscall_handler (struct intr_frame *);
void sys_halt(void);
void sys_exit(int status);
tid_t sys_exec(const char *file_name);
int sys_wait(tid_t pid);
void sys_read(int fd, void* buffer, unsigned size);
void sys_write(int fd, void* buffer, unsigned size);

void accessing_user_memory(const void* vaddr) {
    if(!is_user_vaddr(vaddr)) 
        sys_exit(-1);
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{

  void* sPtr = f->esp;

  switch (*(uint32_t*)sPtr) {
      case SYS_HALT: {
                        sys_halt();
                        break;
                     }
      case SYS_EXIT: {
                         accessing_user_memory(sPtr+4);
                         sys_exit(*(int*)(sPtr+4));
                         break;
                     }
      case SYS_EXEC: {
                         accessing_user_memory(sPtr+4);
                         const char* file_name = *(char**)(sPtr+4);
                         f->eax = sys_exec(file_name);
                         break;
                     }
      case SYS_WAIT: {
                         accessing_user_memory(sPtr+4);
                         tid_t pid = (tid_t*)*(uint32_t*)(sPtr+4);
                         f->eax = sys_wait(pid);
                         break;
                     }
      case SYS_CREATE: break;
      case SYS_REMOVE: break;
      case SYS_OPEN: break;
      case SYS_FILESIZE: break;
      case SYS_READ: {
                         accessing_user_memory(sPtr+4);
                         accessing_user_memory(sPtr+8);
                         int fd = *(int*)(sPtr+4);
                         void* buffer = (void*)*(uint32_t*)(sPtr+8);
                         unsigned size = (unsigned)*(uint32_t*)(sPtr+12);
                         sys_read(fd, buffer, size);
                         break;
                     }
      case SYS_WRITE: {
                         accessing_user_memory(sPtr+4);
                         accessing_user_memory(sPtr+8);
                         int fd = (int)*(uint32_t*)(sPtr+4);
                         void* buffer = (void*)*(uint32_t*)(sPtr+8);
                         unsigned size = (unsigned)*(uint32_t*)(sPtr+12);
                         sys_write(fd, buffer, size);
                         break;
                      }
      case SYS_SEEK: break;
      case SYS_TELL: break;
      case SYS_CLOSE: break;
      case SYS_FIBONACCI: {
                              accessing_user_memory(sPtr+4);
                              int n = *(int*)(sPtr+4);
                              f->eax = fibonacci(n);
                              break;
                          }
      case SYS_MAX_OF_FOUR_INT: {
                                    accessing_user_memory(sPtr+4);
                                    accessing_user_memory(sPtr+8);
                                    accessing_user_memory(sPtr+12);
                                    accessing_user_memory(sPtr+16);
                                    int a = *(int*)(sPtr+4);
                                    int b = *(int*)(sPtr+8);
                                    int c = *(int*)(sPtr+12);
                                    int d = *(int*)(sPtr+16);
                                    f->eax = max_of_four_int(a,b,c,d);
                                    break;
                                }
  }

  //thread_exit ();
}

void sys_halt(void) {
    shutdown_power_off();
}

void sys_exit(int status) {
    thread_current()->exit_status = status;
    printf("%s: exit(%d)\n", thread_name (), status);
    thread_exit();
}

tid_t sys_exec(const char *cmd_line) {
    return process_execute(cmd_line);
}

int sys_wait(tid_t pid) {
    return process_wait(pid);
}

void sys_read(int fd, void* buffer, unsigned size) {
    if(fd == 0) { // STDIN
        for(int i = 0; i < size; i++) {
            *(char*)(buffer+i) = input_getc();
        }
    }
}

void sys_write(int fd, void* buffer, unsigned size) {
    if(fd == 1) { // STDOUT
        putbuf(buffer, size);
    }
}

int fibonacci(int n) {
    int a, b, c;
    a = 0; b = c = 1;
    for(int i=0; i<n; i++){
        c = a + b; a = b; b = c;
    }
    return a;
}

int max_of_four_int(int a, int b, int c, int d) {
    int max = a;
    if(max < b) max = b;
    if(max < c) max = c;
    if(max < d) max = d;
    return max;
}
