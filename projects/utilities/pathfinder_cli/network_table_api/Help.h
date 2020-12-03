// Copyright 2017 UBC Sailbot

#ifndef HELP_H_
#define HELP_H_

#include <string>

#include "Uccms.pb.h"
#include "Sensors.pb.h"
#include "Value.pb.h"
#include "Node.pb.h"

namespace NetworkTable {

void PrintNode(const NetworkTable::Node &root);

/*
 * Returns node at given uri.
 * Does not modify root (I can't get use const though for reasons)
 * @param uri - path to the node to get, seperated by '/'.
 *              eg. "/gps/lat" or "gps/lat"
 * @param root - root node that uri indexes into
 * @throws - NodeNotFoundException if the node at the uri doesn't exist
 */
NetworkTable::Node GetNode(std::string uri, NetworkTable::Node *root);

/*
 * Sets a given node in the tree. Creates the intermediate nodes
 * if they don't exist.
 * @param uri - path to the node to set, seperated by '/'.
 *              eg. "/gps/lat" or "gps/lat"
 * @param root - root node that uri indexes into. This will be modified.
 * @param value - The value at the given uri.
 */
void SetNode(std::string uri, NetworkTable::Value value, NetworkTable::Node *root);

/*
 * Writes a node to disk.
 */
void Write(std::string filepath, const NetworkTable::Node &root);

/*
 * Loads a node from disk.
 */
NetworkTable::Node Load(const std::string &filepath);

/*
 * Converts Node.proto to Sensor.proto
 * Does not modify root (I can't get use const though for reasons)
 */
NetworkTable::Sensors RootToSensors(NetworkTable::Node *root);

/*
 * Converts Sensor.proto to Node.proto
 */
NetworkTable::Node SensorsToRoot(const NetworkTable::Sensors &sensors);

/*
 * Converts Node.proto to Uccms.proto
 * Does not modify root (I can't get use const though for reasons)
 */
NetworkTable::Uccms RootToUccms(NetworkTable::Node *root);

/*
 * Converts Uccms.proto to Node.proto
 */
NetworkTable::Node UccmsToRoot(const NetworkTable::Uccms &uccms);

}  // namespace NetworkTable

#endif  // HELP_H_
