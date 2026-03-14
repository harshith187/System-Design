#include "../crow/scripts/crow_all.h"

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "argc = " << argc << std::endl;

    for(int i = 0; i < argc; i++)
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;

    if(argc != 3)
    {
        std::cout << "Usage: ./server <server_id> <port>\n";
        return 1;
    }

    std::string serverId = argv[1];
    int port = std::stoi(argv[2]);

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([serverId]()
    {
        return "Hello from Server " + serverId + "!";
    });

    std::cout << "Starting server on port " << port << std::endl;

    app.port(port).multithreaded().run();
}