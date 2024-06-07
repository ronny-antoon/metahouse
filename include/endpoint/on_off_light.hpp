#ifndef ON_OFF_LIGHT_HPP
#define ON_OFF_LIGHT_HPP

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
        // Namespace for the on/off light endpoint
        namespace on_off_light
        {
            constexpr static const uint32_t DEVICE_TYPE_ID = ESP_MATTER_ON_OFF_LIGHT_DEVICE_TYPE_ID;          // On/off light device type ID
            constexpr static const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_ON_OFF_LIGHT_DEVICE_TYPE_VERSION; // On/off light device type version
            constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::OnOff::Id;

            // Updateable attributes for the on/off light
            constexpr static const uint32_t _ATTRIBUTE_ID = chip::app::Clusters::OnOff::Attributes::OnOff::Id;

            // Configuration structure for the on/off light
            typedef struct config
            {
                mh_matter::cluster::bridged_device_basic_information::config_t
                    bridged_device_basic_information; // used only if the device is a bridged device
                esp_matter::cluster::descriptor::config_t descriptor;
                esp_matter::cluster::identify::config_t identify;
                esp_matter::cluster::groups::config_t groups;
                esp_matter::cluster::scenes_management::config_t scenes_management;
                esp_matter::cluster::on_off::config_t on_off;
            } config_t;

            /** create
             * @brief Create the on/off light
             *
             * @param node Node to create the on/off light on
             * @param config Configuration for the on/off light
             * @param aggregator Aggregator as parent for the on/off light (optional)
             * @param priv_data Private data for the on/off light (optional)
             *
             * @return On/off light
             * @note If the aggregator is provided, the on/off light will be created as bridged device on the aggregator
             *
             */
            esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                                           void *priv_data = nullptr);

        } // namespace on_off_light
    } // namespace endpoint
} // namespace mh_matter

#endif // ON_OFF_LIGHT_HPP