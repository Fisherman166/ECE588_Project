#!/bin/tcsh

# Generic
 
set Threads=`seq 1 32`
set SP_file=temp.txt
set time_file=output.csv

set Runs=`seq 1 20`


echo "Run,threads,time" > ${time_file}
foreach r ($Runs)
#echo "***********   RUN " $r "***************">> ${file}
 foreach t ($Threads)
	 ./database_join $t > ${SP_file}

 set time_sec="`grep seconds ${SP_file}|cut -d '(' -f2|cut -d ' ' -f1`"
 echo $r","$t","$time_sec >> ${time_file}
end  
end 

rm ${SP_file}


#python format_presults.py