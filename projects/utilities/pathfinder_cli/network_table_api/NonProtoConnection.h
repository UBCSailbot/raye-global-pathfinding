// Non-proto Connection.h
// Copyright 2017 UBC Sailbot

#ifndef NONPROTOCONNECTION_H_
#define NONPROTOCONNECTION_H_

#include "Connection.h"
#include "Exceptions.h"
#include "Node.pb.h"
#include "Value.pb.h"

#include <exception>
#include <map>
#include <string>
#include <utility>
#include <list>
#include <vector>

namespace NetworkTable{
class NonProtoConnection : public Connection {
 public:
    NonProtoConnection();

    /*
     * Set Integer value in the network table, or create
     * it if it doesn't exist.
     */
    void SetIntValue(const std::string &uri, const int &values);

    /*
     * Set multiple Integer values in the network table, or create
     * them if they don't exist.
     * @param values - map from string to Value, where the string
     *                 is the uri, and Value is what to set the
     *                 value at that uri to.
     */
    void SetIntValues(const std::map<std::string, int> &values);

    /*
     * Set Float value in the network table, or create
     * it if it doesn't exist.
     */
    void SetFloatValue(const std::string &uri, const float &values);

    /*
     * Set multiple Float values in the network table, or create
     * them if they don't exist.
     * @param values - map from string to Value, where the string
     *                 is the uri, and Value is what to set the
     *                 value at that uri to.
     */
    void SetFloatValues(const std::map<std::string, float> &values);

    /*
     * Set String value in the network table, or create
     * it if it doesn't exist.
     */
    void SetStringValue(const std::string &uri, const std::string &values);

    /*
     * Set multiple String values in the network table, or create
     * them if they don't exist.
     * @param values - map from string to Value, where the string
     *                 is the uri, and Value is what to set the
     *                 value at that uri to.
     */
    void SetStringValues(const std::map<std::string, std::string> &values);

    /*
     * Set Boolean value in the network table, or create
     * it if it doesn't exist.
     */
    void SetBooleanValue(const std::string &uri, const bool &values);

    /*
     * Set multiple Boolean values in the network table, or create
     * them if they don't exist.
     * @param values - map from string to Value, where the string
     *                 is the uri, and Value is what to set the
     *                 value at that uri to.
     */
    void SetBooleanValues(const std::map<std::string, bool> &values);

    /*
     * Set gps Waypoint value in the network table, or create
     * it if it doesn't exist.
     */
    void SetWaypointValue(const std::pair<double, double> &value);

    /*
     * Set multiple gps Waypoint values in the network table, or create
     * them if they don't exist.
     * @param values - map from string to Value, where the string
     *                 is the uri, and Value is what to set the
     *                 value at that uri to.
     */
    void SetWaypointValues(const std::vector<std::pair<double, double>> &values);


    /*
     * Returns the current gps waypoint coordinates  
     */
    std::list<std::pair<double, double>> GetCurrentWaypoints(); 

	/* 
	 * Returns current gps location
	 */
    std::pair<double, double> GetCurrentGpsCoords(); 

};

}  // namespace NetworkTable

#endif  // NONPROTOCONNECTION_H_
