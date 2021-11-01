cmake_minimum_required(VERSION 3.20)
project(simplest_sniffer C)

set(CMAKE_C_STANDARD 11)

add_executable(ssniffer
        client.c
        messenger.c
        model.c
        usage_information.c
        usage_information.h)

find_package (Threads)
include(FindPCAP.cmake)

add_executable(ssnifferd
        daemon.c
        stat_holder.c
        trie.c
        sniffer.c)

target_link_libraries(ssnifferd ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ssnifferd ${PCAP_LIBRARY})