#!/bin/bash

check_file() {
    
file_path=$1
    check_name=`echo "${file_path##*\/}" | grep -io "$search_query"`
    if [[ ! $check_name ]] 
    then    
        return 0;
    fi

    case "$search_type" in
            a)      echo "$file_path";;
            l)      [ -L "$file_path" ] && echo "$file_path";;
            s)      [ -S "$file_path" ] && echo "$file_path";;
            *)      [ -$search_type "$file_path" ] && echo "$file_path";;
    esac
}

find_file () {
    for path in "$1"/* ; do
        check_file "$path"
        if [ -d "$path" -a ! -L "$path" ]
        then
            find_file "$path"
        fi
    done
}


dir=`pwd`
search_type="a"
search_query=".*"
while [ ! $# -eq 0 ]; 
    do
        case "$1" in
            -iname   ) search_query="$2"; shift 2;;
            -type    ) search_type="$2"; shift 2;;
            *        ) dir=$1; shift 1;;
        esac
    done
find_file "$dir"
