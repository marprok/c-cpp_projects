#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

constexpr float EPSILON = 0.01;

struct Coin
{
    std::string name;
    float       price;
};

struct CoinPrice
{
    float             from;
    float             to;
    std::vector<Coin> coins;

    bool can_add(const Coin& coin)
    {

        const float low_diff  = coin.price - from;
        const float high_diff = to - coin.price;

        return !std::signbit(low_diff) && low_diff >= EPSILON && !std::signbit(high_diff) && high_diff >= EPSILON;
    }
};

int main()
{
    std::vector<CoinPrice> coins_per_price;
    coins_per_price.reserve(3);
    coins_per_price.push_back({ 0.1, 10.0, {} });
    coins_per_price.push_back({ 10.0, 50.0, {} });
    coins_per_price.push_back({ 50, std::numeric_limits<float>::max(), {} });

    auto add_coin = [&coins_per_price](const Coin& coin)
    {
        for (auto& coin_price : coins_per_price)
            if (coin_price.can_add(coin))
                coin_price.coins.push_back(coin);
    };

    auto print_coins = [&coins_per_price](bool is_in_high_trading)
    {
        for (auto& coin_price : coins_per_price)
        {
            if (is_in_high_trading)
                std::sort(coin_price.coins.begin(), coin_price.coins.end(), [](const Coin& l, const Coin& r)
                          { return (r.price - l.price) >= EPSILON; });

            const std::size_t coin_count = coin_price.coins.size();
            std::cout << "Price: [" << coin_price.from
                      << ((coin_price.to != std::numeric_limits<float>::max())
                              ? std::string(" - ") + std::to_string(coin_price.to) + "]"
                              : "+]")
                      << std::endl;
            for (std::size_t i = 0; i < coin_count; ++i)
            {
                const auto& coin = coin_price.coins[i];
                std::cout << coin.name << ": " << coin.price;
                if (i != coin_price.coins.size() - 1)
                    std::cout << ((i == coin_count - 2) ? " && " : " || ");
            }
            std::cout << std::endl
                      << std::endl;
        }
    };

    // All available coins in the world:
    std::unordered_map<std::string, const float> coins_global {
        { "Bitcoin", 22900 },
        { "GreenCoin", 0.5 },
        { "NewCoin", 2 },
        { "Ethereum", 1618 },
        { "Litecoin", 58 },
        { "FileCoin", 8 },
        { "Stablecoin", 0.8 },
        { "Spartacus", 30.4 },
        { "Aurora", 4 },
        { "Tether", 1 },
        { "Polygon", 0.9 },
        { "PaxGold", 1799 },
        { "Solana", 38 },
        { "Metria", 14 },
        { "BNB", 301 }
    };

    // The coins we support
    std::set<std::string> supported_coins { "Tether", "Polygon", "FileCoin", "PaxGold", "Metria", "BNB", "Litecoin", "Spartacus", "GreenCoin" };

    for (const auto& coin : supported_coins)
    {
        if (coins_global.contains(coin))
        {
            add_coin({ coin, coins_global[coin] });
        }
    }

    std::cout << "Questtion 1" << std::endl;
    print_coins(false);
    std::cout << "Questtion 2" << std::endl;
    print_coins(true);
    return 0;
}
