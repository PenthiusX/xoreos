include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(set_check_compiler_flag_c FLAG)
  CHECK_C_COMPILER_FLAG(${FLAG} hasC${FLAG})
  if(hasC${FLAG})
    if(ARGN)
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ARGN}" PARENT_SCOPE)
    else()
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

function(set_check_compiler_flag_cxx FLAG)
  CHECK_CXX_COMPILER_FLAG(${FLAG} hasCXX${FLAG})
  if(hasCXX${FLAG})
    if(ARGN)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ARGN}" PARENT_SCOPE)
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

macro(set_check_compiler_flag_c_cxx FLAG)
  set_check_compiler_flag_c(${FLAG} ${ARGN})
  set_check_compiler_flag_cxx(${FLAG} ${ARGN})
endmacro()