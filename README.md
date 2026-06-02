# Minishell

C로 구현한 Unix 계열 셸입니다.

사용자 입력을 토큰화하고, 파서가 AST 형태의 command tree를 만든 뒤,
확장, redirection, fork/execve, wait/status 갱신까지 직접 처리합니다.
interactive 모드와 테스트 가능한 non-interactive `-c` 모드를 모두 지원합니다.

## 주요 기능

- `readline` 기반 command history
- `PATH` 검색, 상대 경로, 절대 경로 실행
- built-in 명령어
  - `echo -n`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- pipe: `|`
- 조건 실행: `&&`, `||`
- subshell: `( ... )`
- 현재 디렉터리 기준 wildcard expansion
- single quote, double quote 처리
- redirection: `<`, `>`, `<<`, `>>`
- environment variable expansion
- heredoc
- `Ctrl-C`, `Ctrl-D`, `Ctrl-\` signal 처리
- `./minishell -c 'command'` 형태의 non-interactive 실행

## 구조

```text
input
-> lexer
-> parser
-> AST command tree
-> expansion
-> redirection setup
-> command execution
-> wait/status update
```

파싱 단계는 lexer와 parser로 분리되어 있습니다. lexer는 raw input을 word와
meta token stream으로 변환하고, parser는 token stream을 소비해 `t_cmd` 기반
AST command tree를 만듭니다. 실행부는 이 tree를 순회하면서 simple command,
pipe, conditional, subshell을 처리합니다.

runtime 상태는 `t_shell` context에 모여 있습니다. exit status, interactive
mode, environment store가 한 곳에서 관리합니다.

## 디렉터리 구조

```text
.
├── include/
│   ├── cmd/          # command tree와 built-in 인터페이스
│   ├── parse/        # lexer/parser/expansion 인터페이스
│   └── utils/        # 문자열, 파일, signal, shell context 유틸리티
├── src/
│   ├── cmd/
│   │   ├── builtin/   # echo, cd, pwd, export, unset, env, exit
│   │   ├── execution/ # fork, execve, wait, PATH lookup
│   │   └── structure/ # command node 생성과 실행 순회
│   ├── parse/
│   │   ├── expansion/ # env expansion과 wildcard expansion
│   │   ├── lexer.c    # raw input -> token stream
│   │   ├── quote.c    # quote state helper
│   │   └── parser/    # token stream -> AST command tree
│   ├── utils/
│   │   ├── env.c      # resizable environment store
│   │   ├── lst/       # linked-list helper
│   │   └── str/       # string helper
│   └── main.c
├── tests/            # regression, fd, terminal, valgrind check
└── Makefile
```

## 요구 사항

- C compiler
- `libreadline-dev`

## 빌드

```sh
make
```

debug/sanitizer build:

```sh
make debug
make asan
```

## 실행

interactive shell:

```sh
./minishell
```

non-interactive command:

```sh
./minishell -c 'echo hello | wc -c'
```
