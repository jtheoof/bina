#!/bin/bash

BASE_TEMPLATE_DIR="${HOME}/Library/Developer/Xcode/Templates/"
BASE_FILES_DIR_NAME="File Templates"


setup_colors()
{
	fg_bk=`tput setaf 0` #black
	fg_rd=`tput setaf 1` #red
	fg_gr=`tput setaf 2` #green
	#fg_lgr=
	fg_yw=`tput setaf 3` #yellow
	fg_bl=`tput setaf 4` #blue
	fg_pk=`tput setaf 5` #pink
	fg_cy=`tput setaf 6` #cyan
	fg_gy=`tput setaf 7` #gray
	fg_wh=`tput setaf 9` #white
	return 0
}

print_template_banner()
{
	Banner=$1
   	echo "${fg_cy}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_cy}$Banner" | tee -a $LOGFILE > /dev/tty
	echo "${fg_cy}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_wh}"
	return 0
	
	
}

print_ok()
{
	echo "${fg_gr}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_gr}$1" | tee -a $LOGFILE > /dev/tty
	echo "${fg_gr}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_wh}"
	return 0
}

print_err()
{
	echo "${fg_rd}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_rd}$1" | tee -a $LOGFILE > /dev/tty
	echo "${fg_rd}---------------------------------------------------------------------------" | tee -a $LOGFILE > /dev/tty
	echo "${fg_rd}"
	return 0
}

__main__()
{
	setup_colors
	
	print_template_banner "Installing Qt Templates"
	
	echo "Creating directories"
	
	mkdir -p "${BASE_TEMPLATE_DIR}/${BASE_FILES_DIR_NAME}"
	
	echo "Intalling Project Templates"
	
	projects=`find projects -mindepth 1 -maxdepth 1 -type d`
	
	for prj in $projects
	do
		echo "Copying $prj to ${BASE_TEMPLATE_DIR}"
		cp -r "$prj" "${BASE_TEMPLATE_DIR}"
	done
	
	echo "Intalling File Templates"
	
	files=`find files -mindepth 1 -maxdepth 1 -type d`
	
	for file in $files
	do
		echo "Copying $file to ${BASE_TEMPLATE_DIR}}/${BASE_FILES_DIR_NAME}"
		cp -r "$file" "${BASE_TEMPLATE_DIR}/${BASE_FILES_DIR_NAME}"
	done
	
	echo 
	print_ok "Done"
}

__main__


