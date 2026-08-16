
/* *********************************************************
 * includes
 */

#include "lslocales.h"

#include <xtests/xtests.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* *********************************************************
 * forward declarations
 */

static void test_version_macros(void);
static void test_lists_at_least_one_locale(void);
static void test_names_only_emits_single_column(void);
static void test_identical_id_and_name_are_single_column(void);
#if !defined(_WIN32)
static void test_posix_names_include_c(void);
#endif /* !_WIN32 */


/* *********************************************************
 * main
 */

int main(int argc, char* argv[])
{
    int retCode = EXIT_SUCCESS;
    int verbosity;

    XTESTS_COMMANDLINE_PARSE_VERBOSITY(argc, argv, &verbosity);

    if (XTESTS_START_RUNNER("lslocales", verbosity))
    {
        XTESTS_RUN_CASE(test_version_macros);
        XTESTS_RUN_CASE(test_lists_at_least_one_locale);
        XTESTS_RUN_CASE(test_names_only_emits_single_column);
        XTESTS_RUN_CASE(test_identical_id_and_name_are_single_column);
#if !defined(_WIN32)
        XTESTS_RUN_CASE(test_posix_names_include_c);
#endif /* !_WIN32 */

        XTESTS_PRINT_RESULTS();
        XTESTS_END_RUNNER_UPDATE_EXITCODE(&retCode);
    }

    return retCode;
}


/* *********************************************************
 * test function implementations
 */

static void test_version_macros(void)
{
    XTESTS_TEST_INTEGER_EQUAL(0, SISTOOL_LSLOCALES_VER_MAJOR);
    XTESTS_TEST_INTEGER_EQUAL(1, SISTOOL_LSLOCALES_VER_MINOR);
    XTESTS_TEST_INTEGER_EQUAL(0, SISTOOL_LSLOCALES_VER_PATCH);
    XTESTS_TEST_INTEGER_EQUAL(0x81, SISTOOL_LSLOCALES_VER_ALPHABETA);
    XTESTS_TEST_INTEGER_EQUAL(SISTOOL_LSLOCALES_VER_PATCH, SISTOOL_LSLOCALES_VER_REVISION);
}

static void test_lists_at_least_one_locale(void)
{
    FILE*   stm = tmpfile();
    int     rc;
    char    buf[256];

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, stm));

    rc = sistool_lslocales(stm, SISTOOL_LSLOCALES_F_NAMES_ONLY);

    XTESTS_TEST_INTEGER_EQUAL(0, rc);

    rewind(stm);

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, fgets(buf, (int)sizeof(buf), stm)));

    XTESTS_TEST_BOOLEAN_TRUE('\0' != buf[0]);

    fclose(stm);
}

static void test_names_only_emits_single_column(void)
{
    FILE*   stm = tmpfile();
    int     rc;
    char    buf[256];
    char*   tab;

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, stm));

    rc = sistool_lslocales(stm, SISTOOL_LSLOCALES_F_NAMES_ONLY);

    XTESTS_TEST_INTEGER_EQUAL(0, rc);

    rewind(stm);

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, fgets(buf, (int)sizeof(buf), stm)));

    tab = strchr(buf, '\t');

    XTESTS_TEST_POINTER_EQUAL(NULL, tab);

    fclose(stm);
}

static void test_identical_id_and_name_are_single_column(void)
{
    FILE*   stm = tmpfile();
    int     rc;
    char    buf[256];
    int     saw_tab = 0;
    int     nlines = 0;

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, stm));

    rc = sistool_lslocales(stm, 0);

    XTESTS_TEST_INTEGER_EQUAL(0, rc);

    rewind(stm);

    while (NULL != fgets(buf, (int)sizeof(buf), stm))
    {
        ++nlines;

        if (NULL != strchr(buf, '\t'))
        {
            saw_tab = 1;
        }
    }

    XTESTS_TEST_BOOLEAN_TRUE(nlines > 0);

#if defined(_WIN32)
    XTESTS_TEST_BOOLEAN_TRUE(saw_tab);
#else /* ? _WIN32 */
    XTESTS_TEST_BOOLEAN_FALSE(saw_tab);
#endif /* _WIN32 */

    fclose(stm);
}

#if !defined(_WIN32)

static void test_posix_names_include_c(void)
{
    FILE*   stm = tmpfile();
    int     rc;
    char    buf[256];
    int     found = 0;

    XTESTS_REQUIRE(XTESTS_TEST_POINTER_NOT_EQUAL(NULL, stm));

    rc = sistool_lslocales(stm, SISTOOL_LSLOCALES_F_NAMES_ONLY);

    XTESTS_TEST_INTEGER_EQUAL(0, rc);

    rewind(stm);

    while (NULL != fgets(buf, (int)sizeof(buf), stm))
    {
        size_t len = strlen(buf);

        while (len != 0 && ('\n' == buf[len - 1] || '\r' == buf[len - 1]))
        {
            buf[--len] = '\0';
        }

        if (0 == strcmp(buf, "C"))
        {
            found = 1;
            break;
        }
    }

    XTESTS_TEST_BOOLEAN_TRUE(found);

    fclose(stm);
}

#endif /* !_WIN32 */


/* ///////////////////////////// end of file //////////////////////////// */
