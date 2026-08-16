# lslocales - Installation and Use <!-- omit in toc -->


- [Requisites](#requisites)
- [Building](#building)
  - [via CMake](#via-cmake)


## Requisites

The **lslocales** program is implemented in terms of:

* [**CLASP**](https://github.com/synesissoftware/CLASP) - for command-line handling;
* [**Diagnosticism**](https://github.com/synesissoftware/Diagnosticism) - for version strings;
* [**sistools-common-c**](https://github.com/sistools/sistools-common-c) - for shared `--help` / `--version` usage helpers;
* [**STLSoft**](https://github.com/synesissoftware/STLSoft) - for CLI and system utility façades;

Further, the **lslocales_test** program also depends on:

* [**xTests**](https://github.com/synesissoftware/xTests);

Detailed instructions are provided in the [**REQUISITES.md**](./REQUISITES.md) document for how to obtain and install each of these that you require.

> **NOTE**: if you do not wish to build **lslocales_test**, then you need not obtain/install the **xTests** dependency (use `./prepare_cmake.sh -T`).


## Building


### via CMake

The primary choice for installation is by use of **CMake**.

1. Obtain the latest distribution of **lslocales**, from
   https://github.com/sistools/lslocales/, e.g.

    ```bash
    $ mkdir -p ~/open-source
    $ cd ~/open-source
    $ git clone https://github.com/sistools/lslocales/
    ```

2. Prepare the CMake configuration, via the **prepare_cmake.sh** script, as
   in:

    ```bash
    $ cd ~/open-source/lslocales
    $ ./prepare_cmake.sh
    ```

   The default language standard is **C11**. To override, pass one of:

    ```bash
    $ ./prepare_cmake.sh --c-standard 99
    $ ./prepare_cmake.sh --c-standard 17
    $ ./prepare_cmake.sh --c-standard 23
    ```

   **NOTE**: if you intend only to build the **lslocales** program then you can eschew building of tests (via flag `-T`) and use the command:

    ```bash
    $ cd ~/open-source/lslocales
    $ ./prepare_cmake.sh -T
    ```

3. Run a build of the generated **CMake**-derived build files via the
   **build_cmake.sh** script, as in:

    ```bash
    $ cd ~/open-source/lslocales
    $ ./build_cmake.sh
    ```

   (**NOTE**: if you provide the flag `--run-make` (=== `-m`) in step 2 then you do
   not need this step.)

4. As a check, execute the built program, as in:

    ```bash
    $ cd ~/open-source/lslocales
    $ ./_build/lslocales --help
    ```

5. If, in Step 2, you did not skip preparation for building tests (via flag `-T`), you can run the tests as in:

    ```bash
    $ cd ~/open-source/lslocales
    $ ./run_all_unit_tests.sh
    ```

6. Finally, if you wish to do so, you can install the tool on the host, via `cmake`, as in:

    ```bash
    $ cd ~/open-source/lslocales
    $ sudo cmake --install ./_build --config Release
    ```


<!-- ########################### end of file ########################### -->
