#
# SPDX-FileCopyrightText: 2010-2025 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# SPDX-License-Identifier: BSD-3-Clause
#
# Generic Plugin Compilation Rules
#

set(gmic_generic_SRCS
    ${CMAKE_SOURCE_DIR}/src/generic/gmicqtplugin.cpp
)

foreach(_file ${gmic_bqm_SRCS})
    set_property(SOURCE ${_file} PROPERTY COMPILE_DEFINITIONS ${modern_qt_definitions})
endforeach()

add_library(Generic_Gmic_Plugin
            MODULE

            ${gmic_generic_SRCS}
            ${gmic_qt_QRC}
            ${gmic_qt_QM}
)

set_target_properties(Generic_Gmic_Plugin PROPERTIES PREFIX "")

set_target_properties(Generic_Gmic_Plugin
                      PROPERTIES
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS NO
)

target_link_libraries(Generic_Gmic_Plugin
                      PRIVATE

                      gmic_qt_common

                      Digikam::digikamcore
                      Digikam::digikamgui
                      Digikam::digikamdatabase

                      ${gmic_qt_LIBRARIES}
)

install(TARGETS Generic_Gmic_Plugin
        DESTINATION ${QT_PLUGINS_DIR}/digikam/generic)

# Install debug symbols

if(MSVC)
    install(FILES "$<TARGET_PDB_FILE:Generic_Gmic_Plugin>"
            DESTINATION ${QT_PLUGINS_DIR}/digikam/generic
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif()

if(APPLE)
    install(FILES "$<TARGET_FILE:Generic_Gmic_Plugin>.dSYM"
            DESTINATION ${QT_PLUGINS_DIR}/digikam/generic
            CONFIGURATIONS Debug RelWithDebInfo
    )
endif()
