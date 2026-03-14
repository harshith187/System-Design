#include "../crow/scripts/crow_all.h"
#include <unordered_map>
#include <vector>
#include <ctime>

constexpr int LIMIT = 5;
constexpr int WINDOW = 60;

std::unordered_map<std::string, std::vector<time_t>> requestLog;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/data")
    ([](const crow::request& req) {
        std::string user =
            req.url_params.get("user_id") ? req.url_params.get("user_id") : "anon";

        time_t now = time(nullptr);
        auto& timestamps = requestLog[user];

        timestamps.erase(
            std::remove_if(timestamps.begin(), timestamps.end(),
                [&](time_t t) { return difftime(now, t) > WINDOW; }),
            timestamps.end());

        if (timestamps.size() >= LIMIT)
            return crow::response(429, "Too Many Requests: Try again later.");

        timestamps.push_back(now);
        return crow::response("Data Recieved for user: " + user + ". Requests in last " + std::to_string(WINDOW) + " seconds: " + std::to_string(timestamps.size()) + "\n");
    });

    app.port(8080).run();
}