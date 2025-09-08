#!/bin/bash

while read -e -p "auth_shell> " cmd; do
    case "$cmd" in
        exit)
            echo "Выход из оболочки"
            break
            ;;
        "")
            continue
            ;;
        *)
            eval "auth_system $cmd"
            ;;
    esac
done