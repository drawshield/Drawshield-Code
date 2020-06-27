# DrawShield Regression Tests

This folder contains bash shell scripts and data files that implement a simplistic regression test suite.

The contents are as follows:

refresh.sh - bash script to generate the expected data, for later comparison

runtests.sh - bash script to run the tests and compare against the expected data

results.txt - diffs arising from any failed tests

testcases/ - folder of text files containing test cases, see details below

expected/ - folder containing svg output files created by refresh.sh and used for comparisons in runtests.sh

responses/ -folder containing svg output files created by runtests.sh and diff compared against those in the "expected" folder

## Test case data format

Each test case is stored in a separate file. The filename is the name of the test, with the suffix .txt. If required you could group tests by giving them a common prefix.

Each file is formatted as follows:

Lines beginning with a hash character are comments and will be ignored. The hash **must** be the first visible character on the line.

Blank lines are also ignored

All other lines should be a parameter and a value, separated by an equals sign.

During processing, each of the test case files will:

* have all comment lines removed
* have all empty lines removed
* have all remaining lines prepended with an ampersand and concatentated
* be URL encoded

The resulting single line will then be passed as POST argument to the drawshield program.

## Typical operation

If new test cases are added, or it there is a significant change to the drawshield code that invalidates

## Notes and limitations

Note that at present, each argument **must** be on a single line, so it is not possible to test multi-line blazons.

All result files (except the empty blazon) will always have a difference in the timestamp that is included in the metadata. To avoid false failures,  lines containing the string "timestamp" are ignored during the comparison.

If the Drawshield program code is committed to github it will get a new version id string, which appears as a text element as part of the shield image. To avoid false positives following an update, any line containing the string "version-id" (which is the id used by the relevant text element) will be ignored during the comparison.

