#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/minishell"
TMP_DIR="$ROOT_DIR/tests/tmp"
FAILURES=0

mkdir -p "$TMP_DIR"

run_case() {
	local name="$1"
	local command="$2"
	local expected_stdout="$3"
	local expected_status="${4:-0}"
	local expected_stderr="${5:-}"
	local stdout_file="$TMP_DIR/stdout"
	local stderr_file="$TMP_DIR/stderr"
	local status
	local actual_stdout
	local actual_stderr

	"$BIN" -c "$command" >"$stdout_file" 2>"$stderr_file"
	status=$?
	actual_stdout="$(cat "$stdout_file")"
	actual_stderr="$(cat "$stderr_file")"
	if [[ "$actual_stdout" != "$expected_stdout" || \
		"$actual_stderr" != "$expected_stderr" || \
		"$status" != "$expected_status" ]]; then
		printf 'not ok - %s\n' "$name"
		printf '  command: %s\n' "$command"
		printf '  expected status: %s\n' "$expected_status"
		printf '  actual status:   %s\n' "$status"
		printf '  expected stdout: [%s]\n' "$expected_stdout"
		printf '  actual stdout:   [%s]\n' "$actual_stdout"
		printf '  expected stderr: [%s]\n' "$expected_stderr"
		printf '  actual stderr:   [%s]\n' "$actual_stderr"
		FAILURES=$((FAILURES + 1))
	else
		printf 'ok - %s\n' "$name"
	fi
}

run_stdin_case() {
	local name="$1"
	local command="$2"
	local stdin_data="$3"
	local expected_stdout="$4"
	local stdout_file="$TMP_DIR/stdout"
	local stderr_file="$TMP_DIR/stderr"
	local status
	local actual_stdout
	local actual_stderr

	printf '%s' "$stdin_data" | "$BIN" -c "$command" >"$stdout_file" 2>"$stderr_file"
	status=$?
	actual_stdout="$(cat "$stdout_file")"
	actual_stderr="$(cat "$stderr_file")"
	if [[ "$actual_stdout" != "$expected_stdout" || \
		"$actual_stderr" != "" || "$status" != "0" ]]; then
		printf 'not ok - %s\n' "$name"
		printf '  command: %s\n' "$command"
		printf '  expected stdout: [%s]\n' "$expected_stdout"
		printf '  actual stdout:   [%s]\n' "$actual_stdout"
		printf '  expected stderr: []\n'
		printf '  actual stderr:   [%s]\n' "$actual_stderr"
		FAILURES=$((FAILURES + 1))
	else
		printf 'ok - %s\n' "$name"
	fi
}

run_case "external command" "echo hello" "hello"
run_case "environment survives sequence" "export FOO=bar; echo \$FOO; unset FOO; echo \$FOO" $'bar'
run_case "pipe" "echo hello | wc -c" "6"
run_case "conditionals" "false || echo recovered; true && echo ok" $'recovered\nok'
run_case "exit status expansion" "false; echo \$?" "1"
run_case "quotes" "echo \"a b\"; echo '\$USER'" $'a b\n$USER'
run_case "wildcard" "echo Make*" "Makefile"
run_stdin_case "heredoc expansion" "cat << EOF" $'hello $USER\nEOF\n' "hello ${USER:-}"
run_stdin_case "quoted heredoc delimiter" "cat << 'EOF'" $'hello $USER\nEOF\n' 'hello $USER'

rm -f "$TMP_DIR/redir"
"$BIN" -c "echo redir-ok > $TMP_DIR/redir; cat < $TMP_DIR/redir" >"$TMP_DIR/stdout" 2>"$TMP_DIR/stderr"
status=$?
actual_stdout="$(cat "$TMP_DIR/stdout")"
actual_stderr="$(cat "$TMP_DIR/stderr")"
if [[ "$actual_stdout" != "redir-ok" || "$actual_stderr" != "" || \
	"$status" != "0" ]]; then
	printf 'not ok - redirection\n'
	printf '  expected stdout: [redir-ok]\n'
	printf '  actual stdout:   [%s]\n' "$actual_stdout"
	printf '  expected stderr: []\n'
	printf '  actual stderr:   [%s]\n' "$actual_stderr"
	FAILURES=$((FAILURES + 1))
else
	printf 'ok - redirection\n'
fi

rm -rf "$TMP_DIR"

if [[ "$FAILURES" -ne 0 ]]; then
	printf '%s regression test(s) failed\n' "$FAILURES"
	exit 1
fi

printf 'all regression tests passed\n'
