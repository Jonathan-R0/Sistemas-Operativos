# Tabla de Contenidos

- [Parte 1](#parte-1)
    - [Strace Cat](#strace-cat)
    - [Strace Ls](#strace-ls)
    - [Strace Touch](#strace-touch)
    - [Syscalls Utilizadas](#syscalls-utilizadas)
    - [Strace Strace](#strace-strace)
    - [Syscalls Utilizadas por Strace](#syscalls-utilizadas-por-strace)
- [Parte 2](#parte-2)
    - [Ejemplos de Ejecución](#ejemplos-de-ejecución)

# Parte 1

Adjuntamos los comandos ejecutados con strace.

## Strace Cat

Ejecutamos el comando `strace cat hello.txt` para ver como se imprime el contenido del archivo `hello.txt`.

```bash
execve("/usr/bin/cat", ["cat", "hello.txt"], 0x7ffc8a174718 /* 58 vars */) = 0
brk(NULL)                               = 0x55b1e1fb7000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffe2587f0f0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0327564000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=81835, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 81835, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f0327550000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f0327328000
mmap(0x7f0327350000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f0327350000
mmap(0x7f03274e5000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f03274e5000
mmap(0x7f032753d000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7f032753d000
mmap(0x7f0327543000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f0327543000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0327325000
arch_prctl(ARCH_SET_FS, 0x7f0327325740) = 0
set_tid_address(0x7f0327325a10)         = 11065
set_robust_list(0x7f0327325a20, 24)     = 0
rseq(0x7f03273260e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f032753d000, 16384, PROT_READ) = 0
mprotect(0x55b1e02c7000, 4096, PROT_READ) = 0
mprotect(0x7f032759e000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f0327550000, 81835)           = 0
getrandom("\x64\x2a\x84\x3a\xbc\x29\xa9\xb1", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x55b1e1fb7000
brk(0x55b1e1fd8000)                     = 0x55b1e1fd8000
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=8303504, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 8303504, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f0326b39000
close(3)                                = 0
newfstatat(1, "", {st_mode=S_IFREG|0664, st_size=0, ...}, AT_EMPTY_PATH) = 0
openat(AT_FDCWD, "hello.txt", O_RDONLY) = 3
newfstatat(3, "", {st_mode=S_IFREG|0664, st_size=3, ...}, AT_EMPTY_PATH) = 0
fadvise64(3, 0, 0, POSIX_FADV_SEQUENTIAL) = 0
mmap(NULL, 139264, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f0326b17000
read(3, "hi\n", 131072)                 = 3
write(1, "hi\n", 3)                     = 3
read(3, "", 131072)                     = 0
munmap(0x7f0326b17000, 139264)          = 0
close(3)                                = 0
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

## Strace Ls

Ejecutamos este comando dentro de un directorio de prueba. Notamos como en el `execve`, la primera llamada al sistema registrada, lo se le pasan argumentos, pues no enviamos ninguno.

```bash
execve("/usr/bin/ls", ["ls"], 0x7ffd3e36de30 /* 58 vars */) = 0
brk(NULL)                               = 0x55c458f0c000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffca27b4100) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fb73ae41000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=81835, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 81835, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fb73ae2d000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=166280, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 177672, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fb73ae01000
mprotect(0x7fb73ae07000, 139264, PROT_NONE) = 0
mmap(0x7fb73ae07000, 106496, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7fb73ae07000
mmap(0x7fb73ae21000, 28672, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x20000) = 0x7fb73ae21000
mmap(0x7fb73ae29000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x27000) = 0x7fb73ae29000
mmap(0x7fb73ae2b000, 5640, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fb73ae2b000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fb73abd9000
mmap(0x7fb73ac01000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7fb73ac01000
mmap(0x7fb73ad96000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7fb73ad96000
mmap(0x7fb73adee000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7fb73adee000
mmap(0x7fb73adf4000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fb73adf4000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libpcre2-8.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=613064, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 615184, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fb73ab42000
mmap(0x7fb73ab44000, 438272, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7fb73ab44000
mmap(0x7fb73abaf000, 163840, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6d000) = 0x7fb73abaf000
mmap(0x7fb73abd7000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x94000) = 0x7fb73abd7000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fb73ab40000
arch_prctl(ARCH_SET_FS, 0x7fb73ab40c40) = 0
set_tid_address(0x7fb73ab40f10)         = 11515
set_robust_list(0x7fb73ab40f20, 24)     = 0
rseq(0x7fb73ab415e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7fb73adee000, 16384, PROT_READ) = 0
mprotect(0x7fb73abd7000, 4096, PROT_READ) = 0
mprotect(0x7fb73ae29000, 4096, PROT_READ) = 0
mprotect(0x55c458673000, 4096, PROT_READ) = 0
mprotect(0x7fb73ae7b000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7fb73ae2d000, 81835)           = 0
statfs("/sys/fs/selinux", 0x7ffca27b4140) = -1 ENOENT (No such file or directory)
statfs("/selinux", 0x7ffca27b4140)      = -1 ENOENT (No such file or directory)
getrandom("\x91\x7b\xf8\x6a\xa5\x4c\x39\x90", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x55c458f0c000
brk(0x55c458f2d000)                     = 0x55c458f2d000
openat(AT_FDCWD, "/proc/filesystems", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0444, st_size=0, ...}, AT_EMPTY_PATH) = 0
read(3, "nodev\tsysfs\nnodev\ttmpfs\nnodev\tbd"..., 1024) = 407
read(3, "", 1024)                       = 0
close(3)                                = 0
access("/etc/selinux/config", F_OK)     = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=8303504, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 8303504, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fb73a354000
close(3)                                = 0
ioctl(1, TCGETS, {B38400 opost isig icanon echo ...}) = 0
ioctl(1, TIOCGWINSZ, {ws_row=56, ws_col=69, ws_xpixel=0, ws_ypixel=0}) = 0
openat(AT_FDCWD, ".", O_RDONLY|O_NONBLOCK|O_CLOEXEC|O_DIRECTORY) = 3
newfstatat(3, "", {st_mode=S_IFDIR|0775, st_size=4096, ...}, AT_EMPTY_PATH) = 0
getdents64(3, 0x55c458f13ab0 /* 4 entries */, 32768) = 112
getdents64(3, 0x55c458f13ab0 /* 0 entries */, 32768) = 0
close(3)                                = 0
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x2), ...}, AT_EMPTY_PATH) = 0
write(1, "testdir  testfile\n", 18testdir  testfile
)     = 18
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

## Strace Touch

Finalmente ejecutamos strace para el comando touch.

```bash
execve("/usr/bin/touch", ["touch", "test.txt"], 0x7fff5c107ff8 /* 58 vars */) = 0
brk(NULL)                               = 0x5611285e2000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffdcaaa02d0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f34cb38f000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=81835, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 81835, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f34cb37b000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f34cb153000
mmap(0x7f34cb17b000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f34cb17b000
mmap(0x7f34cb310000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f34cb310000
mmap(0x7f34cb368000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7f34cb368000
mmap(0x7f34cb36e000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f34cb36e000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f34cb150000
arch_prctl(ARCH_SET_FS, 0x7f34cb150740) = 0
set_tid_address(0x7f34cb150a10)         = 10897
set_robust_list(0x7f34cb150a20, 24)     = 0
rseq(0x7f34cb1510e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f34cb368000, 16384, PROT_READ) = 0
mprotect(0x561126762000, 4096, PROT_READ) = 0
mprotect(0x7f34cb3c9000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f34cb37b000, 81835)           = 0
getrandom("\xb2\x9c\xac\x7f\xeb\xc9\xb2\xc6", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x5611285e2000
brk(0x561128603000)                     = 0x561128603000
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=8303504, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 8303504, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f34ca964000
close(3)                                = 0
openat(AT_FDCWD, "test.txt", O_WRONLY|O_CREAT|O_NOCTTY|O_NONBLOCK, 0666) = 3
dup2(3, 0)                              = 0
close(3)                                = 0
utimensat(0, NULL, NULL, 0)             = 0
close(0)                                = 0
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```

Vemos en cada línea del strace posee en orden de llamada, el orden de la syscall llamada, los parámetros que recibe y el valor de retorno de cada una. Es una herramienta muy útil para entender como funciona un programa, pues intercepta todas las llamadas al sistema que realiza el mismo.

## Syscalls Utilizadas

- [`read`](https://man7.org/linux/man-pages/man2/read.2.html)
- [`ioctl`](https://man7.org/linux/man-pages/man2/ioctl.2.html)
- [`mprotect`](https://man7.org/linux/man-pages/man2/mprotect.2.html)
- [`getrandom`](https://man7.org/linux/man-pages/man2/getrandom.2.html)
- [`fadvise64`](https://man7.org/linux/man-pages/man2/fadvise64.2.html)
- [`set_robust_list`](https://man7.org/linux/man-pages/man2/set_robust_list.2.html)
- [`utimensat`](https://man7.org/linux/man-pages/man2/utimensat.2.html)
- [`openat`](https://man7.org/linux/man-pages/man2/openat.2.html)
- [`mmap`](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [`rseq`](https://www.spinics.net/lists/kernel/msg3548662.html)
- [`access`](https://man7.org/linux/man-pages/man2/access.2.html)
- [`pread64`](https://man7.org/linux/man-pages/man2/pread64.2.html)
- [`execve`](https://man7.org/linux/man-pages/man2/execve.2.html)
- [`arch_prctl`](https://man7.org/linux/man-pages/man2/arch_prctl.2.html)
- [`newfstatat`](https://man7.org/linux/man-pages/man2/newfstatat.2.html)
- [`statfs`](https://man7.org/linux/man-pages/man2/statfs.2.html)
- [`close`](https://man7.org/linux/man-pages/man2/close.2.html)
- [`prlimit64`](https://man7.org/linux/man-pages/man2/prlimit64.2.html)
- [`munmap`](https://man7.org/linux/man-pages/man2/munmap.2.html)
- [`set_tid_address`](https://man7.org/linux/man-pages/man2/set_tid_address.2.html)
- [`exit_group`](https://man7.org/linux/man-pages/man2/exit_group.2.html)
- [`write`](https://man7.org/linux/man-pages/man2/write.2.html)
- [`getdents64`](https://man7.org/linux/man-pages/man2/getdents64.2.html)
- [`dup2`](https://man7.org/linux/man-pages/man2/dup2.2.html)
- [`brk`](https://man7.org/linux/man-pages/man2/brk.2.html)

Aquí encontramos syscalls conocidas en general como es el caso de `read`, `openat`, `close` y `write`. Otras fueron vistas en los trabajos a realizar durante la materia, como es el caso de `mmap`, `munmap` y `brk`, vistos en el TP1 para implementar el malloc. Las primeras dos nos permitieron pedir y devolver memoria respectivamente mientras que vimos que `brk` en cambio aumenta el final del data segment en `n` bytes y devuelve la posición de memoria correspondiente. Una de las syscalls vistas en los labs fue el caso de `execve` (y toda la familia de syscalls del estilo) en el lab shell. Estas nos permiten transformar un proceso para que pase a ser otro diferente.

## Strace Strace

```bash
execve("/usr/bin/strace", ["strace"], 0x7ffd7f15b810 /* 58 vars */) = 0
brk(NULL)                               = 0x559e7829d000
arch_prctl(0x3001 /* ARCH_??? */, 0x7fffb0c4cb30) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f5db9b77000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=81835, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 81835, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f5db9b63000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libunwind-ptrace.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=18840, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 20840, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5db9b5d000
mmap(0x7f5db9b5e000, 8192, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1000) = 0x7f5db9b5e000
mmap(0x7f5db9b60000, 4096, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f5db9b60000
mmap(0x7f5db9b61000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f5db9b61000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libunwind-x86_64.so.8", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=71920, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 117200, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5db9b40000
mprotect(0x7f5db9b42000, 61440, PROT_NONE) = 0
mmap(0x7f5db9b42000, 49152, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7f5db9b42000
mmap(0x7f5db9b4e000, 8192, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xe000) = 0x7f5db9b4e000
mmap(0x7f5db9b51000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x10000) = 0x7f5db9b51000
mmap(0x7f5db9b53000, 39376, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f5db9b53000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5db9918000
mmap(0x7f5db9940000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f5db9940000
mmap(0x7f5db9ad5000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f5db9ad5000
mmap(0x7f5db9b2d000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7f5db9b2d000
mmap(0x7f5db9b33000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f5db9b33000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/liblzma.so.5", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=170456, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 172296, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5db98ed000
mmap(0x7f5db98f0000, 110592, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f5db98f0000
mmap(0x7f5db990b000, 45056, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1e000) = 0x7f5db990b000
mmap(0x7f5db9916000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f5db9916000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libunwind.so.8", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=63744, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 109264, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f5db98d2000
mmap(0x7f5db98d4000, 40960, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x2000) = 0x7f5db98d4000
mmap(0x7f5db98de000, 12288, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xc000) = 0x7f5db98de000
mmap(0x7f5db98e1000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xe000) = 0x7f5db98e1000
mmap(0x7f5db98e3000, 39632, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f5db98e3000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f5db98d0000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f5db98ce000
arch_prctl(ARCH_SET_FS, 0x7f5db98d1540) = 0
set_tid_address(0x7f5db98d1810)         = 11794
set_robust_list(0x7f5db98d1820, 24)     = 0
rseq(0x7f5db98d1ee0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f5db9b2d000, 16384, PROT_READ) = 0
mprotect(0x7f5db9916000, 4096, PROT_READ) = 0
mprotect(0x7f5db98e1000, 4096, PROT_READ) = 0
mprotect(0x7f5db9b51000, 4096, PROT_READ) = 0
mprotect(0x7f5db9b61000, 4096, PROT_READ) = 0
mprotect(0x559e7802c000, 393216, PROT_READ) = 0
mprotect(0x7f5db9bb1000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f5db9b63000, 81835)           = 0
getrandom("\xd2\xf1\x32\x68\x29\xe8\xa7\x9c", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x559e7829d000
brk(0x559e782be000)                     = 0x559e782be000
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=8303504, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 8303504, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f5db90e2000
close(3)                                = 0
getpid()                                = 11794
uname({sysname="Linux", nodename="niyopc", ...}) = 0
openat(AT_FDCWD, "/proc/sys/kernel/pid_max", O_RDONLY) = 3
read(3, "4194304\n", 23)                = 8
close(3)                                = 0
write(2, "strace: must have PROG [ARGS] or"..., 40strace: must have PROG [ARGS] or -p PID
) = 40
write(2, "Try 'strace -h' for more informa"..., 38Try 'strace -h' for more information.
) = 38
getpid()                                = 11794
exit_group(1)                           = ?
+++ exited with 1 +++
```

## Syscalls Utilizadas por Strace

- [`read`](https://man7.org/linux/man-pages/man2/read.2.html)
- [`mprotect`](https://man7.org/linux/man-pages/man2/mprotect.2.html)
- [`getrandom`](https://man7.org/linux/man-pages/man2/getrandom.2.html)
- [`set_robust_list`](https://man7.org/linux/man-pages/man2/set_robust_list.2.html)
- [`openat`](https://man7.org/linux/man-pages/man2/openat.2.html)
- [`uname`](https://man7.org/linux/man-pages/man2/uname.2.html)
- [`mmap`](https://man7.org/linux/man-pages/man2/mmap.2.html)
- [`rseq`](https://www.spinics.net/lists/kernel/msg3548662.html)
- [`access`](https://man7.org/linux/man-pages/man2/access.2.html)
- [`pread64`](https://man7.org/linux/man-pages/man2/pread64.2.html)
- [`execve`](https://man7.org/linux/man-pages/man2/execve.2.html)
- [`arch_prctl`](https://man7.org/linux/man-pages/man2/arch_prctl.2.html)
- [`getpid`](https://man7.org/linux/man-pages/man2/getpid.2.html)
- [`newfstatat`](https://man7.org/linux/man-pages/man2/newfstatat.2.html)
- [`close`](https://man7.org/linux/man-pages/man2/close.2.html)
- [`prlimit64`](https://man7.org/linux/man-pages/man2/prlimit64.2.html)
- [`munmap`](https://man7.org/linux/man-pages/man2/munmap.2.html)
- [`set_tid_address`](https://man7.org/linux/man-pages/man2/set_tid_address.2.html)
- [`exit_group`](https://man7.org/linux/man-pages/man2/exit_group.2.html)
- [`write`](https://man7.org/linux/man-pages/man2/write.2.html)
- [`brk`](https://man7.org/linux/man-pages/man2/brk.2.html)

# Parte 2

Nuestra implementación de strace comienza verificando que la cantidad de argumentos enviados sean suficientes para que el proceso hijo pueda nacer. Posteriormente se forkea al proceso base, el cual será el padre y el hijo sera el proceso que ejecutara el comando recibido por argumento.

Antes de ello, se hace una llamada a `ptrace(PTRACE_TRACEME,0,0,0)` para que el proceso hijo pueda ser rastreado por el padre. Luego se hace una llamada a `execvp` para que el proceso hijo ejecute el comando recibido por argumento. 

Aclaramos que segun la documentacion, al llamar a `ptrace(PTRACE_TRACEME, ...)` se ignoran los últimos tres argumentos.

```
PTRACE_TRACEME
              Indicate that this process is to be traced by its parent.  A process probably shouldn't make this request if its parent isn't expecting to trace it.  (pid, addr, and data are ignored.)
```

Posterior al wait del child llamamos a `ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_TRACESYSGOOD)`. El flag `PTRACE_O_TRACESYSGOOD` nos permite distinguir entre una syscall y una señal.

```
PTRACE_O_TRACESYSGOOD (since Linux 2.4.6)
                     When  delivering  system  call traps, set bit 7 in the signal number (i.e., deliver SIGTRAP|0x80).  This makes it easy for the tracer to distinguish normal traps from those caused by a
                     system call.
```

El flag `PTRACE_SETOPTIONS` simplemente nos permite setear opciones para el proceso hijo (siendo que esto lo indicamos en el segundo parámetro de la llamada).

Posteriormente se hace un loop infinito que se romperá cuando el proceso hijo termine. En cada iteración se hacen dos `ptrace(PTRACE_SYSCALL, pid, 0, 0)`, uno para detener la ejecución del proceso hijo y detectar la syscall utilizada y otra para detener la ejecución del hijo al finalizar la syscall y detectar el valor de retorno.

Notamos que estas pausas y la idea hacer la detección de la syscall (al momento de hacer `ptrace(PTRACE_GETREGS, pid, 0, &reg)`) y luego la detección del valor del retorno (al momento de hacer `ptrace(PTRACE_GET_SYSCALL_INFO, pid, sizeof(info), &info)`) están detalladas en la documentación (sección en la que se detalla `PTRACE_SYSCALL`)

```
PTRACE_SYSCALL, PTRACE_SINGLESTEP
              Restart  the  stopped  tracee as for PTRACE_CONT, but arrange for the tracee to be stopped at the next entry to or exit from a system call, or after execution of a single instruction, respectively.  (The tracee will also, as usual, be stopped upon receipt of a signal.)  From the tracer's perspective, the tracee will appear to have been stopped by receipt of a SIGTRAP.   So,  for
              PTRACE_SYSCALL,  for  example, the idea is to inspect the arguments to the system call at the first stop, then do another PTRACE_SYSCALL and inspect the return value of the system call at the
              second stop.  The data argument is treated as for PTRACE_CONT.  (addr is ignored.)
```

Finalmente detenemos el proceso padre cuando se ejecuta `exit_group(2)` (para hacer un exit a todos los threads del proceso hijo).

## Ejemplos de Ejecución

Ejecución de `./strace cat hello.txt`

```
brk = 94101757153280
arch_prctl = -22
mmap = 140336400797696
access = -2
openat = 3
newfstatat = 0
mmap = 140336400715776
close = 0
openat = 3
read = 832
pread64 = 784
pread64 = 48
pread64 = 68
newfstatat = 0
pread64 = 784
mmap = 140336398454784
mmap = 140336398618624
mmap = 140336400277504
mmap = 140336400637952
mmap = 140336400662528
close = 0
mmap = 140336398442496
arch_prctl = 0
set_tid_address = 13379
set_robust_list = 0
rseq = 0
mprotect = 0
mprotect = 0
mprotect = 0
prlimit64 = 0
munmap = 0
getrandom = 8
brk = 94101757153280
brk = 94101757288448
openat = 3
newfstatat = 0
mmap = 140336390135808
close = 0
newfstatat = 0
openat = 3
newfstatat = 0
fadvise64 = 0
mmap = 140336389996544
read = 3
hi
write = 3
read = 0
munmap = 0
close = 0
close = 0
close = 0
+++ exited with 0 +++
```

Ejecución de `strace cat hello.txt`

```bash
execve("/usr/bin/cat", ["cat", "hello.txt"], 0x7ffd1f5d8448 /* 58 vars */) = 0
brk(NULL)                               = 0x564bb4c9c000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffee294b260) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1959dbf000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=81835, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 81835, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f1959dab000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0i8\235HZ\227\223\333\350s\360\352,\223\340."..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=2216304, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2260560, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f1959b83000
mmap(0x7f1959bab000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f1959bab000
mmap(0x7f1959d40000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7f1959d40000
mmap(0x7f1959d98000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x214000) = 0x7f1959d98000
mmap(0x7f1959d9e000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f1959d9e000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1959b80000
arch_prctl(ARCH_SET_FS, 0x7f1959b80740) = 0
set_tid_address(0x7f1959b80a10)         = 12433
set_robust_list(0x7f1959b80a20, 24)     = 0
rseq(0x7f1959b810e0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f1959d98000, 16384, PROT_READ) = 0
mprotect(0x564bb3171000, 4096, PROT_READ) = 0
mprotect(0x7f1959df9000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f1959dab000, 81835)           = 0
getrandom("\xe2\x0e\xdf\xa3\x26\x15\xa4\x00", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x564bb4c9c000
brk(0x564bb4cbd000)                     = 0x564bb4cbd000
openat(AT_FDCWD, "/usr/lib/locale/locale-archive", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=8303504, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 8303504, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f1959394000
close(3)                                = 0
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x1), ...}, AT_EMPTY_PATH) = 0
openat(AT_FDCWD, "hello.txt", O_RDONLY) = 3
newfstatat(3, "", {st_mode=S_IFREG|0664, st_size=3, ...}, AT_EMPTY_PATH) = 0
fadvise64(3, 0, 0, POSIX_FADV_SEQUENTIAL) = 0
mmap(NULL, 139264, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f1959372000
read(3, "hi\n", 131072)                 = 3
write(1, "hi\n", 3hi
)                     = 3
read(3, "", 131072)                     = 0
munmap(0x7f1959372000, 139264)          = 0
close(3)                                = 0
close(1)                                = 0
close(2)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++
```
