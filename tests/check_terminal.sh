#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
TMP_DIR="$ROOT_DIR/tests/tmp"

if ! command -v script >/dev/null 2>&1; then
	printf 'skip - terminal check requires script(1)\n'
	exit 0
fi

mkdir -p "$TMP_DIR"

script -q -e -c "cd '$ROOT_DIR' && before=\$(stty -g) && printf 'exit\n' | ./minishell >/dev/tty 2>/dev/tty && after=\$(stty -g) && test \"\$before\" = \"\$after\"" /dev/null >"$TMP_DIR/terminal.out" 2>&1
status=$?
rm -rf "$TMP_DIR"

if [[ "$status" -eq 0 ]]; then
	printf 'ok - terminal state restored\n'
else
	printf 'not ok - terminal state changed\n'
fi

exit "$status"
