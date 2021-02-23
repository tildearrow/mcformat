project(mcformat)

add_executable(mcformat main.c)

install(TARGETS mcformat RUNTIME DESTINATION bin)
