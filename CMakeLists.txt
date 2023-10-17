project(p2)

add_executable(server server.cc server_main.cc)
add_executable(client client.cc board.cc client_main.cc)