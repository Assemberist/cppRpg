#!/bin/bash

function ask(){
    read str
    if [[ $str == "y" ]]
    then
        echo "1"
    else
        echo "0"
    fi
}

###############################################################################
#### Start of script                                                       ####
###############################################################################

Build=()

echo "Choose build type:"
Opts=("release" "debug")
select opt in "${Opts[@]}"
do
    case $opt in
        "release")
            Build+=('build')
            break ;;
        "debug")
            Build+=('debug')
            break ;;
        *)
            echo "wrong type"
            exit ;;
    esac
done

echo "Turn on action logs? [Y/n]"
if [[ $(ask) == "n" ]]
then
    Build+=('trace1="-D DONT_LOG_ACTIONS"')
fi

echo "Turn on state calculation logs? [Y/n]"
if [[ $(ask) == "n" ]]
then
    Build+=('trace2="-D DONT_LOG_STATE"')
fi

echo "Which graphic to use?"
Opts=("ncurses (default)")
select opt in "${Opts[@]}" 
do
    case $opt in
        *)
            Build+=('graphic=ncurses')
            break ;;
    esac
done

make "${Build[@]}"