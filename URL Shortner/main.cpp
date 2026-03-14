// Include the Crow single-header web framework.
// This provides HTTP server functionality and routing similar to Flask.
#include "../crow/scripts/crow_all.h"

#include <unordered_map>
#include <string>
#include <atomic>

// In-memory storage for mapping short IDs -> original long URLs.
// unordered_map provides average O(1) lookup time.
std::unordered_map<std::string, std::string> urlStore;

// Atomic counter used to generate unique IDs for each new URL.
// Atomic ensures thread-safety if the server runs with multiple threads.
std::atomic<int> counter{1};

// Base62 character set used for encoding numeric IDs into short strings.
// This keeps the URL short and avoids special characters.
const std::string BASE62 =
"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";


// Converts an integer ID into a Base62 encoded string.
// Example:
// 1 -> "1"
// 62 -> "10"
// 125 -> "21"
std::string base62Encode(int num)
{
    std::string result;

    // Convert the number to base62 representation
    while(num > 0)
    {
        result = BASE62[num % 62] + result;
        num /= 62;
    }

    // If result is empty (edge case for 0), return "0"
    return result.empty() ? "0" : result;
}


int main()
{
    // Create Crow application instance (HTTP server)
    crow::SimpleApp app;

    // ---------------------------------------------
    // API ENDPOINT: POST /shorten
    //
    // Accepts a JSON body containing a long URL:
    // {
    //    "url": "https://google.com"
    // }
    //
    // Generates a short ID and stores the mapping.
    // Returns JSON response containing the short ID.
    // ---------------------------------------------
    CROW_ROUTE(app, "/shorten").methods("POST"_method)
    ([](const crow::request& req)
    {
        // Parse incoming JSON request body
        auto body = crow::json::load(req.body);

        // Validate request
        if(!body || !body.has("url"))
            return crow::response(400,"Invalid JSON");

        // Extract the long URL from request
        std::string longUrl = body["url"].s();

        // Generate a unique short ID using Base62 encoding
        std::string shortId = base62Encode(counter++);

        // Store mapping in memory
        urlStore[shortId] = longUrl;

        // Prepare JSON response
        crow::json::wvalue res;
        res["short_url"] = shortId;

        return crow::response(res);
    });


    // ---------------------------------------------
    // API ENDPOINT: GET /<shortId>
    //
    // When a user accesses the short URL:
    // http://localhost:8080/<shortId>
    //
    // The server looks up the original URL and
    // responds with an HTTP 302 redirect.
    // ---------------------------------------------
    CROW_ROUTE(app,"/<string>")
    ([](const std::string& shortId) -> crow::response
    {
        // Check if shortId exists in storage
        if(urlStore.count(shortId))
        {
            // Create HTTP redirect response
            crow::response res(302, "");

            // Add redirect location header
            res.add_header("Location", urlStore[shortId]);

            return res;
        }

        // If shortId not found, return 404 error
        return crow::response(404, "URL not found");
    });


    // Start HTTP server on port 8080
    // The server will listen for incoming requests
    // such as:
    // POST /shorten
    // GET  /<shortId>
    app.port(8080).run();
}