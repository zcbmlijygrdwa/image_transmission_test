cmake_minimum_required(VERSION 2.8)

project(getCam_proj)

find_package( OpenCV REQUIRED )

add_executable(test_get_cam test.cpp)
target_link_libraries(test_get_cam ${OpenCV_LIBS})

add_executable(test_send test_send.cpp)
target_link_libraries(test_send ${OpenCV_LIBS})

add_executable(udp_server udp_server.cpp)
target_link_libraries(udp_server ${OpenCV_LIBS})
