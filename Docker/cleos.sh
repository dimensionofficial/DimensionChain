#!/bin/bash

# Usage:
# Go into cmd loop: sudo ./cleon.sh
# Run single cmd:  sudo ./cleon.sh <cleon paramers>

PREFIX="docker-compose exec nodeosd cleon"
if [ -z $1 ] ; then
  while :
  do
    read -e -p "cleon " cmd
    history -s "$cmd"
    $PREFIX $cmd
  done
else
  $PREFIX "$@"
fi
