#!/bin/bash

export SDL_VIDEODRIVER=dummy
readonly DOSBOX=${DOSBOX:-"$(git rev-parse --show-toplevel)/src/dosbox"}

expecting_output () {
	echo "$1" > t000-dirb-out.txt
	unix2dos t000-dirb-out.txt
}

set -xe

rm -rfv drive_c/*
mkdir drive_c

$DOSBOX -conf t000-dirb.conf

expecting_output \
'HELLO.TXT
OUT.TXT'

diff -u t000-dirb-out.txt drive_c/OUT.TXT

rm -rfv drive_c
rm t000-dirb-out.txt
