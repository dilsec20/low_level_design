# LLD Project: URL Shortener System (like bit.ly)

## Problem Statement
Design a URL Shortening service.
- Given a long URL, the service should generate a short and unique alias (e.g., `bit.ly/3x8AbQ`).
- When a user accesses the short link, they should be redirected to the original long URL.
- Links can have custom aliases (if available).
- We need to track the number of times a short link is clicked (Analytics).
- (Optional) Links can expire.

## Object-Oriented Design Approach

### 1. Identifying Entities
- **URL Mapping:** The core entity storing the relationship between the Short URL, Long URL, User ID (owner), Creation Date, and Expiration Date.
- **User:** Represents the person generating the URLs.
- **Analytics:** Stores click counts and access data for a specific short URL.

### 2. Identifying Services (The Logic)
- **URLShortenerService:** The main interface (Facade). Handles:
  - `shorten_url(long_url, custom_alias)`: Generates or assigns an alias.
  - `get_long_url(short_url)`: Resolves the alias and increments analytics.
- **HashGenerator:** A strategy/service meant purely to generate random strings (using Base62 encoding).
- **Database / Repository:** An interface (`IUrlRepository`) for saving and fetching `URLMappings`. This follows the Dependency Inversion Principle!

## Core Algorithmic Concept: Base62 Encoding
To generate short URLs, we often use Base62 encoding.
Base62 uses `[A-Z, a-z, 0-9]` which gives 26 + 26 + 10 = 62 characters.
If we generate a random 6-character string, we have $62^6$ (approx 56.8 Billion) possible unique URLs.

## Pattern Highlights in this Code
In `url_shortener.cpp`:
1. **Dependency Inversion Principle (DIP):** The `UrlShortenerService` does not depend on a concrete database like MySQL. It depends on `IUrlRepository`. We use an `InMemoryUrlRepository` for testing.
2. **Single Responsibility Principle (SRP):** The `Base62Encoder` class has ONE job: generating short random strings. The `UrlShortenerService` handles the business rules (checking if an alias exists, mapping linking).
3. **Facade:** `UrlShortenerService` wraps the complexity of generating strings, checking the database, and updating analytics into simple `shorten()` and `expand()` methods.
