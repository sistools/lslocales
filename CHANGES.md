# lslocales - Changes <!-- omit in toc -->


## 0.1.0 - 24th August 2026

* Initial port from internal `2021-dev/…/tools/lslocales` into **sistools**;
* C CLI with **CLASP** plus **sistools-common-c** `--help` / `--version`;
* Windows: `EnumSystemLocales` + `GetLocaleInfo` (`LOCALE_SNAME` / `LOCALE_SLANGUAGE`);
* POSIX: `locale -a` with a directory-scan fallback;
* **CMake**, helper scripts, unit tests, man page, and CI;


<!-- ########################### end of file ########################### -->
