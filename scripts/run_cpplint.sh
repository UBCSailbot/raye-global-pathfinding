#!/usr/bin/env bash

set -o nounset

echo "===== Running CPP lint ====="

SCRIPTS_DIRECTORY=${BASH_SOURCE%/*}
CPP_LINT=${SCRIPTS_DIRECTORY}/cpplint.py

WORKSPACE_DIRECTORY=${SCRIPTS_DIRECTORY}/..
CPPLINT_FILE=${WORKSPACE_DIRECTORY}/cpplint.txt

filtered_output() {
    cat ${CPPLINT_FILE} | grep -v "Total errors found: 0"
}

# Run cpplint on src
SRC_DIRECTORY=${WORKSPACE_DIRECTORY}/src
${CPP_LINT} \
--linelength=120 --counting=detailed \
--root=src \
$( find ${SRC_DIRECTORY} -name \*.h -or -name \*.cpp ) \
&>> ${CPPLINT_FILE}

# Common projects dir
PROJECTS_DIRECTORY=${WORKSPACE_DIRECTORY}/projects

# Run cpplint on viewer
VIEWER_DIRECTORY=${PROJECTS_DIRECTORY}/viewer
${CPP_LINT} \
--linelength=120 --counting=detailed \
--root="projects/viewer" \
$( find ${VIEWER_DIRECTORY} -name \*.h -or -name \*.cpp ) \
&>> ${CPPLINT_FILE}

# Common test dir
TEST_DIRECTORY=${WORKSPACE_DIRECTORY}/test

# Run cpplint on basic_tests
BASIC_TESTS_DIRECTORY=${TEST_DIRECTORY}/basic_tests
${CPP_LINT} \
--linelength=120 --counting=detailed \
--root="test/basic_tests" \
$( find ${BASIC_TESTS_DIRECTORY} -name \*.h -or -name \*.cpp ) \
&>> ${CPPLINT_FILE}

filtered_output 1>&2

# in cppcheck 1.59 missingInclude suppression appears to be broken -- this is a workaround:
#
error_line_count=$(filtered_output | wc -l)

rm ${CPPLINT_FILE}

if [ ${error_line_count} != 0 ];
then
    echo "===== CPP lint Failed =====";
    exit 1;
fi

echo "===== Completed CPP lint ====="