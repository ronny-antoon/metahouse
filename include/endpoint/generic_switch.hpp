#ifndef GENERIC_SWITCH_HPP
#define GENERIC_SWITCH_HPP

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
        // Namespace for the generic switch endpoint
        namespace generic_switch
        {
            constexpr static const uint32_t DEVICE_TYPE_ID = ESP_MATTER_GENERIC_SWITCH_DEVICE_TYPE_ID;          // Generic switch device type ID
            constexpr static const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_GENERIC_SWITCH_DEVICE_TYPE_VERSION; // Generic switch device type version
            constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::Switch::Id;

            // Updateable attributes for the generic switch
            constexpr static const uint32_t _ATTRIBUTE_ID = chip::app::Clusters::Switch::Attributes::CurrentPosition::Id;

            // Configuration structure for the generic switch
            typedef struct config
            {
                mh_matter::cluster::bridged_device_basic_information::config_t
                    bridged_device_basic_information; // used only if the device is a bridged device
                esp_matter::cluster::descriptor::config_t descriptor;
                esp_matter::cluster::identify::config_t identify;
                esp_matter::cluster::switch_cluster::config_t switch_cluster;
            } config_t;

            /** create
             * @brief Create the generic switch
             *
             * @param node Node to create the generic switch on
             * @param config Configuration for the generic switch
             * @param aggregator Aggregator as parent for the generic switch (optional)
             * @param priv_data Private data for the generic switch (optional)
             *
             * @return Generic switch
             * @note If the aggregator is provided, the generic switch will be created as bridged device on the aggregator
             *
             */
            esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                                           void *priv_data = nullptr);

        } // namespace generic_switch
    } // namespace endpoint
} // namespace mh_matter

#endif // GENERIC_SWITCH_HPP