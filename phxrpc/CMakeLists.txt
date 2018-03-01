set(SOURCE_FILES
        http/http_client.cpp
        http/http_msg.cpp
        http/http_protocol.cpp

        mqtt/mqtt_client.cpp
        mqtt/mqtt_msg.cpp
        mqtt/mqtt_protocol.cpp

        network/socket_stream_base.cpp
        network/uthread_runtime.cpp
        network/uthread_epoll.cpp
        network/socket_stream_block.cpp
        network/socket_stream_uthread.cpp
        network/uthread_context_util.cpp
        network/uthread_context_base.cpp
        network/uthread_context_system.cpp
        network/timer.cpp

        file/log_utils.cpp
        file/file_utils.cpp
        file/opt_map.cpp
        file/config.cpp

        rpc/phxrpc.pb.cc
        rpc/http_caller.cpp
        rpc/mqtt_caller.cpp
        rpc/server_config.cpp
        rpc/client_config.cpp
        rpc/socket_stream_phxrpc.cpp
        rpc/uthread_caller.cpp
        rpc/client_monitor.cpp
        rpc/server_monitor.cpp
        rpc/monitor_factory.cpp
        rpc/hsha_server.cpp
        rpc/server_base.cpp

        msg/base_msg.cpp
        msg/base_protocol.cpp
        msg/protocol_factory.cpp
        )

add_library(phxrpc ${SOURCE_FILES})

link_libraries(phxrpc)

add_executable(test_http_client http/test_http_client.cpp)
add_executable(test_mqtt_client mqtt/test_mqtt_client.cpp)
add_executable(test_mqtt_protocol mqtt/test_mqtt_protocol.cpp)
add_executable(test_echo_client network/test_echo_client.cpp)
add_executable(test_echo_server network/test_echo_server.cpp)
add_executable(test_epoll_client network/test_epoll_client.cpp)
add_executable(test_epoll_server network/test_epoll_server.cpp)
add_executable(test_timer network/test_timer.cpp)
add_executable(test_uthread network/test_uthread.cpp)
add_executable(test_uthread_context network/test_uthread_context.cpp)

add_executable(test_hsha_server rpc/test_hsha_server.cpp)
add_executable(test_http_client_rpc rpc/test_http_client.cpp)
add_executable(test_mqtt_client_rpc rpc/test_mqtt_client.cpp)
add_executable(test_thread_queue rpc/test_thread_queue.cpp)
