#!/bin/bash

#set -exu

# Variables
WORK_PATH=$( cd $(dirname .) ; pwd -P )
WORK_TMP_FOLDER="/var/tmp/teamcity-setup"
#WORK_TMP_FOLDER="/var/tmp/teamcity-`date "+%y%m%d-%H%M%S"`"
BINA_PASSWORD=""
BINA_USER=""

TEAMCITY_PATH_WORK=
TEAMCITY_PATH_DATA=$TEAMCITY_DATA_PATH
TEAMCITY_URL_BACKUP="https://dl.dropboxusercontent.com/u/2624170/TeamCity_Backup_20130811_233740.zip"
TEAMCITY_URL="http://download-ln.jetbrains.com/teamcity/TeamCity-8.0.2.tar.gz"

MYSQL_CONNECTOR_URL="http://mirror.cogentco.com/pub/mysql/Connector-J/mysql-connector-java-5.1.26.tar.gz"
                               
function print_error() {
  echo "${fg_rd}✗ Error : $1"
  exit 1
}

function print_title() {
  echo ""
  echo "${fg_gy}$1${fg_cl}"
}

function print_ok() {
  echo -e "${fg_gy}✔ $1"
}
                                              
function ask()
{
   if [ -z "$2" ]; then
      echo "$fg_pk$1  > \c"
   else
      echo "$fg_pk$1 [$2]  > \c"
   fi
   read answer < /dev/tty
   if [ -n "${answer}" ] ; then
      echo ${answer}
   else
      echo $2
   fi
   unset answer
}
  
function show_usage() {
  echo "$fg_cl" 
  echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
  echo "+ The purpose of this script is to restore a backup file from        +"
  echo "+ teamcity into a fresh setup                                        +"
  echo "+ Requirements:  - mysql should be installed and running             +"
  echo "+                - TEAMCITY_DATA_PATH should be set in environment   +"
  echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
  echo "$fg_cl"
}

function ask_questions() {
  print_title "Asking Parameters..." 

  echo "Which path will do you want teamcity to be installed to ?"
  read TEAMCITY_PATH_WORK < /dev/tty
 
  echo "Which username whould you like for the teamcity db ?"
  read BINA_USER < /dev/tty

  echo "Which password whould you like to use for this user ?"
  read -s BINA_PASSWORD

  TEAMCITY_PATH_BACKUP=$TEAMCITY_PATH_WORK/backup
}

function check_requirements() {
  print_title "Checking Requirements..." 

  pre_team_data=`env|grep TEAMCITY_DATA_PATH`
  if [[ "$pre_team_data" == "" ]]; then
    print_error "The TEAMCITY_DATA_PATH variable is not in your environment"
  fi

  pre_mysql=`mysqladmin -umysql ping|grep alive`
  if [[ "$pre_mysql" == "" ]]; then
    print_error "mysql doesn't seem to be running"
  fi
}

function create_tmp_folder() {
  print_title "Creating temporary working folder: ${WORK_TMP_FOLDER}"
  mkdir -p ${WORK_TMP_FOLDER}
  cd ${WORK_TMP_FOLDER}
 }

function erase_tmp_folder() {
  print_title "Cleaning up temporary working folder: ${WORK_TMP_FOLDER}"
 }

function setup_database() {
  print_title "Setting up Database..."
  echo "${fg_cl}You will be prompted for your mysql root password"

  echo "
  CREATE DATABASE teamcity DEFAULT CHARSET utf8;
  GRANT ALL PRIVILEGES ON teamcity.* TO ${BINA_USER}@localhost 
  IDENTIFIED BY \'${BINA_PASSWORD}\'; "|\
  xargs -I {} mysql -u root -p -e {}

  if [[ $? != 0 ]]; then
    print_error "last command failed..."
  fi

}

function download_teamcity() {
  print_title "Downloading TeamCity..."

  if [[ -f `echo $TEAMCITY_URL|grep -oE "[^/]+$"` ]]; then
    echo "TeamCity already downloaded, skipping download"
  else
    # Warning the TEAMCITY_PATH_DATA is used by team city and needs to be in
    # the environment variable,
    # otherwise the default ~/.BuildServer will be used
    # I suggest you put it in the path/to/teamcity/data
    cd $WORK_TMP_FOLDER
    mkdir -p ${TEAMCITY_PATH_WORK}
    curl -O "$TEAMCITY_URL"
  fi

  tar -xf `echo $TEAMCITY_URL|grep -oE "[^/]+$"`

  # check untar
  if [[ $? != 0 ]]; then print_error "last command failed..."; fi

  # move to teamcity work path
  mv TeamCity $TEAMCITY_PATH_WORK 
}

function download_teamcity_backup() {
  print_title "Downloading TeamCity Backup File..."

  cd $WORK_TMP_FOLDER
  mkdir -p $TEAMCITY_PATH_BACKUP
  cd $TEAMCITY_PATH_BACKUP
  curl -O "${TEAMCITY_URL_BACKUP}"
  backup_zip_file=`ls |grep -i "TeamCity.*Backup.*.zip"`
}

function download_mysql_connector() {
  print_title "Downloading mysql JAVA Connector..."

  cd $WORK_TMP_FOLDER
  if [[ -f `echo $MYSQL_CONNECTOR_URL|grep -oE "[^/]+$"` ]]; then
    echo "mysql JAVA Connector already downloaded, skipping download"
  else
    cd $WORK_TMP_FOLDER
    mkdir -p ${TEAMCITY_PATH_DATA}/lib/jdbc
    mysql_connector_file=`echo $MYSQL_CONNECTOR_URL|grep -oE "[^/]+$"`
    curl -O "$MYSQL_CONNECTOR_URL"
  fi

  tar -xf `echo $MYSQL_CONNECTOR_URL|grep -oE "[^/]+$"`

  #check untar
  if [[ $? != 0 ]]; then print_error "last command failed..." ; fi

  # copy jar file to lib/jdbc
  find .            \
    -type f         \
    -maxdepth 2     \
    -name "*.jar"   \
    -exec cp -f {} "${TEAMCITY_PATH_DATA}/lib/jdbc" \;

  # check copy was fine
  if [[ $? != 0 ]]; then print_error "last command failed..." ; fi
}

function setup_teamcity() {
  print_title "TeamCity Setup..."

  cd $TEAMCITY_PATH_WORK

  # dump database.properties file
  echo "connectionUrl=jdbc:mysql://localhost:3306/teamcity
    connectionProperties.user=jjbrothers
    connectionProperties.password=$BINA_PASSWORD" >database.properties

  # restore db
  bin/maintainDB.sh restore                   \
    -F $TEAMCITY_PATH_BACKUP/$backup_zip_file \
    -T ../database.properties

  #check restore
  if [[ $? != 0 ]]; then print_error "last command failed..." ; fi
}

function setup_colors() {
  fg_bk=`tput setaf 0` #black
  fg_rd=`tput setaf 1` #red
  fg_gr=`tput setaf 2` #green
  fg_yw=`tput setaf 3` #yellow
  fg_bl=`tput setaf 4` #blue
  fg_pk=`tput setaf 5` #pink
  fg_cy=`tput setaf 6` #cyan
  fg_gy=`tput setaf 7` #gray
  fg_wh=`tput setaf 9` #white
  fg_cl=`tput sgr0`    #clear
}

function __main__() {
  setup_colors
  show_usage
  check_requirements
  ask_questions
  create_tmp_folder
  setup_database
  download_teamcity
  download_teamcity_backup
  download_mysql_connector
  setup_teamcity
  erase_tmp_folder
  print_ok "Done"
}

__main__ $*





