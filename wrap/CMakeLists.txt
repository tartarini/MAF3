#
#  CMakeLists.txt
#  MAF
#
#  Created by Daniele Giunchi - Paolo Quadrani on 22/03/12.
#  Copyright 2012 SCS-B3C. All rights reserved.
#
#  See Licence at: http://tiny.cc/QXJ4D
#
#
INCLUDE(ExternalProject)

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            set(originalName ${templateFile})
            STRING(REGEX REPLACE ".conf$" "" templateFile ${originalName})
            set(result 0)
            STRING(COMPARE EQUAL ${templateFile} ${originalName} result)
            if(NOT result)
                configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
            else(NOT result)
                configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    COPYONLY)
            endif(NOT result)
        else(NOT IS_DIRECTORY ${srcTemplatePath})
            set(nextDir ${destDir}/${templateFile})
            set(resultCVSWin 0)
            set(resultSVNWin 0)
            set(resultCVSUnix 0)
            set(resultSVNUnix 0)
            STRING(COMPARE EQUAL ${templateFile} "CVS" resultCVSWin)
            STRING(COMPARE EQUAL ${templateFile} "SVN" resultSVNWin)
            STRING(COMPARE EQUAL ${templateFile} ".cvs" resultCVSUnix)
            STRING(COMPARE EQUAL ${templateFile} ".svn" resultSVNUnix)
            STRING(COMPARE EQUAL ${templateFile} ".svn" resultSVNUnix)
            if(NOT(resultCVSWin OR resultSVNWin OR resultCVSUnix OR resultSVNUnix))
                make_directory(${nextDir})
                configure_files(${srcTemplatePath} ${nextDir})
            endif(NOT(resultCVSWin OR resultSVNWin OR resultCVSUnix OR resultSVNUnix))
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

ExternalProject_Add(wrap
      DOWNLOAD_COMMAND ""
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      DEPENDS doc
)

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR})
execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/mafPythonQtDecorators)
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/mafPythonQtDecorators)


get_filename_component(CUR_ABSOLUTE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
configure_files("${CUR_ABSOLUTE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
