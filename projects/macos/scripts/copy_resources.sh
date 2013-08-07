#!/usr/bin/env sh
#
# Build phase script to copy resources 
# Usage is: ./copy_resources.sh

#set -exu

PATH_RES="${SRCROOT}/../../assets"

folders=`ls ${PATH_RES}`
for folder in ${folders}; do
    from="${PATH_RES}/${folder}"
    to="${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}"

    if [ -d "${from}" ]; then
      echo $from;
      echo $to;
      rsync -avyz ${from} ${to}
      #rsync -avyz $exclude_option "$sub_type_path/" "${CONFIGURATION_BUILD_DIR}/${UNLOCALIZED_RESOURCES_FOLDER_PATH}"
    fi
done