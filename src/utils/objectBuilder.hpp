#pragma once

#include <string>
#include <map>
#include <fmt/format.h>
#include <Geode/utils/string.hpp>
#include "utils.hpp"

#define OBJ_ID 1
#define OBJ_X 2
#define OBJ_Y 3
#define OBJ_SCALE 32

class ObjectBuilder
{
    std::map<unsigned int, std::string> m_properties;

    public:
        ObjectBuilder() {}
        ObjectBuilder(unsigned int objID) {
            setID(objID);
        }
        ObjectBuilder(float x, float y) {
            setX(x);
            setY(y);
        }
        ObjectBuilder(unsigned int objID, float x, float y) {
            setID(objID);
            setX(x);
            setY(y);
        }

        /**
         * Adds a property to the object
         * @param propertyID the property ID to add
         * @param value the value to link to the property ID
         */
        inline void addProperty(unsigned int propertyID, std::string value) {
            m_properties[propertyID] = value;
        }

        /**
         * Adds a property to the object
         * @param propertyID the property ID to add
         * @param value the value to link to the property ID
         */
        template<typename T>
        inline void addProperty(unsigned int propertyID, T value) {
            m_properties[propertyID] = fmt::to_string(value);
        }

        /**
         * Removes a property from the property list
         * @param propertyID the property ID to remove
         */
        inline void removeProperty(unsigned int propertyID) {
            m_properties.erase(propertyID);
        }

        // ----

        /**
         * Sets the id of the current object
         * @note this is the same as doing `.addProperty(1, objID)`
         * @param objID the object's ID
         */
        inline void setID(unsigned int objID) {
            m_properties[OBJ_ID] = objID;
        }

        /**
         * Sets the id of the current object
         * @note this is the same as doing `.addProperty(2, x)`
         * @param x the object's x pos
         */
        inline void setX(float x) {
            m_properties[OBJ_X] = x;
        }

        /**
         * Sets the id of the current object
         * @note this is the same as doing `.addProperty(3, y)`
         * @param y the object's y pos
         */
        inline void setY(float y) {
            m_properties[OBJ_Y] = y;
        }

        /**
         * Sets the id of the current object
         * @note this is the same as doing `.addProperty(32, scale)`
         * @param scale the object's scale
         */
        inline void setScale(float scale) {
            m_properties[OBJ_SCALE] = scale;
        }

        // ----

        /**
         * @return the property map
         */
        inline std::map<unsigned int, std::string> getProperties() {
            return m_properties;
        }

        /**
         * Gives the object string
         * @example `object.setID(2); object.getObjectString()` -> `1,2`
         */
        inline std::string getObjectString() {
            if (m_properties.empty()) { return ""; }

            std::string out;

            for (const auto& [k, v] : m_properties) {
                out += fmt::format(
                    "{},{},", fmt::to_string(k), v
                );
            }
            
            return out;
        }
};