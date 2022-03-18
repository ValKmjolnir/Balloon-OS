# __Balloon-OS__

toy OS just for FUN

## Environment

Ubuntu 16.04.6-LTS-amd64

## System Test Environment

qemu-system-i386

if having problems compiling like this:

```C++
/usr/include/stdint.h:26:10: fatal error: bits/libc-header-start.h: No such file or directory
 #include <bits/libc-header-start.h>
          ^~~~~~~~~~~~~~~~~~~~~~~~~~
```

it is because gcc-multilib is not downloaded. try downloading it.

if get 404 when downloading on ubuntu, try `apt-get update`.