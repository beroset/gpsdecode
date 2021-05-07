cmake_minimum_required(VERSION 3.16)

# Add API Reference generation
find_package(Doxygen 1.8.16)
set(PROJECT_BRIEF "AMR WAN to latitude/longitude converter library")
set(MD_SOURCES 01_intro.md 02_building.md 03_using.md)
if(DOXYGEN_FOUND)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/doxygen.conf.in"
        "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf"
        @ONLY
    )
    add_custom_target(
        doc
        "${DOXYGEN_EXECUTABLE}"
        "${CMAKE_CURRENT_BINARY_DIR}/doxygen.conf"
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        DEPENDS "${MD_SOURCES}"
        COMMENT "Generating code documentation..." VERBATIM
    )
    add_custom_target(
        pdf
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf"
    )
    add_custom_command(
        OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/latex/refman.pdf"
        COMMAND "make"
        DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/latex"
        COMMENT "Generating PDF format code documentation..." VERBATIM
    )

endif(DOXYGEN_FOUND)
