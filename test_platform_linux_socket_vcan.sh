#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

set -m # Enable Job Control

mkdir $DIR/logs

$DIR/build.sh

$DIR/vcanEnable.sh vcan0

$DIR/bin/test_platform_linux_socket vcan0 0 &

$DIR/bin/test_platform_linux_socket vcan0 1 &

sleep 0.1

cansend vcan0 00000101#0000000000000000

sleep 0.1

timestamp() {
  date +"%T"
}


function finish {
    pkill test_platform_r
    pkill test_platform_l

    # Wait for all parallel jobs to finish
    while [ 1 ]; do fg 2> /dev/null; [ $? == 1 ] && break; done
}
trap finish EXIT

$DIR/bin/test_platform_read_linux_socket vcan0 > $DIR/logs/test_platform_read_linux_socket_$(timestamp).txt




