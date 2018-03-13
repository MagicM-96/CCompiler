#!/bin/bash

# Script to automate running all test (success/fail) scripts

CC=../../bin/dhbwcc
CFLAGS=-p

: ${SUCCESS_TEST_DIR:=./success}
: ${FAIL_TEST_DIR:=./fail}

SUCCESS_TESTS=`find "$SUCCESS_TEST_DIR" -type f -name '*.c' | sort`
FAIL_TESTS=`find "$FAIL_TEST_DIR" -type f -name '*.c' | sort`

# Check that compiler exists
if [[ ! -e "$CC" ]]; then
	echo "ERROR: Compiler ("$CC") could not be found"
	exit 1
fi

# Run success tests
for test in $SUCCESS_TESTS;
do
	cat <<- EOM
		----------------------------------------
		Running success test: $test
		Command: $CC $CLAGS $test
		Command/compiler output:
	EOM

	output=`"$CC" "$CFLAGS" "$test"`
	result=$?

	cat <<- EOM
		===========================
		$output
		===========================
		Crashed: `[[ $result -ne 0 ]] && echo "YES" || echo "NO"`
		----------------------------------------
	EOM

	if [[ $result -ne 0 ]]; then
		echo "ERROR: Stopped testing as compiler crashed on success test"
		exit 1
	fi
done

# Run fail tests
for test in $FAIL_TESTS;
do
	cat <<- EOM
		----------------------------------------
		Running fail test: $test
		Command: $CC $CLAGS $test
		Command/compiler output:
	EOM

	output=`"$CC" "$CFLAGS" "$test"`
	result=$?

	cat <<- EOM
		===========================
		$output
		===========================
		Crashed: `[[ $result -gt 1 ]] && echo "YES" || echo "NO"`
		----------------------------------------
	EOM

	if [[ $result -gt 1 ]]; then # dhbwcc is supposed to return 1 on any compilation issue
		echo "ERROR: Stopped testing as compiler crashed on fail test"
		exit 1
	fi

	if [[ $result -eq 0 ]]; then

		echo "ERROR: Stopped testing as compiler did not fail on fail test"
		exit 1
	fi
done
