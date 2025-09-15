#!/bin/bash

readonly EXECUTE_COMMAND=2

while read -e -p "auth_shell> " cmd; do
    case "$cmd" in
        exit)
            echo "Выход из оболочки"
            rm /home/tnovikov/study/iu6/IBAS/lab2/.auth_shell/.current_user >> /dev/null 2>&1
            break
            ;;
        "")
            continue
            ;;
        *)
            eval "auth_system $cmd"
            exit_code=$?
            
            if [ $exit_code -eq $EXECUTE_COMMAND ]; then
                eval "$cmd"
            fi
            ;;
    esac
done