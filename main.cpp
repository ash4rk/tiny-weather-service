#include "client.h"
#include "date.h"
#include "day.h"
#include <iostream>
#include <chrono>
#include <map>
#include <algorithm>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

const std::string API_KEY = "7f13dc3cec2d0541576b1160c7ef37a9";
const std::string API_ARGUMENTS_1 = "/data/2.5/onecall?lat=";
const std::string API_ARGUMENTS_2 = "&exclude=current,minutely,hourly,alerts,&appid=";
const std::string METRIC_UNIT_PARAMETER = "&units=metric";
const std::string GEO_API = "/geo/1.0/direct?q=";
const std::string GEO_LIMIT_PARAMETER = "&limit=5&appid=";



void PrintByUnixTimeStamp(const int& unixTime);                 //Convert + print time


int main()
{
    std::string city, lat, lon;
    std::cout << "Enter your city: " << std::endl;
    std::cin >> city;

    std::string geoResult = Client::GetResponse(GEO_API + city + GEO_LIMIT_PARAMETER + API_KEY);    //Geo api to define lat,lon values for your city
    lat = GetLatLonFromJson(geoResult, "lat");
    lon = GetLatLonFromJson(geoResult, "lon");

    std::string res = Client::GetResponse(API_ARGUMENTS_1 + lat + "&lon=" + lon + API_ARGUMENTS_2 + API_KEY);  //Forecast query
    std::vector <Day> daysList = GetFiveDaysFromJson(res);

    // Get iterator for day with maximum day long
    auto maxDayLong = std::max_element(std::begin(daysList), std::end(daysList),
        [](auto const& lhs, auto const& rhs) -> bool
        {
            return lhs.GetDayLong() < rhs.GetDayLong();
        });

    // Get iterator for day with minimum delta temperature
    auto minDeltaTemp = std::max_element(std::begin(daysList), std::end(daysList),
        [](auto const& lhs, auto const& rhs) -> bool
        {
            return std::abs(lhs.GetDeltaNightTemp()) > std::abs(rhs.GetDeltaNightTemp());
        });


    std::cout << "1)" << std::endl;                                            //Print first case
    PrintByUnixTimeStamp((*minDeltaTemp).GetDt());
    std::cout << " day with minimum difference between feels like and real night temperature. " << std::endl
        << "Diff is: (" << (*minDeltaTemp).GetDeltaNightTemp() << ") Celsium."<< std::endl;

    std::cout << "2)" << std::endl;                                            //Print second case
    PrintByUnixTimeStamp((*maxDayLong).GetDt());
    std::cout << " day with maximum day long." << std::endl << "Day long is : (" << (*maxDayLong).GetDayLong()/3600 
        << " hours, " << ((*maxDayLong).GetDayLong() / 60) % 60 << " minutes and " << ((*maxDayLong).GetDayLong() / 3600) % 60 << " seconds." << std::endl;
    std::cout << (*maxDayLong).GetDayLong();
  
    return 0; 
}

void PrintByUnixTimeStamp(const int& unixTime) 
{
    time_t time = unixTime;
    auto sysTime = std::chrono::system_clock::from_time_t(time);
    auto date = date::year_month_day(floor<date::days>(sysTime));
    std::cout << date;
}