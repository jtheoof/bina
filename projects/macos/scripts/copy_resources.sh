#!/usr/bin/env sh
#
# Build phase script to copy resources 
# Usage is: ./copy_resources.sh

#set -exu

PATH_RES="${SRCROOT}/../../assets"

from="${PATH_RES}"
to="${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}"

rsync -avyz ${from} ${to}