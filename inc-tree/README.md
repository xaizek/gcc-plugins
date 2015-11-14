_inc-tree, 0.1, 2015_

### Brief Description ###

The purpose of this plugin is to display tree of includes per compilation unit.
With its help one can answer from which place some particular include comes
from.

### Supported Environment ###

Tested with GCC 4.9.3, should work with 4.9 and maybe with newer versions.

### Prerequisites ###

* C++11 compatible compiler.
* GCC installation with plugin support.
* GNU Make.

### Building ###

```
make
```

### Usage example ###

Invocation:

```
g++ -std=c++11 -fplugin=./inc-tree.so test.c
```

Output:

```
test.c
    /usr/include/stdc-predef.h
    /usr/include/stdio.h
        /usr/include/features.h
            /usr/include/sys/cdefs.h
                /usr/include/bits/wordsize.h
            /usr/include/gnu/stubs.h
                /usr/include/gnu/stubs-64.h
        /usr/lib64/gcc/x86_64-slackware-linux/4.9.3/include/stddef.h
        /usr/include/bits/types.h
            /usr/include/bits/wordsize.h
            /usr/include/bits/typesizes.h
        /usr/include/libio.h
            /usr/include/_G_config.h
                /usr/lib64/gcc/x86_64-slackware-linux/4.9.3/include/stddef.h
                /usr/include/wchar.h
            /usr/lib64/gcc/x86_64-slackware-linux/4.9.3/include/stdarg.h
        /usr/include/bits/stdio_lim.h
        /usr/include/bits/sys_errlist.h
```

### License ###

GNU General Public License, version 3 or later
