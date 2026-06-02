#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/minishell"
TMP_DIR="$ROOT_DIR/tests/tmp"
FAILURES=0

if ! command -v valgrind >/dev/null 2>&1; then
	printf 'skip - valgrind is not installed\n'
	exit 0
fi

mkdir -p "$TMP_DIR"

run_memcheck() {
	local name="$1"
	local command="$2"
	local expected_status="${3:-0}"
	local log_file="$TMP_DIR/valgrind.log"
	local stdout_file="$TMP_DIR/valgrind.stdout"
	local stderr_file="$TMP_DIR/valgrind.stderr"
	local status

	valgrind \
		--quiet \
		--leak-check=full \
		--show-leak-kinds=definite,indirect \
		--errors-for-leak-kinds=definite,indirect \
		--error-exitcode=99 \
		--log-file="$log_file" \
		"$BIN" -c "$command" >"$stdout_file" 2>"$stderr_file"
	status=$?
	if [[ "$status" -ne "$expected_status" || -s "$log_file" ]]; then
		printf 'not ok - valgrind %s\n' "$name"
		printf '  command: %s\n' "$command"
		printf '  expected status: %s\n' "$expected_status"
		printf '  actual status:   %s\n' "$status"
		printf '  stdout:\n'
		sed 's/^/    /' "$stdout_file"
		printf '  stderr:\n'
		sed 's/^/    /' "$stderr_file"
		printf '  valgrind:\n'
		sed 's/^/    /' "$log_file"
		FAILURES=$((FAILURES + 1))
	else
		printf 'ok - valgrind %s\n' "$name"
	fi
}

run_stdin_memcheck() {
	local name="$1"
	local command="$2"
	local stdin_data="$3"
	local log_file="$TMP_DIR/valgrind.log"
	local stdout_file="$TMP_DIR/valgrind.stdout"
	local stderr_file="$TMP_DIR/valgrind.stderr"
	local status

	printf '%s' "$stdin_data" | valgrind \
		--quiet \
		--leak-check=full \
		--show-leak-kinds=definite,indirect \
		--errors-for-leak-kinds=definite,indirect \
		--error-exitcode=99 \
		--log-file="$log_file" \
		"$BIN" -c "$command" >"$stdout_file" 2>"$stderr_file"
	status=$?
	if [[ "$status" -ne 0 || -s "$log_file" ]]; then
		printf 'not ok - valgrind %s\n' "$name"
		printf '  command: %s\n' "$command"
		printf '  status: %s\n' "$status"
		printf '  stdout:\n'
		sed 's/^/    /' "$stdout_file"
		printf '  stderr:\n'
		sed 's/^/    /' "$stderr_file"
		printf '  valgrind:\n'
		sed 's/^/    /' "$log_file"
		FAILURES=$((FAILURES + 1))
	else
		printf 'ok - valgrind %s\n' "$name"
	fi
}

run_memcheck "builtins and expansion" "export FOO=bar; echo \$FOO; unset FOO; echo \$FOO; false; echo \$?"
run_memcheck "quotes and redirection" "echo \"hello world\" > $TMP_DIR/memcheck_redir; echo done"
run_memcheck "syntax error cleanup" "echo > >" 2
run_memcheck "pipeline" "echo hello | wc -c"
run_stdin_memcheck "heredoc" "cat << EOF" $'hello $USER\nEOF\n'

rm -rf "$TMP_DIR"

if [[ "$FAILURES" -ne 0 ]]; then
	exit 1
fi
