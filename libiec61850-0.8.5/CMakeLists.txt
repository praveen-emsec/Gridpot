cmake_minimum_required(VERSION 2.8)

# automagically detect if we should cross-compile
if(DEFINED ENV{TOOLCHAIN})
	set(CMAKE_C_COMPILER	$ENV{TOOLCHAIN}gcc)
	set(CMAKE_CXX_COMPILER	$ENV{TOOLCHAIN}g++)
	set(CMAKE_AR	"$ENV{TOOLCHAIN}ar" CACHE FILEPATH "CW archiver" FORCE)
endif()

project(libiec61850)

set(LIB_VERSION_MAJOR "0")
set(LIB_VERSION_MINOR "8")
set(LIB_VERSION_PATCH "5")

# feature checks
include(CheckLibraryExists)
check_library_exists(rt clock_gettime "time.h" CONFIG_SYSTEM_HAS_CLOCK_GETTIME)

# check if we are on a little or a big endian
include (TestBigEndian)
test_big_endian(PLATFORM_IS_BIGENDIAN)

set(CONFIG_MMS_MAXIMUM_PDU_SIZE "65000" CACHE STRING "Configure the maximum size of an MMS PDU (default 65000)"   )
set(CONFIG_MAXIMUM_TCP_CLIENT_CONNECTIONS 5 CACHE STRING "Configure the maximum number of clients allowed to connect to the server")

option(BUILD_EXAMPLES "Build the examples" ON)

option(CONFIG_MMS_SINGLE_THREADED "Compile for single threaded version" OFF)
option(CONFIG_MMS_THREADLESS_STACK "Optimize stack for threadless operation (warning: single- or multi-threaded server will not work!)" OFF)

# choose the library features which shall be included
option(CONFIG_INCLUDE_GOOSE_SUPPORT "Build with GOOSE support" ON)

option(CONFIG_IEC61850_CONTROL_SERVICE "Build with support for IEC 61850 control features" ON)

option(CONFIG_IEC61850_REPORT_SERVICE "Build with support for IEC 61850 reporting services" ON)

option(CONFIG_IEC61850_SETTING_GROUPS "Build with support for IEC 61850 setting group services" ON)

option(CONFIG_ACTIVATE_TCP_KEEPALIVE "Activate TCP keepalive" ON)

set(CONFIG_REPORTING_DEFAULT_REPORT_BUFFER_SIZE "8000" CACHE STRING "Default buffer size for buffered reports in byte" )

# advanced options
option(DEBUG "Enable debugging mode (include assertions)" OFF)
option(DEBUG_SOCKET "Enable printf debugging for socket layer" OFF)
option(DEBUG_COTP "Enable COTP printf debugging" OFF)
option(DEBUG_ISO_SERVER "Enable ISO SERVER printf debugging" OFF)
option(DEBUG_ISO_CLIENT "Enable ISO CLIENT printf debugging" OFF)
option(DEBUG_IED_SERVER "Enable IED SERVER printf debugging" OFF)
option(DEBUG_IED_CLIENT "Enable IED CLIENT printf debugging" OFF)
option(DEBUG_MMS_SERVER "Enable MMS SERVER printf debugging" OFF)
option(DEBUG_MMS_CLIENT "Enable MMS CLIENT printf debugging" OFF)
#mark_as_advanced(DEBUG DEBUG_COTP DEBUG_ISO_SERVER DEBUG_ISO_CLIENT DEBUG_IED_SERVER
#				 DEBUG_IED_CLIENT DEBUG_MMS_SERVER DEBUG_MMS_CLIENT)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/config
    src/common/inc
    src/goose
    src/hal/inc
    src/iec61850/inc
    src/iec61850/inc_private
    src/mms/inc
    src/mms/inc_private
    src/mms/iso_mms/asn1c
)

set(API_HEADERS 
    src/hal/inc/hal_time.h 
    src/hal/inc/hal_thread.h
    src/hal/inc/hal_filesystem.h 
    src/common/inc/libiec61850_common_api.h
    src/common/inc/linked_list.h
    src/common/inc/byte_buffer.h
    src/common/inc/lib_memory.h
	src/iec61850/inc/iec61850_client.h
	src/iec61850/inc/iec61850_common.h
	src/iec61850/inc/iec61850_server.h
	src/iec61850/inc/iec61850_model.h
	src/iec61850/inc/iec61850_cdc.h
	src/iec61850/inc/iec61850_dynamic_model.h
	src/iec61850/inc/iec61850_config_file_parser.h
	src/mms/inc/mms_value.h
	src/mms/inc/mms_common.h
	src/mms/inc/mms_types.h
	src/mms/inc/mms_device_model.h
	src/mms/inc/mms_server.h
	src/mms/inc/mms_named_variable_list.h
	src/mms/inc/mms_type_spec.h
	src/mms/inc/mms_client_connection.h
	src/mms/inc/iso_connection_parameters.h
	src/mms/inc/iso_server.h
	src/mms/inc/ber_integer.h
	src/mms/inc/asn1_ber_primitive_value.h
	src/goose/goose_subscriber.h
	src/goose/goose_receiver.h
)

IF(WIN32)
include_directories(
    src/vs
)
ENDIF(WIN32)

# write the detected stuff to this file
configure_file(config/stack_config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config/stack_config.h)

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif(BUILD_EXAMPLES)

add_subdirectory(src)

INSTALL(FILES ${API_HEADERS} DESTINATION include/libiec61850)


IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)
 
SET(CPACK_SET_DESTDIR "on")
SET(CPACK_INSTALL_PREFIX "/usr")
SET(CPACK_GENERATOR "DEB")
 
SET(CPACK_PACKAGE_DESCRIPTION "IEC 61850 MMS/GOOSE client and server library")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "IEC 61850 MMS/GOOSE client and server library")
SET(CPACK_PACKAGE_VENDOR "The libIEC61850 project")
SET(CPACK_PACKAGE_CONTACT "info@libiec61850.com")
SET(CPACK_PACKAGE_VERSION_MAJOR "${LIB_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${LIB_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${LIB_VERSION_PATCH}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}_${CMAKE_SYSTEM_PROCESSOR}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${LIB_VERSION_MAJOR}.${LIB_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
 
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
 
SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)
 
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")



