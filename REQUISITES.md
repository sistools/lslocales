# lslocales - Requisites <!-- omit in toc -->


- [Introduction](#introduction)
- [Installation by CMake](#installation-by-cmake)
  - [CLASP](#clasp)
  - [Diagnosticism](#diagnosticism)
  - [sistools-common-c](#sistools-common-c)
  - [STLSoft](#stlsoft)
  - [xTests - required only for testing](#xtests---required-only-for-testing)
- [Installation by other means](#installation-by-other-means)


## Introduction

The **lslocales** program (implemented in [**entry.c**](./entry.c) and related sources) is implemented in terms of:

* [**CLASP**](https://github.com/synesissoftware/CLASP);
* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism);
* [**sistools-common-c**](https://github.com/sistools/sistools-common-c);
* [**STLSoft**](https://github.com/synesissoftware/STLSoft);

Further, the **lslocales_test** program (implemented in [**lslocales_test.c**](./lslocales_test.c)) also depends on:

* [**xTests**](https://github.com/synesissoftware/xTests);

> **NOTE**: if you do not wish to build **lslocales_test**, then you need not obtain/install the **xTests** dependency.


## Installation by CMake


### CLASP

**CLASP** is obtained from **https://github.com/synesissoftware/CLASP**, and it provides the means to install via **CMake**, as in the following:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/synesissoftware/CLASP
$ cd ~/open-source/CLASP
$ ./prepare_cmake.sh -m
$ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```


### Diagnosticism

**Diagnosticism** is obtained from **https://github.com/synesissoftware/Diagnosticism**, and it provides the means to install via **CMake**, as in the following:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/synesissoftware/Diagnosticism
$ cd ~/open-source/Diagnosticism
$ ./prepare_cmake.sh -m
$ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```


### sistools-common-c

The **sistools-common-c** library provides shared helpers for **sistools** programs, including Diagnosticism-based `--help` / `--version` usage output.

**sistools-common-c** is obtained from **https://github.com/sistools/sistools-common-c**, and it provides the means to install via **CMake**, as in the following:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/sistools/sistools-common-c
$ cd ~/open-source/sistools-common-c
$ ./prepare_cmake.sh -E -T -m
$ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```

> **NOTE**: **sistools-common-c** itself depends on **CLASP**, **Diagnosticism**, and **STLSoft**; install those first (see its [INSTALL.md](https://github.com/sistools/sistools-common-c/blob/master/INSTALL.md)).


### STLSoft

**STLSoft** is obtained from **https://github.com/synesissoftware/STLSoft**, and it provides the means to install via **CMake**, as in the following:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/synesissoftware/STLSoft
$ cd ~/open-source/STLSoft
$ ./prepare_cmake.sh -m
$ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```


### xTests - required only for testing

**xTests** is obtained from **https://github.com/synesissoftware/xTests**, and it provides the means to install via **CMake**, as in the following:

```bash
$ mkdir -p ~/open-source
$ cd ~/open-source
$ git clone https://github.com/synesissoftware/xTests
$ cd ~/open-source/xTests
$ ./prepare_cmake.sh -m
$ sudo cmake --install ${SIS_CMAKE_BUILD_DIR:-./_build} --config Release
```


## Installation by other means

If you cannot or will not use **CMake** and wish to use other means, we do not currently provide instructions for that - because we cannot know what or how you wish to operate - but if you post a question (in **https://github.com/sistools/lslocales/issues**) we will attempt to help you.


<!-- ########################### end of file ########################### -->
