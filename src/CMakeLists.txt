include(../proj.cmake)

foreach(_module ${modules})
  add_subdirectory(${_module})
endforeach()

add_subdirectory(main)