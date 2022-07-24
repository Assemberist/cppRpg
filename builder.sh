#!/bin/bash

ask(){
    echo "$1? [y\n]"
    read str
    if [[ str == "y" ]]
    then
        return 1
    else
        return 0
    fi
}

###############################################################################
#### Start of script                                                       ####
###############################################################################

echo "1 - release, 2 - debug?"

Opts=("1" "2")

select opt in "${Opts[@]}"
do
    case $opt in
        "1")
            Build=build 
            break ;;
        "2")
            Build=debug
            break ;;
        *)
            echo "wrong type"
            exit ;;
    esac
done

if ( ask "Turn off action logs" )
then
    TraceAct='trace1="-D DONT_LOG_ACTIONS"'
fi

if ( ask "Turn off state calculation logs" )
then
    TraceState='trace2="-D DONT_LOG_STATE"'
fi

make "$Build" "$TraceAct" "$TraceState"