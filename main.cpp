#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include <conio.h>

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "data/constants.h"

using json = nlohmann::json;

struct city {
    std::string name;
    std::string latitude;
    std::string longitude;
};

struct forecast {
    std::vector<std::string> weather_symbol;
    std::string weather_name;
    std::string temperature;
    std::string wind_direction;
    std::string wind_speed;
    std::string visibility;
    std::string precipitation;
    std::string probability;
};

std::string DefineType(std::string *str) {
    if (*str == "true" or *str == "false") {
        return "bool";
    }
    int dots = 0;
    for (char ch: *str) {
        if (ch == '.') {
            dots++;
        } else if (!isdigit(ch)) {
            return "varchar";
        }
    }
    if (dots > 1) {
        return "varchar";
    } else if (dots == 1) {
        return "double";
    } else {
        return "int";
    }
}

void Cut(std::string &str) {
    for (int ch = 1; ch < str.size() - 1; ++ch) {
        str[ch - 1] = str[ch];
    }
    str.pop_back();
    str.pop_back();
}

void Fill(std::string &str1, const std::string &str2, size_t str2_actual_size) {
    if (str2_actual_size <= COLUMN_WIDTH) {
        for (char ch: str2) {
            str1.push_back(ch);
        }
        for (int ch = str2_actual_size; ch < COLUMN_WIDTH; ++ch) {
            str1.push_back(' ');
        }
    } else {
        for (int ch = 0; ch < COLUMN_WIDTH - 1; ++ch) {
            str1.push_back(str2[ch]);
        }
        std::string str3 = "…";
        for (char ch: str3) {
            str1.push_back(ch);
        }
    }
}

void InfoToPrettyPaint(json &weather_info, size_t hour, std::vector<std::string> &result) {
    forecast forecast;

    std::string weather_symbol_name;
    if (WEATHER_CODES_SYMBOL.find(weather_info["weathercode"][hour].dump()) == WEATHER_CODES_SYMBOL.end()) {
        weather_symbol_name = "Unknown";
        forecast.weather_name = "Unknown";
    } else {
        weather_symbol_name = WEATHER_CODES_SYMBOL.find(weather_info["weathercode"][hour].dump())->second;
        forecast.weather_name = WEATHER_CODES_MEANING.find(weather_info["weathercode"][hour].dump())->second;
    }
    forecast.weather_symbol = WEATHER_SYMBOL[weather_symbol_name];
    forecast.temperature = weather_info["temperature_2m"][hour].dump();
    int wind_degree = std::stoi(weather_info["winddirection_10m"][hour].dump());
    if (337 < wind_degree or wind_degree < 23) {
        forecast.wind_direction = "↓";
    } else if (wind_degree < 68) {
        forecast.wind_direction = "↙";
    } else if (wind_degree <= 113) {
        forecast.wind_direction = "←";
    } else if (wind_degree < 158) {
        forecast.wind_direction = "↖";
    } else if (wind_degree < 203) {
        forecast.wind_direction = "↑";
    } else if (wind_degree < 248) {
        forecast.wind_direction = "↗";
    } else if (wind_degree < 293) {
        forecast.wind_direction = "→";
    } else {
        forecast.wind_direction = "↘";
    }
    forecast.wind_speed = weather_info["windspeed_10m"][hour].dump();
    forecast.precipitation = weather_info["precipitation"][hour].dump();
    forecast.visibility = std::to_string(std::stoi(weather_info["visibility"][hour].dump()) / 1000) +
                          "." + std::to_string(std::stoi(weather_info["visibility"][hour].dump()) % 1000 / 100);
    forecast.probability = weather_info["precipitation_probability"][hour].dump();

    Fill(result[result.size() - 6], forecast.weather_symbol[0] + forecast.weather_name,
         SYMBOL_WIDTH + forecast.weather_name.size());
    Fill(result[result.size() - 5], forecast.weather_symbol[1] + forecast.temperature + " °C",
         SYMBOL_WIDTH + forecast.temperature.size() + 3);
    Fill(result[result.size() - 4],
         forecast.weather_symbol[2] + forecast.wind_direction + " " + forecast.wind_speed + " m/s",
         SYMBOL_WIDTH + forecast.wind_direction.size() + forecast.wind_speed.size() + 3);
    Fill(result[result.size() - 3], forecast.weather_symbol[3] + forecast.visibility + " km",
         SYMBOL_WIDTH + forecast.visibility.size() + 3);
    Fill(result[result.size() - 2],
         forecast.weather_symbol[4] + forecast.precipitation + " mm | " + forecast.probability + "%",
         SYMBOL_WIDTH + forecast.precipitation.size() + forecast.probability.size() + 7);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    json config;
    std::ifstream fin("data/config.txt");
    if (!fin) {
        std::cerr << "Can't open config file\n";
        config = json::parse(DEFAULT_CONFIG);
    } else {
        config = json::parse(fin);
    }

    if (!config["cities"].is_array() or config["cities"].empty() or !config["days"].is_number_unsigned() or
        !config["frequency"].is_number_unsigned() or config["frequency"] == 0) {
        std::cerr << "Wrong config";
        config = json::parse(DEFAULT_CONFIG);
    }

    std::vector<city> cities;
    for (auto &it: config["cities"]) {
        std::string city_name = it.dump();
        Cut(city_name);  // обрезаем кавычки
        cpr::Response city_response = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                                               cpr::Parameters{{"name", city_name}},
                                               cpr::Header{{"X-Api-Key", "5n2d+JT6PdXaCliVJ2yO3A==fNOOYLqGj5c2eGIN"}});
        json city_info = json::parse(city_response.text);
        std::string latitude = city_info[0]["latitude"].dump();
        std::string longitude = city_info[0]["longitude"].dump();
        cities.push_back({city_name, latitude, longitude});
    }


    uint32_t city_index = 0;
    while (true) {
        cpr::Response weather_response = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                                  cpr::Parameters{
                                                          {"latitude",       cities[city_index].latitude},
                                                          {"longitude",      cities[city_index].longitude},
                                                          {"hourly",         "temperature_2m,precipitation_probability,precipitation,weathercode,visibility,windspeed_10m,winddirection_10m,weathercode"},
                                                          {"windspeed_unit", "ms"},
                                                          {"timezone",       "GMT"},
                                                          {"forecast_days",  "1"}});

        json weather_info = json::parse(weather_response.text);
        std::vector<std::string> response(8);
        response[0] = "Weather forecast: " + cities[city_index].name;
        std::time_t now = std::time(nullptr);
        std::tm *Time = std::gmtime(&now);
        InfoToPrettyPaint(weather_info["hourly"], Time->tm_hour, response);  // погода прямо сейчас
        for (auto &str: response) {
            std::cout << str << '\n';
        }

        std::string days = config["days"].dump();
        weather_response = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                                    cpr::Parameters{
                                            {"latitude",       cities[city_index].latitude},
                                            {"longitude",      cities[city_index].longitude},
                                            {"hourly",         "temperature_2m,precipitation_probability,precipitation,weathercode,visibility,windspeed_10m,winddirection_10m,weathercode"},
                                            {"windspeed_unit", "ms"},
                                            {"timezone",       "auto"},
                                            {"forecast_days",  days}});
        weather_info = json::parse(weather_response.text);
        response = EMPTY_FRAME;
        for (int day = 0; day < std::stoi(config["days"].dump()); ++day) {
            std::string date = weather_info["hourly"]["time"][day * 24];
            int mid = COLUMN_WIDTH * 6 - 6;
            response[1][mid - 5] = date[8];
            response[1][mid - 4] = date[9];
            response[1][mid - 3] = '.';
            response[1][mid - 2] = date[5];
            response[1][mid - 1] = date[6];
            response[1][mid - 0] = ' ';
            response[1][mid + 1] = ' ';
            response[1][mid + 2] = date[0];
            response[1][mid + 3] = date[1];
            response[1][mid + 4] = date[2];
            response[1][mid + 5] = date[3];
            for (int hour = 3; hour < 24; hour += 6) {
                InfoToPrettyPaint(weather_info["hourly"], day * 24 + hour, response);
                // функция записывает прогноз на выбранный час в предпоследние 5 строчек таблицы
                for (int i = 0; i < 5; ++i) {
                    for (char ch: "│") {
                        response[response.size() - 2 - i].push_back(ch);
                    }
                    response[response.size() - 2 - i].pop_back();
                }
            }
            for (auto &str: response) {
                std::cout << str << '\n';
            }
            response = EMPTY_FRAME;
        }

        if (kbhit()) {
            char key = getch();
            if (key == '+') {
                if (days != "16") {
                    days = std::to_string(std::stoi(days) + 1);
                }
            } else if (key == '-') {
                if (days != "0") {
                    days = std::to_string(std::stoi(days) - 1);
                }
            } else if (key == 'n') {
                if (city_index != cities.size() - 1) {
                    city_index++;
                }
            } else if (key == 'p') {
                if (city_index != 0) {
                    city_index--;
                }
            } else if (key == 27) {  // esc
                break;
            }
        }

        uint32_t sleep = std::stoll(config["frequency"].dump());
        std::this_thread::sleep_for(std::chrono::seconds(sleep));
        system("cls");
    }
}