#!/bin/sh
set -eu

if [ "$#" -lt 1 ]; then
  echo "Usage: build_kenlm_model.sh model.arpa [output.binary] [extra build_binary args...]" >&2
  exit 1
fi

arpa_file=$1
shift

if [ "$#" -gt 0 ] && [ "${1#-}" = "$1" ]; then
  output_file=$1
  shift
else
  output_file="${arpa_file%.*}.binary"
fi

if [ ! -f "$arpa_file" ]; then
  echo "ARPA file not found: $arpa_file" >&2
  exit 1
fi

if [ -z "${KENLM_ROOT:-}" ] && [ -f "$(dirname "$0")/../../kenlm/CMakeLists.txt" ]; then
  KENLM_ROOT="$(cd "$(dirname "$0")/../../kenlm" && pwd)"
fi

build_binary_exe="${KENLM_ROOT:+$KENLM_ROOT/bin/build_binary}"
if [ -n "${build_binary_exe:-}" ] && [ ! -x "$build_binary_exe" ]; then
  build_binary_exe="${KENLM_ROOT}/build-clang64/bin/build_binary"
fi
if [ -n "${build_binary_exe:-}" ] && [ ! -x "$build_binary_exe" ]; then
  build_binary_exe="${KENLM_ROOT}/build-msvc/bin/Release/build_binary.exe"
fi
if [ -n "${build_binary_exe:-}" ] && [ ! -x "$build_binary_exe" ]; then
  build_binary_exe="${KENLM_ROOT}/build/bin/build_binary"
fi
if [ -z "${build_binary_exe:-}" ] || [ ! -x "$build_binary_exe" ]; then
  build_binary_exe=build_binary
fi

echo "Using build_binary: $build_binary_exe"
echo "Input ARPA: $arpa_file"
echo "Output binary: $output_file"

"$build_binary_exe" "$@" "$arpa_file" "$output_file"
