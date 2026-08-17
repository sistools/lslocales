#! /bin/bash

ScriptPath=$0
Dir=$(cd "$(dirname "$ScriptPath")" && pwd)
Basename=$(basename "$ScriptPath")
CMakeDir=${SIS_CMAKE_BUILD_DIR:-$Dir/_build}

if command -v tput > /dev/null; then
  SisClr_Red=${FG_RED:-$(tput setaf 1)}
  SisClr_None=${FD_NONE:-$(tput sgr0)}
else
  SisClr_Red=
  SisClr_None=
fi

case ${1:-} in
  "")
    ;;
  --help)
    [ -f "$Dir/.sis/script_info_lines.txt" ] && cat "$Dir/.sis/script_info_lines.txt"
    printf 'Generates HTML API documentation from public headers via Doxygen\n'
    exit 0
    ;;
  *)
    >&2 printf '%s%s: unrecognised argument %s%s; use --help for usage\n' "$SisClr_Red" "$ScriptPath" "$1" "$SisClr_None"
    exit 1
    ;;
esac

cd "$Dir" || exit 1
command -v doxygen >/dev/null 2>&1 || {
  >&2 printf '%s%s: doxygen not found on PATH%s\n' "$SisClr_Red" "$ScriptPath" "$SisClr_None"
  exit 1
}

mkdir -p "${CMakeDir}/doxygen"
{
  cat Doxyfile
  printf '\n# Output directory (overridden by %s)\n' "$Basename"
  printf 'OUTPUT_DIRECTORY = %s/doxygen\n' "$CMakeDir"
} | doxygen -

printf 'API documentation written to %s/doxygen/html/index.html\n' "$CMakeDir"
