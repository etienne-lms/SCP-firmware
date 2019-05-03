#!/bin/bash - 
cd ./build/product/optee/fw/release/obj

scpfm_path=${HOME}/Projects/stm32mp1-1.0/SCP-firmware
optee_os_path=${HOME}/Projects/stm32mp1-1.0/optee_os


for i in `cat ${scpfm_path}/build/product/optee/fw/obj-list.txt`; do
	echo "### $i";
	echo "$AR x $i";
	$AR x $i;
done

cp ./${scpfm_path}/build/product/optee/fw/fwk_module_list.o .

rm tmp.txt
for i in `ls *.o`; do printf "${scpfm_path}/build/product/optee/fw/release/obj/"$i" " >> tmp.txt; done

