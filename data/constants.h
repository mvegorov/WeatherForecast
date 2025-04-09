#include <vector>
#include <map>

const uint32_t COLUMN_WIDTH=28;
const uint32_t SYMBOL_WIDTH=14;
const std::string DEFAULT_CONFIG=R"("cities":["Saint Petersburg"],"days":7,"frequency":10})";

const std::vector<std::string> EMPTY_FRAME=
{"                                                   ┌─────────────┐                                                   ",
 "┌────────────────────────────┬─────────────────────┤             ├─────────────────────┬────────────────────────────┐",
 "│            night           │          morning    └──────┬──────┘      day            │           evening          │",
 "├────────────────────────────┼────────────────────────────┼────────────────────────────┼────────────────────────────┤",
 "│",
 "│",
 "│",
 "│",
 "│",
 "└────────────────────────────┴────────────────────────────┴────────────────────────────┴────────────────────────────┘"};

const std::map<std::string, std::string> WEATHER_CODES_MEANING = {
        {"0","Clear sky"},
        {"1","Mainly clear"}, {"2","Partly cloudy"}, {"3","Overcast"},
        {"45","Fog"},{"48","Rime fog"},
        {"51","Light drizzle"},{"53","Moderate drizzle"},{"55","Dense drizzle"},
        {"56","Freezing drizzle"},{"57","Freezing drizzle"},
        {"61","Slight rain"},{"63","Moderate rain"},{"65","Dense rain"},
        {"66","Freezing rain"},{"67","Freezing rain"},
        {"71","Light snow fall"},{"73","Snow fall"},{"75","Heavy snow fall"},
        {"77","Snow grains"},
        {"80","Slight showers"},{"81","Moderate shower"},{"82","Violent showers"},
        {"85","Light snow showers"},{"86","Heavy snow showers"},
        {"95","Thunderstorm"},
        {"96","Hail thunderstorm"},{"99","Hail thunderstorm"},
};

const std::map<std::string, std::string> WEATHER_CODES_SYMBOL = {
        {"0","Sunny"},
        {"1","Partly Cloudy"}, {"2","Cloudy"}, {"3","Very Cloudy"},
        {"45","Fog"},{"48","Fog"},
        {"51","Light Rain"},{"53","Light Rain"},{"55","Light Rain"},
        {"56","Light Sleet"},{"57","Light Sleet"},
        {"61","Heavy Rain"},{"63","Heavy Rain"},{"65","Heavy Rain"},
        {"66","Light Sleet"},{"67","Light Sleet"},
        {"71","Light Snow"},{"73","Light Snow"},{"75","Heavy Snow"},
        {"77","Light Snow"},
        {"80","Light Showers"},{"81","Light Showers"},{"82","Heavy Showers"},
        {"85","Light Snow Showers"},{"86","Heavy Snow Showers"},
        {"95","Thundery Heavy Rain"},
        {"96","Thundery Snow"},{"99","Thundery Snow"},
};

std::map<std::string, std::vector<std::string>> WEATHER_SYMBOL = {
        {"Unknown", {
        "    .-.       ",
        "     __)      ",
        "    (         ",
        "     `-’      ",
        "      •       "}},
        {"Sunny", {
        "    \\   /     ",
        "     .-.      ",
        " -- (   ) --  ",
        "     `-’      ",
        "    /   \\     "}},
        {"Partly Cloudy", {
        "   \\  /       ",
        " _ /\"\".-.     ",
        "   \\_(   ).   ",
        "   /(___(__)  ",
        "              "}},
        {"Cloudy", {
        "              ",
        "     .--.     ",
        "  .-(    ).   ",
        " (___.__)__)  ",
        "              "}},
        {"Very Cloudy", {
        "              ",
        "     .--.     ",
        "  .-(    ).   ",
        " (___.__)__)  ",
        "              "}},
        {"Light Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "     ‘ ‘ ‘ ‘  ",
        "    ‘ ‘ ‘ ‘   "}},
        {"Heavy Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "   ‚‘‚‘‚‘‚‘   ",
        "   ‚’‚’‚’‚’   "}},
        {"Light Snow Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "     *  *  *  ",
        "    *  *  *   "}},
        {"Heavy Snow Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "    * * * *   ",
        "   * * * *    "}},
        {"Light Sleet Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "     ‘ * ‘ *  ",
        "    * ‘ * ‘   "}},
        {"Thundery Showers", {
        " _`/\"\".-.     ",
        "  ,\\_(   ).   ",
        "   /(___(__)  ",
        "    ⚡‘ ‘⚡‘ ‘  ",
        "    ‘ ‘ ‘ ‘   "}},
        {"Thundery Heavy Rain", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "  ‚‘⚡‘‚⚡‚‘    ",
        "  ‚’‚’⚡’‚’    "}},
        {"Thundery Snow", {
        "      .-.     ",
        "    _(   ).   ",
        "    (___(__)  ",
        "     *⚡*⚡*    ",
        "    *  *  *   "}},
        {"Light Rain", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "    ‘ ‘ ‘ ‘   ",
        "   ‘ ‘ ‘ ‘    "}},
        {"Heavy Rain", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "  ‚‘‚‘‚‘‚‘    ",
        "  ‚’‚’‚’‚’    "}},
        {"Light Snow", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "    *  *  *   ",
        "   *  *  *    "}},
        {"Heavy Snow", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "   * * * *    ",
        "  * * * *     "}},
        {"Light Sleet", {
        "     .-.      ",
        "    (   ).    ",
        "   (___(__)   ",
        "    ‘ * ‘ *   ",
        "   * ‘ * ‘    "}},
        {"Fog", {
        "              ",
        " _ - _ - _ -  ",
        "  _ - _ - _   ",
        " _ - _ - _ -  ",
        "              "}}};
