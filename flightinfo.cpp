#include "flightinfo.h"

flightInfo::flightInfo(){}
void flightInfo:: fromJson(const QJsonObject &jsonObject)
{
    id = jsonObject["id"].toInt();
    flightNumber = jsonObject["flightNumber"].toString();
    departure = jsonObject["departure"].toString();
    destination = jsonObject["destination"].toString();
    departureTime = jsonObject["departureTime"].toInt();
    arrivalTime = jsonObject["arrivalTime"].toInt();
    firstClassSeats = jsonObject["firstClassSeats"].toInt();
    economyClassSeats = jsonObject["economyClassSeats"].toInt();
    firstClassPrice = jsonObject["firstClassPrice"].toDouble();
    economyClassPrice = jsonObject["economyClassPrice"].toDouble();
    airlineCompany = jsonObject["airlineCompany"].toString();
    aircraftModel = jsonObject["aircraftModel"].toString();
    boardingGate = jsonObject["boardingGate"].toString();
    arrivalAirport = jsonObject["arrivalAirport"].toString();
    departureAirport = jsonObject["departureAirport"].toString();
    hasMeal = jsonObject["hasMeal"].toInt();
    isInternational = jsonObject["isInternational"].toInt();
    luggageSizeLimit = jsonObject["luggageSizeLimit"].toInt();
    businessClassSeats = jsonObject["businessClassSeats"].toInt();
    businessClassPrice = jsonObject["businessClassPrice"].toDouble();
}
