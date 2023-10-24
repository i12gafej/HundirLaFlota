project(p2)

add_executable(server server.cc server_main.cc game.cc player.cc)
add_executable(client player.cc client_main.cc)