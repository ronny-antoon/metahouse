#ifndef FAN_HPP
#define FAN_HPP

#include "cluster/bridged_device_basic_information.hpp"

#include <esp_matter_endpoint.h>
#include <esp_matter_cluster.h>
#include <esp_matter_core.h>

// Main namespace for the project
namespace mh_matter
{
    // Namespace for application endpoints
    namespace endpoint
    {
        // Namespace for the fan endpoint
        namespace fan
        {
            const uint32_t DEVICE_TYPE_ID = ESP_MATTER_FAN_DEVICE_TYPE_ID;          // Fan device type ID
            const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_FAN_DEVICE_TYPE_VERSION; // Fan device type version

            // Configuration structure for the fan
            typedef struct config
            {
                mh_matter::cluster::bridged_device_basic_information::config_t
                    bridged_device_basic_information; // used only if the device is a bridged device
                esp_matter::cluster::descriptor::config_t descriptor;
                esp_matter::cluster::identify::config_t identify;
                esp_matter::cluster::groups::config_t groups;
                esp_matter::cluster::scenes_management::config_t scenes_management;
                esp_matter::cluster::fan_control::config_t fan_control;
            } config_t;

            /** create
             * @brief Create the fan
             *
             * @param node Node to create the fan on
             * @param config Configuration for the fan
             * @param aggregator Aggregator as parent for the fan (optional)
             * @param priv_data Private data for the fan (optional)
             *
             * @return Fan
             * @note If the aggregator is provided, the fan will be created as bridged device on the aggregator
             *
             */
            esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                                           void *priv_data = nullptr);

        } // namespace fan
    } // namespace endpoint
} // namespace mh_matter

#endif // FAN_HPP