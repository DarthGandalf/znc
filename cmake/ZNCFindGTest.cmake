include(FindPackageMessage)

find_package(GTest CONFIG)
if (NOT GMock_FOUND)
	find_path(GTEST_ROOT src/gtest-all.cc
		HINTS ENV GTEST_ROOT
		PATHS "${ZNC_SOURCE_DIR}/third_party/googletest/googletest")
	find_path(GMOCK_ROOT src/gmock-all.cc
		HINTS ENV GMOCK_ROOT
		PATHS "${ZNC_SOURCE_DIR}/third_party/googletest/googlemock")

	set(gtest_error_msg "not found, testing will be disabled.")
	set(gtest_error_msg "${gtest_error_msg} You can set environment variables")
	set(gtest_error_msg "${gtest_error_msg} GTEST_ROOT and GMOCK_ROOT")
	if(GTEST_ROOT)
		find_package_message(gtest "Found GoogleTest: ${GTEST_ROOT}"
			"${GTEST_ROOT}")
	else()
		find_package_message(gtest "GoogleTest ${gtest_error_msg}" "${GTEST_ROOT}")
		return()
	endif()
	if(GMOCK_ROOT)
		find_package_message(gmock "Found GoogleMock: ${GMOCK_ROOT}"
			"${GMOCK_ROOT}")
	else()
		find_package_message(gmock "GoogleMock ${gtest_error_msg}" "${GMOCK_ROOT}")
		return()
	endif()
	add_library(zncgtest EXCLUDE_FROM_ALL "${GTEST_ROOT}/src/gtest-all.cc")
	add_library(zncgmock EXCLUDE_FROM_ALL "${GMOCK_ROOT}/src/gmock-all.cc")
	add_library(zncgmock_main EXCLUDE_FROM_ALL "${GMOCK_ROOT}/src/gmock_main.cc")
	target_include_directories(zncgtest PUBLIC
		"${GTEST_ROOT}" "${GTEST_ROOT}/include"
		"${GMOCK_ROOT}" "${GMOCK_ROOT}/include")
	target_link_libraries(zncgmock zncgtest)
	target_link_libraries(zncgmock_main zncgmock)
	add_library(GTest::gtest ALIAS zncgtest)
	add_library(GTest::gmock ALIAS zncgmock)
	add_library(GTest::gmock_main ALIAS zncgmock_main)
	set(GMock_FOUND 1)
endif()
