#!/bin/sh

ulimit -c 0

cd ${HOME}/DQM/dqm-gui

LD_LIBRARY_PATH=
VO_CMS_SW_DIR=${PWD}/sw
SCRAM_ARCH=slc5_amd64_gcc434
source ${VO_CMS_SW_DIR}/cmsset_default.sh
source ${VO_CMS_SW_DIR}/${SCRAM_ARCH}/cms/dqmgui/6.0.7/etc/profile.d/env.sh

if [ -e /tmp/createRunIndex.lock ]; then
  echo "Lock file is present, exit"
  exit 1
fi

touch /tmp/createRunIndex.lock

rm -fr /data/ecalod-disk01/dqm-gui/idx

echo "Index create: begin"

visDQMIndex create /data/ecalod-disk01/dqm-gui/idx

echo "Index create: end"

rm /tmp/createRunIndex.lock

exit 0
