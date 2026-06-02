#!/usr/bin/env bash
set -u

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT_DIR/minishell"
TMP_DIR="$ROOT_DIR/tests/tmp"
STDIN_FILE="$TMP_DIR/heredoc_input"
TARGET_FILE="$TMP_DIR/fd_redir"
FAILURES=0

mkdir -p "$TMP_DIR"
printf 'heredoc-body\nEOF\n' >"$STDIN_FILE"

"$BIN" -c "echo redir > $TARGET_FILE; cat < $TARGET_FILE | wc -c >/dev/null; cat << EOF >/dev/null; sleep 2" \
	<"$STDIN_FILE" >/dev/null 2>"$TMP_DIR/fd_stderr" &
pid=$!
sleep 0.5

PARENT_FDS=" $(find "/proc/$$/fd" -mindepth 1 -maxdepth 1 -printf '%f ' | sort -n) "

if [[ ! -d "/proc/$pid/fd" ]]; then
	printf 'not ok - fd check process exited early\n'
	sed 's/^/    /' "$TMP_DIR/fd_stderr"
	FAILURES=$((FAILURES + 1))
else
	extra_fds=""
	all_fds="$(find "/proc/$pid/fd" -mindepth 1 -maxdepth 1 -printf '%f\n' | awk '$1 > 2' | sort -n)"
	while IFS= read -r fd; do
		[[ -z "$fd" ]] && continue
		# Check if it was inherited from the parent environment
		if [[ $PARENT_FDS =~ " $fd " ]]; then
			continue
		fi
		extra_fds="${extra_fds}${fd}\n"
	done <<< "$all_fds"
	extra_fds="$(printf "%b" "$extra_fds" | sed '/^$/d')"

	if [[ -n "$extra_fds" ]]; then
		printf 'not ok - fd check found leaked descriptors\n'
		while IFS= read -r fd; do
			[[ -z "$fd" ]] && continue
			printf '  fd %s -> %s\n' "$fd" "$(readlink "/proc/$pid/fd/$fd")"
		done <<< "$extra_fds"
		FAILURES=$((FAILURES + 1))
	else
		printf 'ok - fd check\n'
	fi
fi

wait "$pid"
status=$?
rm -rf "$TMP_DIR"

if [[ "$status" -ne 0 ]]; then
	printf 'not ok - fd check command status %s\n' "$status"
	FAILURES=$((FAILURES + 1))
fi

if [[ "$FAILURES" -ne 0 ]]; then
	exit 1
fi
