project(rapidxml)
cmake_minimum_required(VERSION 3.4)

add_library(rapidxml
    rapidxml.hpp
    rapidxml_iterators.hpp
    rapidxml_print.hpp
    rapidxml_utils.hpp
)

target_include_directories(rapidxml PUBLIC
    .
)

set_target_properties(rapidxml PROPERTIES LINKER_LANGUAGE CXX)