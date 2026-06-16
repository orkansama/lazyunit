cmake_minimum_required(VERSION 3.20)
set(ENV{PROJECTNAME} dotnet-test-tui)

project($ENV{PROJECTNAME} VERSION 1.0)

add_executable($ENV{PROJECTNAME} main.c)

target_link_libraries($ENV{PROJECTNAME} -lncurses)
