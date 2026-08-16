/* /////////////////////////////////////////////////////////////////////////
 * File:    entry.c
 *
 * Purpose: Entry point for the lslocales program.
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


/** @file entry.c
 *
 * @brief [C] Entry point for the lslocales program.
 */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include "lslocales.h"

#include <clasp/clasp.h>

#include <sistools/common/usage.h>

#include <platformstl/filesystem/path_functions.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* /////////////////////////////////////////////////////////////////////////
 * macros
 */

#define SIS_DOTSTAR(slice)                  (int)(slice).len, (slice).ptr


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define TOOLNAME                            "lslocales"
#define PROGRAM_VER_MAJOR                   SISTOOL_LSLOCALES_VER_MAJOR
#define PROGRAM_VER_MINOR                   SISTOOL_LSLOCALES_VER_MINOR
#define PROGRAM_VER_PATCH                   SISTOOL_LSLOCALES_VER_PATCH
#define PROGRAM_VER_ALPHABETA               SISTOOL_LSLOCALES_VER_ALPHABETA
#define SUMMARY                             "Synesis System Tools"
#define COPYRIGHT                           "Copyright (c) 2021-2026 Synesis Information Systems"
#define DESCRIPTION                         "Lists installed system locales"
#define USAGE                               TOOLNAME " [ ... flags/options ... ]"


static clasp_alias_t const Aliases[] = {

    CLASP_GAP_SECTION("behaviour:"),

    CLASP_BIT_FLAG("-n", "--names-only", SISTOOL_LSLOCALES_F_NAMES_ONLY, "emit locale names only"),
    CLASP_BIT_FLAG("-c", "--codes-only", SISTOOL_LSLOCALES_F_CODES_ONLY, "emit locale identifiers only"),
    CLASP_BIT_FLAG("-S", "--supported", SISTOOL_LSLOCALES_F_SUPPORTED, "list supported locales (Windows; no-op on POSIX if the inventory is not split)"),
    CLASP_BIT_FLAG(NULL, "--no-sort", SISTOOL_LSLOCALES_F_UNSORTED, "preserve enumeration order rather than sorting by name"),

    CLASP_GAP_SECTION("standard flags:"),

    CLASP_FLAG(NULL, "--help", "displays this help and terminates"),
    CLASP_FLAG(NULL, "--version", "displays version information and terminates"),

    CLASP_ALIAS_ARRAY_TERMINATOR
};


/* /////////////////////////////////////////////////////////////////////////
 * functions
 */

static
int
run(
    clasp_arguments_t const*    args
,   clasp_alias_t const*        aliases
)
{
    int                         flags = 0;
    clasp_argument_t const*     firstUnusedFlagOrOption;

    if (clasp_flagIsSpecified(args, "--help")) {

        stcc_show_help(
            args
        ,   Aliases
        ,   stdout
        ,   TOOLNAME
        ,   SUMMARY
        ,   COPYRIGHT
        ,   DESCRIPTION
        ,   USAGE
        ,   PROGRAM_VER_MAJOR, PROGRAM_VER_MINOR, PROGRAM_VER_PATCH, PROGRAM_VER_ALPHABETA
        );

        return EXIT_SUCCESS;
    }

    if (clasp_flagIsSpecified(args, "--version")) {

        stcc_show_version(
            stdout
        ,   TOOLNAME
        ,   PROGRAM_VER_MAJOR, PROGRAM_VER_MINOR, PROGRAM_VER_PATCH, PROGRAM_VER_ALPHABETA
        );

        return EXIT_SUCCESS;
    }

    clasp_checkAllFlags(args, aliases, &flags);

    if (0 != clasp_reportUnusedFlagsAndOptions(args, &firstUnusedFlagOrOption, 0)) {

        fprintf(stderr, "%.*s: unrecognised flag/option: '%.*s'\n", SIS_DOTSTAR(args->programName), SIS_DOTSTAR(firstUnusedFlagOrOption->resolvedName));

        return EXIT_FAILURE;
    }

    if (0 != args->numValues) {

        fprintf(stderr, "%.*s: unexpected value '%.*s'\n", SIS_DOTSTAR(args->programName), SIS_DOTSTAR(args->values[0].value));

        return EXIT_FAILURE;
    }

    if ((SISTOOL_LSLOCALES_F_NAMES_ONLY & flags) &&
        (SISTOOL_LSLOCALES_F_CODES_ONLY & flags))
    {
        fprintf(stderr, "%.*s: --names-only and --codes-only are mutually exclusive\n", SIS_DOTSTAR(args->programName));

        return EXIT_FAILURE;
    }

    return sistool_lslocales(stdout, flags);
}


/* /////////////////////////////////////////////////////////////////////////
 * main
 */

int main(int argc, char* argv[])
{
    stlsoft_C_string_slice_m_t const    programName = platformstl_C_get_directory_path_from_path(argv[0]);

    unsigned                            flags       = 0;
    clasp_alias_t const*                aliases     = Aliases;
    clasp_diagnostic_context_t const*   ctxt        = NULL;
    clasp_arguments_t const*            args        = NULL;
    int                                 xc;

    {
        int const r = clasp_parseArguments(
            flags
        ,   argc
        ,   argv
        ,   aliases
        ,   ctxt
        ,   &args
        );

        if (0 != r) {

            fprintf(stderr, "%.*s: failed to initialise the command-line parsing libraries: %s\n", SIS_DOTSTAR(programName), strerror(r));

            return EXIT_FAILURE;
        }
    }

    xc = run(args, aliases);

    clasp_releaseArguments(args);

    return xc;
}


/* ///////////////////////////// end of file //////////////////////////// */

