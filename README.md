# System Design Mini Projects (C++)

This repository contains three small projects that demonstrate basic **system design concepts** implemented in **C++**.

---

## Projects

### 1. URL Shortener

A simple service that converts long URLs into short IDs.

**Endpoints**

- `POST /shorten` → create a short URL  
- `GET /{shortId}` → redirect to the original URL  

**Concepts**

- API design
- Base62 ID generation
- Fast lookup using `unordered_map`

---

### 2. Rate Limiter

A basic rate limiter that restricts how many requests a user can make.

**Endpoint**

- `GET /data`

**Limit**

- Maximum **5 requests per minute per user**

**Concepts**

- Request tracking
- Timestamp validation
- Preventing API abuse

---

### 3. Load Balancer

A simple **Round Robin load balancer** that distributes requests across multiple backend servers.

**Backend Servers**

- `localhost:6001`
- `localhost:6002`
- `localhost:6003`

The load balancer forwards requests sequentially to each server.

**Concepts**

- Round Robin algorithm
- Request forwarding using `libcurl`
- Basic traffic distribution

---

## Build

Compile example:

```bash
g++ server.cpp -o server -std=c++17
g++ load_balancer.cpp -o load_balancer -lcurl -std=c++17
