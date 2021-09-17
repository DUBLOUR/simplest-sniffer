cmake_minimum_required(VERSION 3.20)
project(simplest_sniffer C)

set(CMAKE_C_STANDARD 11)

add_executable(simplest_sniffer
        client.c
        messenger.c
        messenger.h
        model.c
        model.h
        printer.c
        printer.h
        usage_information.c
        usage_information.h)
