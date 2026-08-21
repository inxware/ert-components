#!/bin/bash

Help(){
    echo "store_data.sh [adb device id] [test name] [results root dir] [process file]"
    exit 1
}

DeviceNotConnectedError(){
    echo "Device $1 is not connected to adb or invalid"
    Help
}

GeneralError(){
    echo "Something went wrong !"
    Help
}

PageSizeError(){
    echo "Failed to obtain page size !"
    Help
}

Convert2Kilo(){
    VALUE=$1
    if [[ "$VALUE" == *"G"* ]]; then # is Giga
        VALUE=$(echo "$VALUE" | sed 's/G//g')
        echo "$VALUE * 1000000" | bc -l
    elif [[ "$VALUE" == *"M"* ]]; then # is Mega
        VALUE=$(echo "$VALUE" | sed 's/M//g')
        echo "$VALUE * 1000" | bc -l
    else
        echo "$VALUE"
    fi
}

if [ -z "$ADB" ]; then
    if [ -z "$DEVICE_ID"]; then 
        export ADB="adb"
    else 
        export ADB="adb -s $DEVICE_ID"
    fi
fi

PROCESS_NAME_FILTER="0-9a-zA-Z, .?_:"
DEVICE_ID=$1
DATA_NAME=$2
DATA_ROOT=$3
FILTER_IN_PROCESS_FILE=$4

$ADB  get-state || DeviceNotConnectedError "$DEVICE_ID"

if [ -z "$DATA_NAME" ]; then
    echo "Data name must be specified as a first argument!"
    Help
fi

if [ -z "$DATA_ROOT" ]; then
    echo "Data root folder must be specified!"
    Help
fi

if ! [ -f "$FILTER_IN_PROCESS_FILE" ]; then
    echo "Processes must be specified in a file and passed as a last argument."
    Help
fi

TIME=$( date +'%Y-%m-%d_%H-%M-%S' )
DATA_DIR=${DATA_ROOT}/${DATA_NAME}_${TIME}
PROCESS_DIR=${DATA_DIR}/process
OTHER_DIR=${DATA_DIR}/other

TOP_PIDOF_PROCESSES=''
TOP_PROCESSES=''
PROCESS_FILTER_ARRAY=()
PROCESS_LABEL_ARRAY=()
while IFS= read -r line; do
    IFS=', ' read -r -a PROCESS_PAIR <<< "$line"
    PROCESS_LABEL_ARRAY+=( ${PROCESS_PAIR[0]} )
    PROCESS_FILTER_ARRAY+=( $( echo "${PROCESS_PAIR[1]}" | sed "s/[^$PROCESS_NAME_FILTER]*//g" ) )
    TOP_PIDOF_PROCESSES=${TOP_PIDOF_PROCESSES}'$(pidof '${line}'),'
    TOP_PROCESSES=${TOP_PROCESSES}${PROCESS_PAIR[1]}' '
done <${FILTER_IN_PROCESS_FILE}

if [ -z "$TOP_PIDOF_PROCESSES" ]; then
    echo "No process found in in the file ($FILTER_IN_PROCESS_FILE)"
    Help
fi

echo "Starting to store android os stats data ($DATA_DIR)"
echo "for following processes [${PROCESS_FILTER_ARRAY[@]}]"

mkdir -p $DATA_DIR || GeneralError
mkdir -p "$PROCESS_DIR" || GeneralError
mkdir -p "$OTHER_DIR" || GeneralError

# load apps to be filtered in
TIMESTAMP=""
MARKER=""

GetLabel(){
    PROCESS=$1
    for i in "${!PROCESS_FILTER_ARRAY[@]}"; do
        if [[ "${PROCESS_FILTER_ARRAY[$i]}" == "${PROCESS}" ]]; then
            echo "${PROCESS_LABEL_ARRAY[$i]}";
        fi
    done
}

WriteProcessHeader(){
    FILE_NAME=$1
    LABEL=$2
    if ! [ -f "$FILE_NAME" ]; then
        if ! [ -n "$LABEL" ]; then
            echo "time,pid,virt,res,shr,swap,cpu,mem,marker" >> "$FILE_NAME"
        else
            echo "time($LABEL),pid($LABEL),virt($LABEL),res($LABEL),shr($LABEL),swap($LABEL),cpu($LABEL),mem($LABEL),marker($LABEL)" >> "$FILE_NAME"
        fi
    fi
}

WriteProcessNull(){
    FILE_NAME=$1
    TIME=$2
    echo "$TIME,0,0,0,0,0,0,0,$MARKER" >> "$FILE_NAME"
}

WriteMemoryHeader(){
    FILE_NAME=$1
    if ! [ -f "$FILE_NAME" ]; then
        echo "time,total,used,free,buffers,marker" >> "$FILE_NAME"
    fi
}

WriteSwapHeader(){
    FILE_NAME=$1
    if ! [ -f "$FILE_NAME" ]; then
        echo "time,total,used,free,cached,marker" >> "$FILE_NAME"
    fi
}

ProcessTop(){
    LINE=$( echo "$@" | sed -e 's/   */ /g' )
    IFS=' ' read -ra TOP_ARRAY <<< "$LINE"
    LENGTH=${#TOP_ARRAY[@]}
    if [ "$LENGTH" -gt "0" ]; then 
        #echo "Length:$LENGTH"
        FIRST_ELEM=${TOP_ARRAY[0]}
        # check if the first element is a digit, which would idicate a pid number
        re='^[0-9]+$'
        if [[ $FIRST_ELEM =~ $re ]]; then
            PID=${TOP_ARRAY[0]}
            # get process name (make sure it contains valid characters)
            PROCESS_NAME=$( echo ${TOP_ARRAY[9]} | sed "s/[^$PROCESS_NAME_FILTER]*//g" )
            FILE_NAME=${PROCESS_DIR}/${PROCESS_NAME}
            #DATA=${TOP_ARRAY[@]:0:11}
            #echo "$TIMESTAMP $DATA" >> $FILE_NAME
            # write header
            LABEL=$( GetLabel "$PROCESS_NAME" )
            WriteProcessHeader "$FILE_NAME" "$LABEL"
            # pid(0),virt(2),res(3),shr(4),swap(5),cpu(6),mem(7)
            DATA=${TOP_ARRAY[0]},$(Convert2Kilo ${TOP_ARRAY[2]}),$(Convert2Kilo ${TOP_ARRAY[3]}),$(Convert2Kilo ${TOP_ARRAY[4]}),$(Convert2Kilo ${TOP_ARRAY[5]}),${TOP_ARRAY[6]},${TOP_ARRAY[7]},$MARKER
            echo "$TIMESTAMP,$DATA" >> "$FILE_NAME"
            echo -n "$PROCESS_NAME"
        else
            OTHER_NAME=$( echo ${TOP_ARRAY[0]} | sed "s/[^$PROCESS_NAME_FILTER]*//g" )
            FILE_NAME=${OTHER_DIR}/${OTHER_NAME}
            DATA=""
            if [[ "$OTHER_NAME" == "Mem" ]]; then
                WriteMemoryHeader "$FILE_NAME"
                DATA=$(Convert2Kilo ${TOP_ARRAY[1]}),$(Convert2Kilo ${TOP_ARRAY[3]}),$(Convert2Kilo ${TOP_ARRAY[5]}),$(Convert2Kilo ${TOP_ARRAY[7]}),$MARKER
            elif [[ "$OTHER_NAME" == "Swap" ]]; then
                WriteSwapHeader "$FILE_NAME"
                DATA=$(Convert2Kilo ${TOP_ARRAY[1]}),$(Convert2Kilo ${TOP_ARRAY[3]}),$(Convert2Kilo ${TOP_ARRAY[5]}),$(Convert2Kilo ${TOP_ARRAY[7]}),$MARKER
            fi
            if [ -n "$DATA" ]; then
                echo "$TIMESTAMP,$DATA" >> "$FILE_NAME"
            fi
        fi
    fi
}

RunTop(){    
    while 1
    do
        read -t 0.5 -n 1 marker
        MARKER=$marker
        
        TIMESTAMP=$( date +'%H:%M:%S:%3N' )
        # get top data from the device
        TOP_ARRAY=()
        while read -r line ; do
            TOP_ARRAY+=("$line");
        done < <($ADB -s $DEVICE_ID shell 'top -b -d 1 -n 1 -O "VIRT,RES,SHR,SWAP" -p $( echo "'${TOP_PIDOF_PROCESSES}'" | sed "'"s/,\{2,\}/,/g"'" )')
        # process top data
        PROCESSED_ARRAY=()
        for line in "${TOP_ARRAY[@]}"
        do
            PROCESSED=$( ProcessTop $line )
            if [ -n "$PROCESSED" ]; then
                PROCESSED_ARRAY+=($PROCESSED);
            fi
        done
        # check that all expected processes are done
        for PROCESS_NAME in "${PROCESS_FILTER_ARRAY[@]}"
        do
            if [[ "${PROCESSED_ARRAY[*]}" != *"$PROCESS_NAME"* ]]; then
                LABEL=$( GetLabel "$PROCESS_NAME" )
                FILE_NAME=${PROCESS_DIR}/${PROCESS_NAME}
                WriteProcessHeader "$FILE_NAME" "$LABEL"
                WriteProcessNull "$FILE_NAME" "$TIMESTAMP"
            fi
        done
        echo "$TIMESTAMP"
    done
}

#================================================================================

WriteProcProcessHeader(){
    FILE_NAME=$1
    LABEL=$2
    if ! [ -f "$FILE_NAME" ]; then
        if ! [ -n "$LABEL" ]; then
            echo "Time,Pid,PssTotal,PrivateDirty,PrivateClean,SwapPssDirty,HeapSize,HeapAlloc,HeapFree,Marker" >> "$FILE_NAME"
        else
            echo "Time($LABEL),Pid($LABEL),PssTotal($LABEL),PrivateDirty($LABEL),PrivateClean($LABEL),SwapPssDirty($LABEL),HeapSize($LABEL),HeapAlloc($LABEL),HeapFree($LABEL),Marker($LABEL)" >> "$FILE_NAME"
        fi
    fi
}

WriteProcProcessNull(){
    FILE_NAME=$1
    TIME=$2
    #timestamp,Pid,PssTotal,PrivateDirty,PrivateClean,SwapPssDirty,HeapSize,HeapAlloc,HeapFree,Marker
    echo "$TIME,0,0,0,0,0,0,0,0,$MARKER" >> "$FILE_NAME"
}

WriteProcMemoryHeader(){
    FILE_NAME=$1
    if ! [ -f "$FILE_NAME" ]; then
        echo "Time,MemTotal,MemUsed,MemFree,Buffers,MemAvailable,Marker" >> "$FILE_NAME"
    fi
}

WriteProcSwapHeader(){
    FILE_NAME=$1
    if ! [ -f "$FILE_NAME" ]; then
        echo "Time,SwapTotal,SwapUsed,SwapFree,SwapCached,Marker" >> "$FILE_NAME"
    fi
}

WriteProcOtherHeader(){
    FILE_NAME=$1
    if ! [ -f "$FILE_NAME" ]; then
        echo "Time,Active,Inactive,HighTotal,HighFree,LowTotal,LowFree,AnonPages,Shmem,Slab,Cached,KernelStack,PageTables,CmaTotal,CmaFree,Marker" >> "$FILE_NAME"
    fi
}

ProcessProc(){
    if [[ "$3" == "TOTAL" ]]; then
        PROCESS_NAME=$1
        PID=$2
        FILE_NAME=${PROCESS_DIR}/${PROCESS_NAME}
        LABEL=$( GetLabel "$PROCESS_NAME" )
        WriteProcProcessHeader "$FILE_NAME" "$LABEL"
        #timestamp,Pid,PssTotal,PrivateDirty,PrivateClean,SwapPssDirty,HeapSize,HeapAlloc,HeapFree,Marker               
        PROCCESS_MEM="$TIMESTAMP,$PID,$4,$5,$6,$7,$9,$(echo ${10} | sed 's/[^0-9]*//g'),$MARKER"
        echo "$PROCCESS_MEM" >> "$FILE_NAME"
        echo -n "$PROCESS_NAME"
    fi
}

RunProc(){
    while 1
    do
        read -t 1 -n 1 marker
        #mem
        MemTotal=""
        MemFree=""
        MemAvailable=""
        MemUsed=""
        Buffers=""
        #swap
        SwapCached=""
        SwapUsed=""
        SwapTotal=""
        SwapFree=""
        #other
        Active=""
        Inactive=""
        HighTotal=""
        HighFree=""
        LowTotal=""
        LowFree=""
        AnonPages=""
        Shmem=""
        Slab=""
        Cached=""
        KernelStack=""
        PageTables=""
        CmaTotal=""
        CmaFree=""

        MEM_ALL=""
        SWAP_ALL=""
        OTHER_ALL=""
        TIMESTAMP=""
        # get proc data from the device
        TOP_ARRAY=()
        while read -r line ; do
            # check if this is timestamp
            if [ -z "$TIMESTAMP" ]; then
                TIMESTAMP=$(echo "$line" | sed 's/[^0-9.:]*//g')
            elif [ -z $MemTotal ] && [[ "$line" == *"MemTotal:"* ]]; then
                MemTotal=$( echo "$line" | awk '{print $2}' )
            elif [ -z $MemFree ] && [[ "$line" == *"MemFree:"* ]]; then
                MemFree=$( echo "$line" | awk '{print $2}' )
            elif [ -z $MemAvailable ] && [[ "$line" == *"MemAvailable:"* ]]; then
                MemAvailable=$( echo "$line" | awk '{print $2}' )
            elif [ -z $MemUsed ] && [[ "$line" == *"MemUsed:"* ]]; then
                MemUsed=$( echo "$line" | awk '{print $2}' )
            elif [ -z $Buffers ] && [[ "$line" == *"Buffers:"* ]]; then
                Buffers=$( echo "$line" | awk '{print $2}' )
            elif [ -z $SwapCached ] && [[ "$line" == *"SwapCached:"* ]]; then
                SwapCached=$( echo "$line" | awk '{print $2}' )
            elif [ -z $SwapUsed ] && [[ "$line" == *"SwapUsed:"* ]]; then
                SwapUsed=$( echo "$line" | awk '{print $2}' )
            elif [ -z $SwapTotal ] && [[ "$line" == *"SwapTotal:"* ]]; then
                SwapTotal=$( echo "$line" | awk '{print $2}' )
            elif [ -z $SwapFree ] && [[ "$line" == *"SwapFree:"* ]]; then
                SwapFree=$( echo "$line" | awk '{print $2}' )
            #other
            elif [ -z $Active ] && [[ "$line" == *"Active(file):"* ]]; then
                Active=$( echo "$line" | awk '{print $2}' )
            elif [ -z $Inactive ] && [[ "$line" == *"Inactive(file):"* ]]; then
                Inactive=$( echo "$line" | awk '{print $2}' )
            elif [ -z $HighTotal ] && [[ "$line" == *"HighTotal:"* ]]; then
                HighTotal=$( echo "$line" | awk '{print $2}' )
            elif [ -z $HighFree ] && [[ "$line" == *"HighFree:"* ]]; then
                HighFree=$( echo "$line" | awk '{print $2}' )
            elif [ -z $LowTotal ] && [[ "$line" == *"LowTotal:"* ]]; then
                LowTotal=$( echo "$line" | awk '{print $2}' )
            elif [ -z $LowFree ] && [[ "$line" == *"LowFree:"* ]]; then
                LowFree=$( echo "$line" | awk '{print $2}' )
            elif [ -z $AnonPages ] && [[ "$line" == *"AnonPages:"* ]]; then
                AnonPages=$( echo "$line" | awk '{print $2}' )
            elif [ -z $Shmem ] && [[ "$line" == *"Shmem:"* ]]; then
                Shmem=$( echo "$line" | awk '{print $2}' )
            elif [ -z $Slab ] && [[ "$line" == *"Slab:"* ]]; then
                Slab=$( echo "$line" | awk '{print $2}' )
            elif [ -z $Cached ] && [[ "$line" == *"Cached:"* ]]; then
                Cached=$( echo "$line" | awk '{print $2}' )
            elif [ -z $KernelStack ] && [[ "$line" == *"KernelStack:"* ]]; then
                KernelStack=$( echo "$line" | awk '{print $2}' )
            elif [ -z $PageTables ] && [[ "$line" == *"PageTables:"* ]]; then
                PageTables=$( echo "$line" | awk '{print $2}' )
            elif [ -z $CmaTotal ] && [[ "$line" == *"CmaTotal:"* ]]; then
                CmaTotal=$( echo "$line" | awk '{print $2}' )
            elif [ -z $CmaFree ] && [[ "$line" == *"CmaFree:"* ]]; then
                CmaFree=$( echo "$line" | awk '{print $2}' )
            else
                TOP_ARRAY+=("$line");
            fi
        done < <($ADB -s $DEVICE_ID shell 'date +"%H:%M:%S.%3N"; cat /proc/meminfo;
                                           for name in '${TOP_PROCESSES}'; do pid=$( pidof $name ); [ -z $pid ] && continue; 
                                           echo $name $pid $(dumpsys meminfo $pid | grep -m 1 TOTAL); done')
       # upload the generic ones
       if [ -z "$MEM_ALL" ]; then
           FILE_NAME=${OTHER_DIR}/"Mem"
           WriteProcMemoryHeader "$FILE_NAME"
           #time,total,used,free,buffers,available,marker
           MemUsed=$( echo "$MemTotal $MemFree" | awk '{print $1-$2}')
           MEM_ALL="$TIMESTAMP,$MemTotal,$MemUsed,$MemFree,$Buffers,$MemAvailable,$MARKER"
           echo "$MEM_ALL" >> "$FILE_NAME"
       fi
       if [ -z "$SWAP_ALL" ]; then
           FILE_NAME=${OTHER_DIR}/"Swap"
           WriteProcSwapHeader "$FILE_NAME"
           #time,total,used,free,cached
           SwapUsed=$( echo "$SwapTotal $SwapFree" | awk '{print $1-$2}')
           SWAP_ALL="$TIMESTAMP,$SwapTotal,$SwapUsed,$SwapFree,$SwapCached,$MARKER"
           echo "$SWAP_ALL" >> "$FILE_NAME"
       fi
       if [ -z "$OTHER_ALL" ]; then
           FILE_NAME=${OTHER_DIR}/"Other"
           WriteProcOtherHeader "$FILE_NAME"
           #time,...
           OTHER_ALL="$TIMESTAMP,$Active,$Inactive,$HighTotal,$HighFree,$LowTotal,$LowFree,$AnonPages,$Shmem,$Slab,$Cached,$KernelStack,$PageTables,$CmaTotal,$CmaFree,$MARKER"
           echo "$OTHER_ALL" >> "$FILE_NAME"
       fi
        # process top data
        PROCESSED_ARRAY=()
        for line in "${TOP_ARRAY[@]}"
        do
            #echo $line
            PROCESSED=$( ProcessProc $line )
            if [ -n "$PROCESSED" ]; then
                PROCESSED_ARRAY+=($PROCESSED);
            fi
        done
        # check that all expected processes are done
        for PROCESS_NAME in "${PROCESS_FILTER_ARRAY[@]}"
        do
            if [[ "${PROCESSED_ARRAY[*]}" != *"$PROCESS_NAME"* ]]; then
                LABEL=$( GetLabel "$PROCESS_NAME" )
                FILE_NAME=${PROCESS_DIR}/${PROCESS_NAME}
                WriteProcProcessHeader "$FILE_NAME" "$LABEL"
                WriteProcProcessNull "$FILE_NAME" "$TIMESTAMP"
            fi
        done
        echo "$TIMESTAMP"
    done
}

#==============================================================================

NameMatch(){
    if [ "$1" == "$2" ]; then
        echo "yes"
    fi
}

#PssTotal,PrivateDirty,PrivateClean,SwapPssDirty,HeapSize,HeapAlloc,HeapFree
GetProc2MemData(){
    echo "$4"
}

GetProc2SwapData(){
    echo "$7"
}

RunProc2(){
    MEM_DATA_FILE=${PROCESS_DIR}/"mem"
    SWAP_DATA_FILE=${PROCESS_DIR}/"swap"
    #write header
    HEADER_STR="Time,"
    for PROCESS_NAME in "${PROCESS_FILTER_ARRAY[@]}"
    do
        HEADER_STR="$HEADER_STR$PROCESS_NAME,"
    done
    echo "$HEADER_STR" > "$MEM_DATA_FILE"
    echo "$HEADER_STR" > "$SWAP_DATA_FILE"
    # start loop
    while 1
    do
        read -t 1 -n 1 marker
        MARKER=$marker
        TIMESTAMP=""
        # get proc data from the device
        PROCESSED_DATA_ARRAY=()
        while read -r line ; do
            # check if this is timestamp
            if [ -z "$TIMESTAMP" ]; then
                TIMESTAMP=$(echo "$line" | sed 's/[^0-9.:]*//g')
            else
                PROCESSED_DATA_ARRAY+=("$line");
            fi
        done < <($ADB -s $DEVICE_ID shell 'date +"%H:%M:%S.%3N";
                                           for name in '${TOP_PROCESSES}'; do pid=$( pidof $name ); [ -z $pid ] && continue; 
                                           echo $name $pid $(dumpsys meminfo $pid | grep -m 1 TOTAL); done')
        # add timestamp                        
        ALL_MEM_DATA="$TIMESTAMP,"
        ALL_SWAP_DATA="$TIMESTAMP,"
        # check that all expected processes are done
        for PROCESS_NAME in "${PROCESS_FILTER_ARRAY[@]}"
        do
            MEM_DATA=""
            SWAP_DATA=""
            for PROCESSED_DATA in "${PROCESSED_DATA_ARRAY[@]}"
            do
                if [[ $( NameMatch "$PROCESS_NAME" $PROCESSED_DATA ) == "yes" ]]; then
                    MEM_DATA=$( GetProc2MemData $PROCESSED_DATA )
                    SWAP_DATA=$( GetProc2SwapData $PROCESSED_DATA )
                    break
                fi
            done
            if [ -n "$MEM_DATA" ]; then
                ALL_MEM_DATA="$ALL_MEM_DATA$MEM_DATA,"
            else
                ALL_MEM_DATA=$ALL_MEM_DATA$MEM_DATA"0,"
            fi
            if [ -n "$MEM_DATA" ]; then
                ALL_SWAP_DATA="$ALL_SWAP_DATA$SWAP_DATA,"
            else
                ALL_SWAP_DATA=$ALL_SWAP_DATA$SWAP_DATA"0,"
            fi
        done
        echo "$ALL_MEM_DATA" >> "$MEM_DATA_FILE"
        echo "$ALL_SWAP_DATA" >> "$SWAP_DATA_FILE"
        echo "$TIMESTAMP"
    done
}

## start polling top
#RunTop

## start polling proc
RunProc

## start polling proc (with many processes in columns)
#RunProc2
