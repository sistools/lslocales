# lslocales {#mainpage}

**lslocales** is a small utility and internal C library for listing locales
installed on the host.


## Components

| Unit | File(s) | Summary |
| ---- | ------- | ------- |
| Public API | `lslocales.h`, `lslocales.c` | Version macros, flags, and `sistool_lslocales()` |
| Program entry | `entry.c` | Command-line handling and program lifecycle |


## API

The `sistool_lslocales()` function writes available locale identifiers and
names to an output stream according to the supplied flags.


<!-- ########################### end of file ########################### -->
