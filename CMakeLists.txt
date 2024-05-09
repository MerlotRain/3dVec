cmake_minimum_required(VERSION 3.20)

project(lib-cpcd)

add_library(libpcd cpcd_write.c cpcd_read.c)
