#!/bin/bash
mode=$2
name=$1
echo "the ${mode} are great man!"
cd ./Output
./demo_dispUvc ${name} ${mode}