cmake_minimum_required(VERSION 3.24)
project(wet_1_final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wet_1_final
        AVLtree.h
        main23a1.cpp
        player.h
        player_in_scoreboard.h
        player_in_team.h
        ready_team.h
        team.h
        wet1util.h
        worldcup23a1.cpp
        worldcup23a1.h)
