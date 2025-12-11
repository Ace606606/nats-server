# CMake generated Testfile for 
# Source directory: /home/gryakin/workdir/my/git/nats/cpp_client/cmake-3.15.2/Utilities/cmcurl
# Build directory: /home/gryakin/workdir/my/git/nats/cpp_client/cmake-3.15.2/Utilities/cmcurl
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(curl "curltest" "http://open.cdash.org/user.php")
set_tests_properties(curl PROPERTIES  _BACKTRACE_TRIPLES "/home/gryakin/workdir/my/git/nats/cpp_client/cmake-3.15.2/Utilities/cmcurl/CMakeLists.txt;1295;add_test;/home/gryakin/workdir/my/git/nats/cpp_client/cmake-3.15.2/Utilities/cmcurl/CMakeLists.txt;0;")
subdirs("lib")
