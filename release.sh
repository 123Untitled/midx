#!/bin/sh

# set strict modes
set -o nounset 
#-o noclobber -o noglob
#-o errexit
#-o xtrace
#set -o


# root directory
readonly cwd_dir=$(cd "$(dirname "$0")" && pwd)

# project
readonly project='midx'

# executable
readonly executable="$cwd_dir"'/'"$project"

# sources directory
readonly src_dir="$cwd_dir"'/sources'

# include directory
readonly inc_dir="$cwd_dir"'/include'

# fallback compiler
readonly compiler='clang++'

# compiler
readonly cxx="${CXX:-$compiler}"


# detect operating system
readonly os="$(uname -s)"

case $(uname -s) in
	Darwin)
		frameworks='-framework CoreMIDI -framework CoreAudio -framework CoreFoundation'
		;;
	Linux)
		frameworks=''
		;;
	*)
		printf 'Unsupported OS: %s\n' "$os" >&2
		exit 1
		;;
esac


# find all .cpp files
set -- $(find "$src_dir" '-type' 'f' '-name' '*.cpp')


if [ $# -eq 0 ]; then
	printf 'No source files found\n' >&2
	exit 1
fi



animation() {

	i=0

	while kill -0 "$1" 2> '/dev/null'; do

		case $i in
			0) c='⠋' ;;
			1) c='⠙' ;;
			2) c='⠹' ;;
			3) c='⠸' ;;
			4) c='⠼' ;;
			5) c='⠴' ;;
			6) c='⠦' ;;
			7) c='⠧' ;;
			8) c='⠇' ;;
			9) c='⠏' ;;
		esac

		# increment spinner
		i=$(((i + 1) % 10))
		printf "\r\033[33m%s\033[0m Building %s" "$c" "$project"
		sleep '0.1'
	done

	wait "$1"
}

readonly log='./release.log'

readonly logo='
                ░██       ░██
                          ░██
░█████████████  ░██ ░████████ ░██    ░██
░██   ░██   ░██ ░██░██    ░██  ░██  ░██ 
░██   ░██   ░██ ░██░██    ░██   ░█████  
░██   ░██   ░██ ░██░██   ░███  ░██  ░██ 
░██   ░██   ░██ ░██ ░█████░██ ░██    ░██'

printf '\033[34m%s\033[0m\n\n' "$logo"




# compile
"$cxx" '-std=c++23' '-O3' '-flto' '-DNDEBUG' '-march=native' \
	   '-Wall' '-Wextra' '-Werror' '-Wpedantic' '-Weffc++' \
	   '-Wno-unused' '-Wno-unused-variable' '-Wno-unused-parameter' \
	   '-fno-rtti' '-Winline' \
	   '-fdiagnostics-color=never' \
	   '-Wno-unused-function' '-Wno-unused-private-field' '-Wno-unused-local-typedef' \
	   '-ferror-limit=1' \
	   '-I'"$inc_dir" \
	   "$@" -o "$executable" '-flto' $frameworks \
	   > "$log" 2>&1 &

readonly pid=$!


desinit() {

	# back to normal
	printf '\033[?25h'
	stty echo
	\rm -f "$log"
}

cleanup() {

	kill "$pid" 2>/dev/null || true

	# back to normal
	desinit

	exit 130
}

init() {

	# init terminal settings
	printf '\033[?25l'
	stty -echo

	# install trap
	trap cleanup INT QUIT TERM ABRT ILL TRAP FPE BUS SEGV SYS KILL STOP
}



init


animation $pid
readonly code=$?


if [ $code -ne 0 ]; then
	printf '\r\033[0K\033[31mx\033[0m Build failed\n\n' >&2
	\cat "$log" >&2
	desinit
	# show cursor
	exit $code
fi

printf '\r\033[0K\033[32m✓\033[0m Build succeeded: %s\n\n' "$executable"

desinit

cp "$executable" ~/mybin/
