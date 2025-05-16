#
# SPDX-FileCopyrightText: 2010-2025 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# SPDX-License-Identifier: BSD-3-Clause
#

set(FilterSelector_test_SRCS
    ${CMAKE_SOURCE_DIR}/src/tests/host_test.cpp
    ${CMAKE_SOURCE_DIR}/src/tests/main_filterselector.cpp
)

foreach(_file ${FilterSelector_test_SRCS})
    set_property(SOURCE ${_file} PROPERTY COMPILE_DEFINITIONS ${modern_qt_definitions})
endforeach()

add_executable(GmicQt_FilterSelector_test
               ${gmic_qt_QRC}
               ${gmic_qt_QM}
               ${FilterSelector_test_SRCS}
)

target_link_libraries(GmicQt_FilterSelector_test
                      PRIVATE

                      gmic_qt_common

                      Digikam::digikamcore
                      Digikam::digikamgui
                      Digikam::digikamdatabase

                      ${gmic_qt_LIBRARIES}
)

###########################################################################################@@

set(ProcessorBqm_test_SRCS
    ${CMAKE_SOURCE_DIR}/src/tests/host_test.cpp
    ${CMAKE_SOURCE_DIR}/src/tests/main_processor_bqm.cpp
)

foreach(_file ${ProcessorBqm_test_SRCS})
    set_property(SOURCE ${_file} PROPERTY COMPILE_DEFINITIONS ${modern_qt_definitions})
endforeach()

add_executable(GmicQt_Processor_Bqm_test
               ${gmic_qt_QRC}
               ${gmic_qt_QM}
               ${ProcessorBqm_test_SRCS}
)

target_link_libraries(GmicQt_Processor_Bqm_test
                      PRIVATE

                      gmic_qt_common

                      Digikam::digikamcore

                      ${gmic_qt_LIBRARIES}
)

###########################################################################################@@

set(ProcessorGeneric_test_SRCS
    ${CMAKE_SOURCE_DIR}/src/generic/host_digikam_generic.cpp
    ${CMAKE_SOURCE_DIR}/src/tests/main_processor_generic.cpp
)

foreach(_file ${ProcessorGeneric_test_SRCS})
    set_property(SOURCE ${_file} PROPERTY COMPILE_DEFINITIONS ${modern_qt_definitions})
endforeach()

add_executable(GmicQt_Processor_Generic_test
               ${gmic_qt_QRC}
               ${gmic_qt_QM}
               ${ProcessorGeneric_test_SRCS}
)

target_link_libraries(GmicQt_Processor_Generic_test
                      PRIVATE

                      gmic_qt_common

                      Digikam::digikamcore

                      ${gmic_qt_LIBRARIES}
)
