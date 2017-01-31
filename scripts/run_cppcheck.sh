#!/usr/bin/env bash

set -o nounset

echo "===== Running CPP check ====="

SCRIPTS_DIRECTORY=${BASH_SOURCE%/*}
WORKSPACE_DIRECTORY=${SCRIPTS_DIRECTORY}/..
CPPCHECK_FILE=${WORKSPACE_DIRECTORY}/cppcheck.txt

filtered_output() {
    cat ${CPPCHECK_FILE} | grep -v "Cppcheck cannot find all the include files"
}

# Delete old cppcheck.xml file
if [ -f ${CPPCHECK_FILE} ]; then rm ${CPPCHECK_FILE}; fi

# Run CPP check on src
SRC_DIRECTORY=${WORKSPACE_DIRECTORY}/src
cppcheck \
--enable=all --std=c++11 --force --verbose --quiet \
--template='{file}:{line}:{severity}:{message}' \
-I ${SRC_DIRECTORY} \
--suppress=missingInclude \
--suppress=unusedFunction \
--suppress=unmatchedSuppression \
${SRC_DIRECTORY} &>> ${CPPCHECK_FILE}


# Common projects dir
PROJECTS_DIRECTORY=${WORKSPACE_DIRECTORY}/projects

# Run CPP check on viewer
VIEWER_DIRECTORY=${PROJECTS_DIRECTORY}/viewer

cppcheck \
--enable=all --std=c++11 --force --verbose --quiet \
--template='{file}:{line}:{severity}:{message}' \
-I ${VIEWER_DIRECTORY} \
--suppress=missingInclude \
--suppress=unusedFunction \
--suppress=unmatchedSuppression \
${VIEWER_DIRECTORY} &>> ${CPPCHECK_FILE}

filtered_output 1>&2

# in cppcheck 1.59 missingInclude suppression appears to be broken -- this is a workaround:
#
error_line_count=$(filtered_output | wc -l)

rm ${CPPCHECK_FILE}

if [ ${error_line_count} != 0 ];
then
    echo "===== CPP check Failed =====";
    exit 1;
fi

echo "===== Completed CPP check ====="
