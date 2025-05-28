#
# SPDX-FileCopyrightText: 2010-2025 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# SPDX-License-Identifier: BSD-3-Clause
#
# Generic Plugin Compilation Rules
#

include_directories(${CMAKE_SOURCE_DIR}/src/generic/wizard
                    ${CMAKE_SOURCE_DIR}/src/generic/manager
)

set(gmicqt_generic_SRCS
    ${CMAKE_SOURCE_DIR}/src/generic/gmicqtplugin.cpp

    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtwizard.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtintropage.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtimagespage.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtsettingspage.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtfilterspage.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/wizard/gmicqtfinalpage.cpp

    ${CMAKE_SOURCE_DIR}/src/generic/manager/gmicqtsettings.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/manager/gmicqtprocessorthread.cpp
    ${CMAKE_SOURCE_DIR}/src/generic/manager/host_digikam_generic.cpp
)

foreach(_file ${gmicqt_generic_SRCS})
    set_property(SOURCE ${_file} PROPERTY COMPILE_DEFINITIONS ${modern_qt_definitions})
endforeach()

add_library(Generic_GmicQt_Plugin
            MODULE

            ${gmicqt_generic_SRCS}
            ${gmic_qt_QRC}
            ${gmic_qt_QM}
)

set_target_properties(Generic_GmicQt_Plugin PROPERTIES PREFIX "")

set_target_properties(Generic_GmicQt_Plugin
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
)

target_link_libraries(Generic_GmicQt_Plugin
                      PRIVATE

                      gmic_qt_common

                      Digikam::digikamcore
                      Digikam::digikamgui
                      Digikam::digikamdatabase

                      ${gmic_qt_LIBRARIES}
)

install(TARGETS Generic_GmicQt_Plugin
        DESTINATION ${QT_PLUGINS_DIR}/digikam/generic)

# Install debug symbols

if(MSVC)
    install(FILES "$<TARGET_PDB_FILE:Generic_GmicQt_Plugin>"
            DESTINATION ${QT_PLUGINS_DIR}/digikam/generic
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif()

if(APPLE)
    install(FILES "$<TARGET_FILE:Generic_GmicQt_Plugin>.dSYM"
            DESTINATION ${QT_PLUGINS_DIR}/digikam/generic
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif()
