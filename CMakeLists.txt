project(p2)

add_executable(server hypersimple_server.cc server_main.cc game.cc player.cc functionality.cc)
add_executable(client player.cc client_main.cc)