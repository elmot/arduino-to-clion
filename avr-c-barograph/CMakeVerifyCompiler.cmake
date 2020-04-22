###################################
#  Check if the compiler is correct
get_property(__LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)

if ("C" IN_LIST __LANGUAGES)
    get_filename_component(__C_COMPILER_NAME ${CMAKE_C_COMPILER} NAME_WLE)
    if (NOT __C_COMPILER_NAME STREQUAL "avr-gcc")
        message(FATAL_ERROR "Compiler \n${CMAKE_C_COMPILER}\n does not seem to be GCC AVR compiler")
    endif ()
endif ()

if ("CXX" IN_LIST __LANGUAGES)
    get_filename_component(__CXX_COMPILER_NAME ${CMAKE_CXX_COMPILER} NAME_WLE)
    if (NOT __CXX_COMPILER_NAME STREQUAL "avr-g++")
        message(FATAL_ERROR "Compiler\n${CMAKE_CXX_COMPILER}\ndoes not seem to be G++ AVR compiler")
    endif ()
endif ()
#  End of check
###################################
