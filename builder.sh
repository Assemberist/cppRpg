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

echo "1 - release, 2 - debug?"

Opts=("1" "2")
Build=()

select opt in "${Opts[@]}"
do
    case $opt in
        "1")
            Build+=('build')
            break ;;
        "2")
            Build+=('debug')
            break ;;
        *)
            echo "wrong type"
            exit ;;
    esac
done

echo "Turn off action logs?"
if [[ $(ask) == "1" ]]
then
    Build+=('trace1="-D DONT_LOG_ACTIONS"')
fi

echo "Turn off state calculation logs"
if [[ $(ask) == "1" ]]
then
    Build+=('trace2="-D DONT_LOG_STATE"')
fi

make "${Build[@]}"