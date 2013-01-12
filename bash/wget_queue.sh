#!/bin/bash

wget_demon() {  
    touch "$QPATH/running"
    trap "rm '$QPATH/running'" 0
    while :
    do
        for a in "$QPATH/requests"/*
        do
            if [[ -f "$a" ]]; then
                url=`cat "$a"`
                rm "$a"
                wget -c "$url" 2>> "$QPATH/log_file.txt"
            fi
        done
        sleep 5
    done
}

QPATH="$HOME/.wget_demon"

mkdir -p "$QPATH"
mkdir -p "$QPATH/requests"

tmp_fldr="$QPATH/requests"

for a in "$@"
do
    tmp_file=`mktemp $tmp_fldr/queue_node.XXXXXX`
    echo "$a" > "$tmp_file"
done

if [ -f "$QPATH/running" ]; then
    echo "wget_queue_demon is working, everything is fine!"
else
    echo "wget_queue_demon was running"
    wget_demon &
fi