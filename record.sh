#!/bin/zsh

local -r target='record.cast'
local -r cmd='./make.sh test'


# check args
if [[ $# -eq 1 ]]; then
	if [[ $1 == 'play' ]]; then
		asciinema play "$target"
		exit 0
	fi
	if [[ $1 == 'gif' ]]; then
		agg --font-family 'JetBrainsMono Nerd Font Mono' --font-size 18 --speed 1.5 "$target" 'record.gif'
		exit 0
	fi
	echo "Unknown argument: $1"
	exit 1
fi

asciinema rec --command "$cmd" --overwrite "$target"

