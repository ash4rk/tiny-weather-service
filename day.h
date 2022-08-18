#include <iostream>
#include <map>
#include <algorithm>
#include <boost/foreach.hpp>

std::vector <Day> GetFiveDaysFromJson(std::string json);        //Forecast parser
std::string GetLatLonFromJson(std::string json, std::string field);            

class Day {
private:
    int dt;                 //day date
    int dayLong;            //sunset - sunrise
    float deltaNightTemp;   //nightTempFeel - nightTemp
public:
    Day() : dt{ 0 }, dayLong{ 0 }, deltaNightTemp{ 0 } {};
    Day(int dt, int dayLong, float deltaNightTemp) : dt{ dt }, dayLong{ dayLong }, deltaNightTemp{ deltaNightTemp } {};
    int GetDt() const { return this->dt; };
    int GetDayLong() const { return this->dayLong; };
    float GetDeltaNightTemp() const { return this->deltaNightTemp; };
    ~Day() {};
};

std::vector <Day> GetFiveDaysFromJson(std::string json)
{
    std::vector <Day> daysList;

    std::stringstream jsomEncoded(json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(jsomEncoded, root);
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type & v, root.get_child("daily."))
    {
        assert(v.first.empty());

        int dt = std::stoi(v.second.get<std::string>("dt"));
        int sunriseTime = stoi(v.second.get<std::string>("sunrise"));
        int sunsetTime = stoi(v.second.get<std::string>("sunset"));

        float nightTemp = stof(v.second.get<std::string>("temp.night"));
        float feelsNightTemp = stof(v.second.get<std::string>("feels_like.night"));

        Day currentDay(dt, sunsetTime - sunriseTime, feelsNightTemp - nightTemp);

        daysList.push_back(currentDay);

    }
    daysList.resize(5);            

    return daysList;
}

std::string GetLatLonFromJson(std::string json, std::string field) 
{
    std::stringstream jsomEncoded(json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(jsomEncoded, root);

    BOOST_FOREACH(boost::property_tree::ptree::value_type & framePair, root) 
    {
        return framePair.second.get<std::string>(field);
    }
}
