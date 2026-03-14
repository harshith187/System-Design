#include "../crow/scripts/crow_all.h"

#include <vector>
#include <atomic>
#include <curl/curl.h>
#include <iostream>

std::vector<std::string> servers =
{
    "http://localhost:6001",
    "http://localhost:6002",
    "http://localhost:6003"
};

std::atomic<int> currentIndex{0};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string forwardRequest(const std::string& url)
{
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            response = "Backend server unavailable";
        }

        curl_easy_cleanup(curl);
    }

    return response;
}

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]()
    {
        int index = currentIndex++ % servers.size();

        std::string serverURL = servers[index];

        std::cout << "Forwarding request to: " << serverURL << std::endl;

        return forwardRequest(serverURL);
    });

    int port = 8080;

    std::cout << "Load Balancer running on port " << port << std::endl;

    app.port(port);
    app.run();
}