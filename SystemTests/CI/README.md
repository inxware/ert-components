Continuous Integration
======================

Run the regression tests (Note this includes building packages which can take a long time for the android targets!)

./SystemTests/CI/regression_test-published-only.sh

This populates directory structure with pass fail falgs that will be displayed in the console, 
but can also be checked again using this:

./SystemTests/CI/display_regression_tests.sh


Todo
====
1. make the packaing steps an optional feature so that quicker code regression tests can be done.
2. Add an option to run execution (e.g. for linux targets) if the a full -build test app for regression
3. Re-instate the unit test code and run this also as an additional element of the full target build regressions. 
4. Should these files go somewhere else (e.g. ./scripts/build-deploy/ ?)
build_linux_tools_installer.sh
deploy_files.sh

5. What is this are they used by the CI system and if so should the stll be in the ./scripts/build-deploy/ ?
install_inx_CI.sh
send_schedule.sh
start_remote_ehs.sh
validate_all.sh
