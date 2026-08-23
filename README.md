# lslocales <!-- omit in toc -->

Lists installed system locales (on Linux, macOS, Windows)


![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![GitHub release](https://img.shields.io/github/v/release/sistools/lslocales.svg)](https://github.com/sistools/lslocales/releases/latest)
[![Last Commit](https://img.shields.io/github/last-commit/sistools/lslocales)](https://github.com/sistools/lslocales/commits/master)
[![CI](https://github.com/sistools/lslocales/actions/workflows/ci.yml/badge.svg)](https://github.com/sistools/lslocales/actions/workflows/ci.yml)


## Table of Contents <!-- omit in toc -->

- [Introduction](#introduction)
- [Installation](#installation)
- [Components](#components)
- [Examples](#examples)
- [Project Information](#project-information)
  - [Where to get help](#where-to-get-help)
  - [Contribution guidelines](#contribution-guidelines)
  - [Dependencies](#dependencies)
    - [Tests-only Dependencies](#tests-only-dependencies)
  - [Related projects](#related-projects)
  - [License](#license)


## Introduction

**lslocales** is a small, standalone utility program that lists the locales installed on the host. On Windows it enumerates system locales via `EnumSystemLocales` and resolves each LCID with `GetLocaleInfo` (`LOCALE_SNAME`, falling back to `LOCALE_SLANGUAGE`). On POSIX it lists locale names as `locale -a` would, with a directory-scan fallback.

This is **not** **lsloc** (a recursive file-locator in `~/.bin`).


## Installation

Detailed instructions - via **CMake** - are provided in the accompanying [INSTALL.md](./INSTALL.md)
file.


## Components

The project creates a single executable program, **lslocales**.


## Examples

```bash
$ lslocales --names-only | head
C
C.UTF-8
POSIX
en_AU.UTF-8
en_US.UTF-8
. . .
```

Default output is `id<TAB>name`. When the id and name are identical (typical on POSIX) a single column is emitted; on Windows the id is the hex LCID.


## Project Information


### Where to get help

[GitHub Page](https://github.com/sistools/lslocales "GitHub Page")


### Contribution guidelines

Defect reports, feature requests, and pull requests are welcome on [the **lslocales** GitHub page](https://github.com/sistools/lslocales).


### Dependencies

**lslocales** depends on:

* [**CLASP**](https://github.com/synesissoftware/CLASP);
* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism);
* [**sistools-common-c**](https://github.com/sistools/sistools-common-c);
* [**STLSoft**](https://github.com/synesissoftware/STLSoft);


#### Tests-only Dependencies

For unit-testing, **lslocales** depends additionally on:

* [**xTests**](https://github.com/synesissoftware/xTests);


### Related projects

Other (similar) projects include:

* [**chomp**](https://github.com/sistools/chomp);
* [**errni**](https://github.com/sistools/errni) (errno on all platforms, and also GetLastError codes on Windows);
* [**lnunique**](https://github.com/sistools/lnunique);
* [**lstrip**](https://github.com/sistools/lstrip);
* [**mksock**](https://github.com/sistools/mksock) (Unix-only);
* [**ReadDebugString**](https://github.com/sistools/ReadDebugString) (Windows-only);
* [**realpath**](https://github.com/sistools/realpath) (Windows-only);
* [**rstrip**](https://github.com/sistools/rstrip);
* [**WriteDebugString**](https://github.com/sistools/WriteDebugString) (Windows-only);


### License

**lslocales** is released under the 3-clause BSD license. See [LICENSE](./LICENSE) for details.


<!-- ########################### end of file ########################### -->
