<h3 align="center">
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="30" width="0px"/><br/>
    <img src="https://42seoul.kr/template/cms/seoul42/images/common/head_logo.png" width="250" alt="Logo"/><br/>
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="70" width="0px"/>
    Minishell Project for <a href="https://42seoul.kr/seoul42/main/view">42 Seoul</a><br/>
    <img src="https://github.com/user-attachments/assets/ddc4a13d-de36-4802-9bd2-50c9376d580b" height="10" width="0px"/>
</h3>

<p align="center">
This repository is for the 42 seoul project <strong>Minishell</strong>.<br />The objective of this project is to create a simple shell.
</p>

## Features

- support Linux, MacOS
- Working History
- Executables based on the PATH variable / relative or an absolute path
- <details>
  <summary>Built In</summary>
  <ul>
    <li>echo with option -n</li>
    <li>cd with only a relative or absolute path</li>
    <li>pwd with no options</li>
    <li>export with no options</li>
    <li>unset with no options</li>
    <li>env with no options or arguments</li>
    <li>exit with no options </li>
  </ul>
  </details>
- Multiple commands with |, ||, && metadata
- Subshell by parenthesis
- Wildcard for current directory
- Quoted sequence (', ")
- redirections (<, >, <<, >>)
- Environment Variables
- Signal Handle (ctrl-C, ctrl-D and ctrl-\)

## Requirements
- libreadline-dev
