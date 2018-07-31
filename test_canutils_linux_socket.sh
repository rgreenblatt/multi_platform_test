#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

set -m # Enable Job Control

mkdir $DIR/logs

timestamp() {
  date +"%T"
}

$DIR/build.sh

$DIR/canableStart.sh can0

$DIR/bin/test_canutils_linux_socket can0 0  > $DIR/logs/test_canutils1_linux_socket_$(timestamp).txt &

$DIR/canableStart.sh can1

$DIR/bin/test_canutils_linux_socket can1 1  > $DIR/logs/test_canutils2_linux_socket_$(timestamp).txt &

sleep 0.1

cansend can0 00000101#0000000000000000

sleep 0.1

function finish {
    pkill test_platform_r
    pkill test_platform_l

    # Wait for all parallel jobs to finish
    while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done
}
trap finish EXIT

$DIR/bin/test_platform_read_linux_socket vcan0 0 > $DIR/logs/test_platform_read_linux_socket_canutils_$(timestamp).txt
