#!/usr/bin/env -S zsh --no-rcs --no-globalrcs --pipefail
#--errexit


# -- C O L O R S --------------------------------------------------------------

declare -rg success='\x1b[32m'
declare -rg   error='\x1b[31m'
declare -rg warning='\x1b[33m'
declare -rg    info='\x1b[34m'
declare -rg     dim='\x1b[90m'
declare -rg   reset='\x1b[0m'


# -- L O G O ------------------------------------------------------------------

echo $warning \
	'   ▁▁▁▁▁▁▁▁  ▁▁▁▁▁▁▁▁  ▁▁▁▁ ▁▁▁  ▁▁▁▁▁▁▁▁ \n' \
	'  ╱        ╲╱        ╲╱    ╱   ╲╱        ╲\n' \
	' ╱         ╱         ╱         ╱         ╱\n' \
	'╱         ╱         ╱        ▁╱       ▁▁╱ \n' \
	'╲▁▁╱▁▁╱▁▁╱╲▁▁▁╱▁▁▁▁╱╲▁▁▁▁╱▁▁▁╱╲▁▁▁▁▁▁▁▁╱  \n' \
	$reset


# -- O P E R A T I N G  S Y S T E M -------------------------------------------

# get operating system
declare -rg os=$(uname -s)


# -- T H I S  S C R I P T -----------------------------------------------------

# get script absolute directory path
declare -rg cwd_dir=${${0%/*}:a}

# get script absolute path
declare -rg script=${0:a}


# -- T A R G E T S ------------------------------------------------------------

# project name
declare -rg project='midilang'

# main executable
declare -rg executable=$cwd_dir'/'$project

# compile commands database
declare -rg compile_db=$cwd_dir'/compile_commands.json'


# -- D I R E C T O R I E S ----------------------------------------------------

# source directory
declare -rg src_dir=$cwd_dir'/sources'

# include directory
declare -rg inc_dir=$cwd_dir'/include'

# git directory
declare -rg git_dir=$cwd_dir'/.git'


# -- F I L E S ----------------------------------------------------------------

# source files
declare -rg srcs=($src_dir'/'**'/'*'.cpp'(.N))

# object files
declare -rg objs=(${srcs/%.cpp/.o})

# dependency files
declare -rg deps=(${objs/%.o/.d})

# log files
declare -rg logs=(${srcs/%.cpp/.log})


# -- O S  D E P E N D E N C I E S ---------------------------------------------

# linux dependencies
if [[ $os =~ 'Linux' ]]; then
	declare -rg os_dependencies=(
					#'-lasound'
				)
	declare -rg max_jobs=$(nproc)

# macos dependencies
elif [[ $os =~ 'Darwin' ]]; then
	declare -rg os_dependencies=('-framework' 'CoreMIDI' '-framework' 'CoreAudio' '-framework' 'CoreFoundation')
	declare -rg max_jobs=$(sysctl -n hw.ncpu)
fi


# -- C O M P I L E R  S E T T I N G S -----------------------------------------

declare -rg compiler='clang++'

# compiler
if command -v ccache > '/dev/null'; then
	declare -rg cxx=('ccache' $compiler)
else
	declare -rg cxx=($compiler)
fi

# standard
declare -rg std='-std=c++2a'
#'-std=c++2c'

# cxx flags
declare -rg cxxflags=($std
					  '-O0'
					  '-g3'
					  #'-fsanitize=thread'
					  '-fsanitize=address'
					  '-gdwarf-4'
					  '-Wall' '-Wextra' '-Werror' '-Wpedantic' '-Weffc++'
					  '-ferror-limit=1'
					  '-fno-rtti' '-Winline'
					  '-Wno-unused' '-Wno-unused-variable' '-Wno-unused-parameter'
					  '-Wno-unused-function' '-Wno-unused-private-field' '-Wno-unused-local-typedef'
					  #'-Wconversion' '-Wsign-conversion' '-Wfloat-conversion' '-Wnarrowing'
					  '-fdiagnostics-color=always'
					  '-fno-diagnostics-show-note-include-stack'
					  '-fdiagnostics-show-location=once'
					  '-fdiagnostics-show-template-tree'
					  '-Wshadow'

					  #'-Wno-gnu-designator'
					  #'-Wno-c99-designator'
					  #'-Wno-initializer-overrides'
					  #'-Wno-gnu-anonymous-struct'
					  #'-Wno-nested-anon-types'
					  '-I'$inc_dir
				)

# linker flags
declare -rg ldflags=($os_dependencies
					 #'-fsanitize=thread'
					 '-fsanitize=address'
)


# -- F U N C T I O N S --------------------------------------------------------

# function to check required tools
function _check_tools() {

	# required tools
	local -r required=('uname' 'git' 'rm' 'mkdir' 'wait' 'clang++')

	# optional tools
	local -r optional=('ccache')

	# loop over required tools
	for tool in $required; do

		# check if tool is available
		if ! command -v $tool > '/dev/null'; then
			echo 'required tool' $error$tool$reset 'not found.'
			exit 1
		fi
	done

	# loop over optional tools
	for tool in $optional; do

		# check if tool is available
		if ! command -v $tool > '/dev/null'; then
			echo 'optional tool' $warning$tool$reset 'not found.'
		fi
	done
}


# -- C O M P I L E  D A T A B A S E -------------------------------------------

# generate compile database
function _generate_compile_db() {

	local content='[\n'

	for file in $srcs; do

		# current directory
		content+='\t{\n\t\t"directory": "'$cwd_dir'",'

		# source file
		content+='\n\t\t"file": "'$file'",'

		# output file
		content+='\n\t\t"output": "'${file%.cpp}'.o",'

		# arguments
		content+='\n\t\t"arguments": [\n\t\t\t"'$cxx'",\n'

		# cxx flags
		for flag in $cxxflags; do
			content+='\t\t\t"'$flag'",\n'
		done

		# source file
		content+='\t\t\t"-c",\n\t\t\t"'$file'",\n'

		# output file
		content+='\t\t\t"-o",\n\t\t\t"'${file%.cpp}'.o"\n\t\t]\n\t},\n'
	done

	# erase last comma with newline
	content[-3]='\'
	content[-2]='n'
	content[-1]=']'

	# write to compile db
	if ! echo $content > $compile_db; then
		echo 'error while generating' $error'compile_commands.json'$reset
		exit 1
	fi

	# print success
	print $success'[+]'$reset ${compile_db:t}
}


# compile db
function _compile_database() {

	# check compile db exists and is up to date
	if [[ ! -f $compile_db ]] || [[ $compile_db -ot $script ]]; then
		_generate_compile_db
		return
	fi

	# loop over source files
	for src in $srcs; do

		# check if src is newer than compile db
		if [[ $src -nt $compile_db ]]; then
			_generate_compile_db
			break
		fi

	done
}



# -- C O M P I L A T I O N ----------------------------------------------------

function _check_dependency {
	# check if object or dependency file is missing
	if [[ ! -f $1 ]] || [[ ! -f $2 ]] || [[ $script -nt $1 ]]; then
		return 0
	fi
	# loop over words in dependency file
	for word in ${=$(<$2)}; do
		# check if word is not target or escape
		if [[ $word != '\' ]] && [[ $word != *':' ]]; then
			# check if dependency is missing
			[[ $word -nt $1 ]] && return 0
		fi
	done
	return 1
}


function _handle_compilation {

	# $1 source file
	# $2 object file
	# $3 dependency file
	# $4 log file

	# compile source file
	$cxx $cxxflags -MT $2 -MMD -MF $3 -c $1 -o $2 &> $4
	#2>&1

	# check if compilation failed
	if [[ $? -ne 0 ]]; then
		echo '\r\x1b[2K'$error'[x]'$reset ${1:t}
		exit 1
	fi

	echo -n '\r\x1b[2K'$info'[✓]'$reset ${1:t}
	exit 0
}

function _wait_processes {

	# loop over pids
	for pid in $@; do
		# wait for pid
		wait $pid
		# check if compilation failed
		if [[ $? -ne 0 ]]; then
			wait
			# cat all log files
			#cat $logs
			cat $src_dir'/'**'/'*.'log'
			exit 1
		fi
	done
}


function _compile {

	# number of compiled files
	local count=0

	# array of pids
	local pids=()

	# loop over source files
	for srcf in $srcs; do

		# add object file extension
		local objf=${srcf%.cpp}'.o'

		# add dependency file extension
		local depf=${srcf%.cpp}'.d'

		# add log file extension
		local logf=${srcf%.cpp}'.log'

		# check if source file is modified
		if ! _check_dependency $objf $depf; then
			continue
		fi

		if [[ ${#pids[@]} -eq $max_jobs ]]; then
			_wait_processes $pids
			pids=()
		fi

		count=$((count + 1))

		_handle_compilation $srcf $objf $depf $logf &

		pids+=($!)
	done

	# wait for remaining pids
	_wait_processes $pids

	if [[ $count -ne 0 ]]; then
		echo '\r\x1b[2K'$success'[+]'$reset $count 'cxx compiled.'
	fi

	# remove all log files
	rm -f $logs
}


# -- L I N K  F U N C T I O N S -----------------------------------------------

function _link {

	# link object files
	if $cxx $objs '-o' $executable $ldflags; then
		echo $success'[+]'$reset 'linked' ${executable:t}
	else
		echo $error'[x]'$reset 'linking failed'
		exit 1
	fi
}

function _handle_link {

	if [[ ! -f $executable ]]; then
		_link
	fi

	# loop over object files
	for obj in $objs; do

		# check if object file is newer than target
		if [[ $obj -nt $executable ]]; then
			_link
			return
		fi
	done

	# no link required
	echo $success'[>]'$reset ${executable:t} 'is up to date'
}



# build
function _build() {

	# generate compile database
	_compile_database

	# build
	_compile
	_handle_link
}

# clean
function _clean() {

	# clear ccache cache if available
	if command -v ccache > '/dev/null'; then
		ccache '--clear'
	fi

	# remove all build files
	local -r deleted=($(rm -vrf $objs $deps $logs $executable $compile_db '.cache'))

	# get count of deleted files
	local -r count=${#deleted[@]}

	# check if files were deleted
	if [[ $count -ne 0 ]]; then
		echo $info'[x]'$reset $count 'files deleted.'
		return
	fi
}


# -- M A I N ------------------------------------------------------------------


_check_tools

if [[ $# -eq 0 ]]; then
	_build
	exit
fi

# handle arguments
case $1 in

	# run
	run | launch)
		_build
		echo
		cd $cwd_dir'/test' && $executable
		;;

	# clean
	rm | clean | fclean)
		_clean
		;;

	# re
	re | rebuild)
		_clean
		_build
		;;

	# unknown (usage)
	*)
		echo 'usage:' ${script:t} '[run|launch|rm|clean|fclean|re|rebuild|submodules]'
		;;
esac
