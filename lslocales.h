/* /////////////////////////////////////////////////////////////////////////
 * File:    lslocales.h
 *
 * Purpose: Public API for lslocales.
 *
 * Created: 20th January 2021
 * Updated: 16th August 2026
 *
 * Home:    https://github.com/sistools/lslocales/
 *
 * Copyright (c) 2021-2026, Matthew Wilson and Synesis Information Systems
 * Copyright (c) 2001-2019, Matthew Wilson and Synesis Software
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer;
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution;
 * - Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ////////////////////////////////////////////////////////////////////// */


/** @file lslocales.h
 *
 * @brief [C, C++] Public API for lslocales.
 */

#ifndef LSLOCALES_INCL_H_LSLOCALES
#define LSLOCALES_INCL_H_LSLOCALES


/* /////////////////////////////////////////////////////////////////////////
 * version
 */

/**
 * @def SISTOOL_LSLOCALES_VER_MAJOR
 *
 * The major version number of lslocales
 *
 * @def SISTOOL_LSLOCALES_VER_MINOR
 *
 * The minor version number of lslocales
 *
 * @def SISTOOL_LSLOCALES_VER_PATCH
 *
 * The patch version number of lslocales
 *
 * @def SISTOOL_LSLOCALES_VER_ALPHABETA
 *
 * The alphabeta / prerelease discriminator of lslocales
 *
 * @def SISTOOL_LSLOCALES_VER
 *
 * The composite version of lslocales
 *
 * @def SISTOOL_LSLOCALES_VER_REVISION
 *
 * Alias of @c SISTOOL_LSLOCALES_VER_PATCH (for CMake scrapers and older
 * consumers)
 */

#define SISTOOL_LSLOCALES_VER_MAJOR       0
#define SISTOOL_LSLOCALES_VER_MINOR       1
#define SISTOOL_LSLOCALES_VER_PATCH       0
#define SISTOOL_LSLOCALES_VER_ALPHABETA   0x81

#define SISTOOL_LSLOCALES_VER \
    (0\
        |   (   SISTOOL_LSLOCALES_VER_MAJOR       << 24   ) \
        |   (   SISTOOL_LSLOCALES_VER_MINOR       << 16   ) \
        |   (   SISTOOL_LSLOCALES_VER_PATCH       <<  8   ) \
        |   (   SISTOOL_LSLOCALES_VER_ALPHABETA   <<  0   ) \
    )

#define SISTOOL_LSLOCALES_VER_REVISION    SISTOOL_LSLOCALES_VER_PATCH


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include <stdio.h>


/* /////////////////////////////////////////////////////////////////////////
 * API flags
 */

#define SISTOOL_LSLOCALES_F_NAMES_ONLY      (0x00000001)    /*!< emit locale names only */
#define SISTOOL_LSLOCALES_F_CODES_ONLY      (0x00000002)    /*!< emit locale identifiers only */
#define SISTOOL_LSLOCALES_F_SUPPORTED       (0x00000004)    /*!< list supported (not only installed) locales where distinct */
#define SISTOOL_LSLOCALES_F_UNSORTED        (0x00000008)    /*!< preserve enumeration order */


/* /////////////////////////////////////////////////////////////////////////
 * API functions
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** Lists available system locales to @a out according to @a flags.
 *
 * Default output is one locale per line as `id<TAB>name`. When the id and
 * name are identical (typical on POSIX), a single column is emitted. On
 * Windows the id is the hex LCID from `EnumSystemLocales` and the name is
 * `LOCALE_SNAME` (falling back to `LOCALE_SLANGUAGE`). On POSIX names come
 * from `locale -a`, with a directory-scan fallback.
 *
 * @param out Destination stream.
 * @param flags Combination of `SISTOOL_LSLOCALES_F_*`.
 *
 * @return 0 on success; non-zero on failure.
 *
 * @pre NULL != out
 */
int
sistool_lslocales(
    FILE*   out
,   int     flags
);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */


/* /////////////////////////////////////////////////////////////////////////
 * inclusion control
 */

#ifdef STLSOFT_CF_PRAGMA_ONCE_SUPPORT
# pragma once
#endif /* STLSOFT_CF_PRAGMA_ONCE_SUPPORT */

#endif /* !LSLOCALES_INCL_H_LSLOCALES */


/* ///////////////////////////// end of file //////////////////////////// */

