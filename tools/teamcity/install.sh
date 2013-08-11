#!/bin/sh

set -exu

# Variables
WORK_PATH=$( cd $(dirname $0) ; pwd -P )

TEAMCITY_PATH_WORK=/Volumes/Storage/Projects/tmp/teamcity/teamcity
TEAMCITY_PATH_DATA=$TEAMCITY_PATH_WORK/data
TEAMCITY_PATH_BACKUP=$TEAMCITY_PATH_WORK/backup
TEAMCITY_URL_BACKUP="https://dl.dropboxusercontent.com/u/2624170/TeamCity_Backup_20130811_233740.zip"
TEAMCITY_URL="http://download-ln.jetbrains.com/teamcity/TeamCity-8.0.2.tar.gz"

MYSQL_CONNECTOR_URL="http://mirror.cogentco.com/pub/mysql/Connector-J/mysql-connector-java-5.1.26.tar.gz"

function download_teamcity() {
  # Warning the TEAMCITY_PATH_DATA is used by team city and needs to be in the
  # environment variable otherwise the default ~/.BuildServer will be taken.
  # I suggest you put it in the path/to/teamcity/data
  cd $WORK_PATH
  mkdir -p ${TEAMCITY_PATH_WORK}
  #curl -O "$TEAMCITY_URL"
  tar -xf `echo $TEAMCITY_URL|grep -oE "[^/]+$"`
  mv TeamCity $TEAMCITY_PATH_WORK
}

function download_teamcity_backup() {
  cd $WORK_PATH
  mkdir -p $TEAMCITY_PATH_BACKUP
  cd $TEAMCITY_PATH_BACKUP
  curl -O "${TEAMCITY_URL_BACKUP}"
  backup_zip_file=`ls |grep -i "TeamCity.*Backup.*.zip"`
}

function download_mysql_connector() {
  cd $WORK_PATH
  mkdir -p ${TEAMCITY_PATH_DATA}/lib/jdbc
  mysql_connector_file=`echo $MYSQL_CONNECTOR_URL|grep -oE "[^/]+$"`
  #curl -O "$MYSQL_CONNECTOR_URL"
  tar -xf `echo $MYSQL_CONNECTOR_URL|grep -oE "[^/]+$"`
  find .            \
    -type f         \
    -maxdepth 2     \
    -name "*.jar"   \
    -exec cp -f {} "${TEAMCITY_PATH_DATA}/lib/jdbc" \;
}

function setup_teamcity() {
  cd $TEAMCITY_PATH_WORK
  echo "connectionUrl=jdbc:mysql://localhost:3306/teamcity
    connectionProperties.user=jjbrothers
    connectionProperties.password=$BINA_PASSWORD" >database.properties
  bin/maintainDB.sh restore                   \
    -F $TEAMCITY_PATH_BACKUP/$backup_zip_file \
    -T ../database.properties
}

function __main__() {
  download_teamcity
  download_teamcity_backup
  download_mysql_connector
  setup_teamcity
}

__main__ $*





