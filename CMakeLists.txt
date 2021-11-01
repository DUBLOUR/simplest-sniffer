cmake_minimum_required(VERSION 3.20)
project(simplest_sniffer C)

set(CMAKE_C_STANDARD 11)

add_executable(ssniffer
        client/client.c
        client/messenger.c
        client/model.c
        client/usage_information.c
        client/usage_information.h)

find_package (Threads)
include(FindPCAP.cmake)

add_executable(ssnifferd
        daemon/daemon.c
        daemon/stat_holder.c
        daemon/trie.c
        daemon/sniffer.c)

target_link_libraries(ssnifferd ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ssnifferd ${PCAP_LIBRARY})