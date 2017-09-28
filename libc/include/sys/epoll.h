/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _SYS_EPOLL_H_
#define _SYS_EPOLL_H_

#include <sys/cdefs.h>
#include <sys/types.h>
#include <signal.h> /* For sigset_t. */

#include <linux/eventpoll.h>
/* TODO: https://lkml.org/lkml/2017/2/23/416 has a better fix. */
#undef EPOLLWAKEUP
#undef EPOLLONESHOT
#undef EPOLLET

__BEGIN_DECLS

/* TODO: remove once https://lkml.org/lkml/2017/2/23/417 is upstream. */
#define EPOLLIN          0x00000001
#define EPOLLPRI         0x00000002
#define EPOLLOUT         0x00000004
#define EPOLLERR         0x00000008
#define EPOLLHUP         0x00000010
#define EPOLLRDNORM      0x00000040
#define EPOLLRDBAND      0x00000080
#define EPOLLWRNORM      0x00000100
#define EPOLLWRBAND      0x00000200
#define EPOLLMSG         0x00000400
#define EPOLLRDHUP       0x00002000
#define EPOLLWAKEUP      0x20000000
#define EPOLLONESHOT     0x40000000
#define EPOLLET          0x80000000

typedef union epoll_data {
  void* ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t events;
  epoll_data_t data;
}
#ifdef __x86_64__
__packed
#endif
;

int epoll_create(int __size);
int epoll_create1(int __flags) __INTRODUCED_IN(21);

/*
 * Some third-party code uses the existence of EPOLL_CLOEXEC to detect the
 * availability of epoll_create1. This is not correct, since having up-to-date
 * UAPI headers says nothing about the C library, but for the time being we
 * don't want to harm adoption to the unified headers. We'll undef EPOLL_CLOEXEC
 * if we don't have epoll_create1 for the time being, and maybe revisit this
 * later.
 *
 * https://github.com/android-ndk/ndk/issues/302
 * https://github.com/android-ndk/ndk/issues/394
 */
#if __ANDROID_API__ < __ANDROID_API_L__ && defined(EPOLL_CLOEXEC)
#undef EPOLL_CLOEXEC
#endif

int epoll_ctl(int __epoll_fd, int __op, int __fd, struct epoll_event* __event);
int epoll_wait(int __epoll_fd, struct epoll_event* __events, int __event_count, int __timeout_ms);
int epoll_pwait(int __epoll_fd, struct epoll_event* __events, int __event_count, int __timeout_ms, const sigset_t* __mask) __INTRODUCED_IN(21);

__END_DECLS

#endif
