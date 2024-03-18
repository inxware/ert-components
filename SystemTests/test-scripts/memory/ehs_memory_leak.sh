#! /bin/bash

LOG="$PWD/memory.log"
rm $LOG
#echo -e "VSZ\tRSS\tMem Used\tMem Free\tSwap Used" > $LOG
echo "0 0 " > $LOG
gnuplot -persist ./plot_ehs_memory_leak.gplot &
while [ true ]; do
	ps=`ps aux |grep ehs.exe |grep -v grep |awk '{ print $5 "\t" $6 }'`
	echo $ps
	echo $ps >> $LOG
#	free=( $(free |egrep 'Mem:|Swap:') )
#echo $free	
#echo -e "\t${free[2]}\t\t${free[3]}\t\t${free[9]}" >>$LOG
	sleep 1
done
