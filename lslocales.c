/* /////////////////////////////////////////////////////////////////////////
 * File:    lslocales.c
 *
 * Purpose: Implementation of the lslocales API.
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


/** @file lslocales.c
 *
 * @brief [C] Implementation of the lslocales API.
 */


/* /////////////////////////////////////////////////////////////////////////
 * includes
 */

#include "lslocales.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)

# ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
# endif
# include <windows.h>

#else /* ? _WIN32 */

# include <dirent.h>
# include <sys/stat.h>
# include <unistd.h>

#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * constants
 */

#define LSLOCALES_ID_CCH                                    64
#define LSLOCALES_NAME_CCH                                  128
#define LSLOCALES_INITIAL_CAPACITY                          64


/* /////////////////////////////////////////////////////////////////////////
 * types
 */

typedef struct locale_entry
{
    char    id[LSLOCALES_ID_CCH];
    char    name[LSLOCALES_NAME_CCH];
} locale_entry_t;

typedef struct locale_list
{
    locale_entry_t* entries;
    size_t          count;
    size_t          capacity;
    int             err;
} locale_list_t;


/* /////////////////////////////////////////////////////////////////////////
 * helpers
 */

static
void
locale_list_init(
    locale_list_t*  list
)
{
    list->entries   =   NULL;
    list->count     =   0;
    list->capacity  =   0;
    list->err       =   0;
}

static
void
locale_list_destroy(
    locale_list_t*  list
)
{
    free(list->entries);
    list->entries   =   NULL;
    list->count     =   0;
    list->capacity  =   0;
}

static
int
locale_list_reserve(
    locale_list_t*  list
)
{
    locale_entry_t* grown;
    size_t          new_capacity;

    if (list->count < list->capacity)
    {
        return 0;
    }

    new_capacity = (0 == list->capacity) ? LSLOCALES_INITIAL_CAPACITY : (list->capacity * 2);
    grown = (locale_entry_t*)realloc(list->entries, new_capacity * sizeof(locale_entry_t));

    if (NULL == grown)
    {
        list->err = ENOMEM;

        return -1;
    }

    list->entries   =   grown;
    list->capacity  =   new_capacity;

    return 0;
}

static
void
copy_cstr(
    char*       dest
,   size_t      dest_cc
,   char const* src
)
{
    size_t n;

    if (0 == dest_cc)
    {
        return;
    }

    n = strlen(src);

    if (n >= dest_cc)
    {
        n = dest_cc - 1;
    }

    memcpy(dest, src, n);
    dest[n] = '\0';
}

static
int
locale_list_contains_id(
    locale_list_t const*    list
,   char const*             id
)
{
    size_t i;

    for (i = 0; i != list->count; ++i)
    {
        if (0 == strcmp(list->entries[i].id, id))
        {
            return 1;
        }
    }

    return 0;
}

static
int
locale_list_add(
    locale_list_t*  list
,   char const*     id
,   char const*     name
)
{
    locale_entry_t* entry;

    if (NULL == id || '\0' == *id)
    {
        return 0;
    }

    if (locale_list_contains_id(list, id))
    {
        return 0;
    }

    if (0 != locale_list_reserve(list))
    {
        return -1;
    }

    entry = &list->entries[list->count];

    memset(entry, 0, sizeof(*entry));

    copy_cstr(entry->id, LSLOCALES_ID_CCH, id);

    if (NULL != name && '\0' != *name)
    {
        copy_cstr(entry->name, LSLOCALES_NAME_CCH, name);
    }
    else
    {
        copy_cstr(entry->name, LSLOCALES_NAME_CCH, id);
    }

    ++list->count;

    return 0;
}

static
int
locale_entry_cmp(
    void const* lhs
,   void const* rhs
)
{
    locale_entry_t const* a = (locale_entry_t const*)lhs;
    locale_entry_t const* b = (locale_entry_t const*)rhs;
    int const             by_name = strcmp(a->name, b->name);

    if (0 != by_name)
    {
        return by_name;
    }

    return strcmp(a->id, b->id);
}

static
void
write_entry(
    FILE*                   out
,   locale_entry_t const*   entry
,   int                     flags
)
{
    if (0 != (SISTOOL_LSLOCALES_F_NAMES_ONLY & flags))
    {
        fprintf(out, "%s\n", entry->name);
    }
    else if (0 != (SISTOOL_LSLOCALES_F_CODES_ONLY & flags))
    {
        fprintf(out, "%s\n", entry->id);
    }
    else if (0 == strcmp(entry->id, entry->name))
    {
        fprintf(out, "%s\n", entry->name);
    }
    else
    {
        fprintf(out, "%s\t%s\n", entry->id, entry->name);
    }
}


#if defined(_WIN32)

/* EnumSystemLocalesA has no user-data argument. */
static locale_list_t* s_win_list;

# ifndef LOCALE_SNAME
#  define LOCALE_SNAME                                      0x0000005c
# endif

static
BOOL
CALLBACK
enum_system_locales_proc(
    LPSTR   locale_string
)
{
    char        name[LSLOCALES_NAME_CCH];
    LCID        lcid;
    char*       end = NULL;
    int         n;

    if (NULL == s_win_list || 0 != s_win_list->err)
    {
        return FALSE;
    }

    if (NULL == locale_string || '\0' == *locale_string)
    {
        return TRUE;
    }

    lcid = (LCID)strtoul(locale_string, &end, 16);

    if (end == locale_string)
    {
        return TRUE;
    }

    name[0] = '\0';

    n = GetLocaleInfoA(lcid, LOCALE_SNAME, &name[0], LSLOCALES_NAME_CCH);

    if (n <= 1)
    {
        n = GetLocaleInfoA(lcid, LOCALE_SLANGUAGE, &name[0], LSLOCALES_NAME_CCH);
    }

    if (n > 0)
    {
        /* GetLocaleInfo includes the terminating NUL in the count. */
        name[LSLOCALES_NAME_CCH - 1] = '\0';
    }

    if (0 != locale_list_add(s_win_list, locale_string, name))
    {
        return FALSE;
    }

    return TRUE;
}

static
int
collect_windows_locales(
    locale_list_t*  list
,   int             flags
)
{
    DWORD const dwFlags = (0 != (SISTOOL_LSLOCALES_F_SUPPORTED & flags))
        ? LCID_SUPPORTED
        : LCID_INSTALLED
        ;

    s_win_list = list;

    if (!EnumSystemLocalesA(enum_system_locales_proc, dwFlags))
    {
        DWORD const dw = GetLastError();

        s_win_list = NULL;

        if (0 != list->err)
        {
            return list->err;
        }

        return (0 != dw) ? (int)dw : 1;
    }

    s_win_list = NULL;

    return list->err;
}

#else /* ? _WIN32 */

static
int
add_dir_entries(
    locale_list_t*  list
,   char const*     dir_path
)
{
    DIR*            dir;
    struct dirent*  ent;

    dir = opendir(dir_path);

    if (NULL == dir)
    {
        return 0;
    }

    while (NULL != (ent = readdir(dir)))
    {
        char            path[512];
        struct stat     st;
        int             n;

        if ('.' == ent->d_name[0])
        {
            continue;
        }

        if (0 == strcmp(ent->d_name, "locale-archive"))
        {
            continue;
        }

        n = snprintf(path, sizeof(path), "%s/%s", dir_path, ent->d_name);

        if (n < 0 || (size_t)n >= sizeof(path))
        {
            continue;
        }

        if (0 != stat(path, &st))
        {
            continue;
        }

        if (!S_ISDIR(st.st_mode))
        {
            continue;
        }

        if (0 != locale_list_add(list, ent->d_name, ent->d_name))
        {
            closedir(dir);

            return -1;
        }
    }

    closedir(dir);

    return 0;
}

static
int
collect_locale_a(
    locale_list_t*  list
)
{
    FILE*   fp;
    char    line[LSLOCALES_NAME_CCH];
    int     rc;

    fp = popen("locale -a 2>/dev/null", "r");

    if (NULL == fp)
    {
        return 0;
    }

    while (NULL != fgets(line, (int)sizeof(line), fp))
    {
        size_t len = strlen(line);

        while (len != 0 && ('\n' == line[len - 1] || '\r' == line[len - 1]))
        {
            line[--len] = '\0';
        }

        if (0 == len)
        {
            continue;
        }

        if (0 != locale_list_add(list, line, line))
        {
            pclose(fp);

            return -1;
        }
    }

    rc = pclose(fp);

    ((void)rc);

    return 0;
}

static
int
collect_posix_locales(
    locale_list_t*  list
)
{
    static char const* const dirs[] = {

        "/usr/lib/locale",
        "/usr/local/lib/locale",
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
        "/usr/share/locale",
        "/usr/local/share/locale",
#endif
        NULL,
    };
    size_t i;

    if (0 != collect_locale_a(list))
    {
        return -1;
    }

    if (0 == list->count)
    {
        for (i = 0; NULL != dirs[i]; ++i)
        {
            if (0 != add_dir_entries(list, dirs[i]))
            {
                return -1;
            }
        }
    }

    if (0 != locale_list_add(list, "C", "C"))
    {
        return -1;
    }

    if (0 != locale_list_add(list, "POSIX", "POSIX"))
    {
        return -1;
    }

    return list->err;
}

#endif /* _WIN32 */


/* /////////////////////////////////////////////////////////////////////////
 * API
 */

int
sistool_lslocales(
    FILE*   out
,   int     flags
)
{
    locale_list_t   list;
    int             rc;
    size_t          i;

    assert(NULL != out);

    locale_list_init(&list);

#if defined(_WIN32)
    rc = collect_windows_locales(&list, flags);
#else /* ? _WIN32 */
    rc = collect_posix_locales(&list);
#endif /* _WIN32 */

    if (0 != rc)
    {
        locale_list_destroy(&list);

        return rc;
    }

    if (0 != list.count &&
        0 == (SISTOOL_LSLOCALES_F_UNSORTED & flags))
    {
        qsort(list.entries, list.count, sizeof(locale_entry_t), locale_entry_cmp);
    }

    for (i = 0; i != list.count; ++i)
    {
        write_entry(out, &list.entries[i], flags);
    }

    locale_list_destroy(&list);

    return 0;
}


/* ///////////////////////////// end of file //////////////////////////// */

