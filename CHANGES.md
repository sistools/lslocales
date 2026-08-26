# lslocales - Changes <!-- omit in toc -->


## 0.1.1 - 27th August 2026

* Fixed **cmake/BuildType.cmake** so the default `CMAKE_BUILD_TYPE` is set correctly in the CMake cache (`set(CMAKE_BUILD_TYPE … CACHE …)` instead of `set(CACHE CMAKE_BUILD_TYPE …)`);


## 0.1.0 - 24th August 2026

* Promoted **lslocales** from beta to the 0.1.0 release;
* Raised the default language standards to C17 and C++17 and the minimum **STLSoft** version to 1.11.1;
* Added **Doxygen** API documentation and the **generate_doxygen.sh** helper script;
* Updated CI to consume the **dev** branch of **sistools-common-c** and ordered the workflow branch matrix;
* Improved helper-script path handling, terminal colour detection, project-name loading, and CMake status reporting;
* Added editor associations for C headers and documented platform support for related tools in **README.md**;


## 0.1.0-beta1 - 16th August 2026

* Initial port from internal `2021-dev/…/tools/lslocales` into **sistools**;
* C CLI with **CLASP** plus **sistools-common-c** `--help` / `--version`;
* Windows: `EnumSystemLocales` + `GetLocaleInfo` (`LOCALE_SNAME` / `LOCALE_SLANGUAGE`);
* POSIX: `locale -a` with a directory-scan fallback;
* **CMake**, helper scripts, unit tests, man page, and CI;


<!-- ########################### end of file ########################### -->
