#!/usr/bin/env sh
#
# Build phase script to copy resources 
# Usage is: ./copy_resources.sh

#set -exu

PATH_RES="/Users/johan/Documents/Projects/personal/bina/artwork"

folders=`ls ${PATH_RES}`
for folder in ${folders}; do
    if [ -d "${PATH_RES}" ]; then
      echo "${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}/$folder"
      #rsync -avyz $exclude_option "$sub_type_path/" "${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}"
    fi
done

die now