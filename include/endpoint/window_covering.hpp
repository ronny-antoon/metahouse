#ifndef WINDOW_COVERING_HPP
#define WINDOW_COVERING_HPP

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
        // Namespace for the window covering
        namespace window_covering
        {
            constexpr static const uint32_t DEVICE_TYPE_ID = ESP_MATTER_WINDOW_COVERING_DEVICE_TYPE_ID;          // Window covering device type ID
            constexpr static const uint8_t DEVICE_TYPE_VERSION = ESP_MATTER_WINDOW_COVERING_DEVICE_TYPE_VERSION; // Window covering device type version
            constexpr static const uint32_t _CLUSTER_ID = chip::app::Clusters::WindowCovering::Id;               // Window covering cluster ID

            // Updateable attributes for the window covering
            constexpr static const uint32_t _ATTRIBUTE_TARGET_ID =
                chip::app::Clusters::WindowCovering::Attributes::TargetPositionLiftPercent100ths::Id;
            constexpr static const uint32_t _ATTRIBUTE_CURRENT_ID =
                chip::app::Clusters::WindowCovering::Attributes::CurrentPositionLiftPercent100ths::Id;

            // Configuration structure for the window covering
            typedef struct config
            {
                mh_matter::cluster::bridged_device_basic_information::config_t
                    bridged_device_basic_information; // used only if the device is a bridged device
                esp_matter::cluster::descriptor::config_t descriptor;
                esp_matter::cluster::identify::config_t identify;
                esp_matter::cluster::groups::config_t groups;
                esp_matter::cluster::scenes_management::config_t scenes_management;
                esp_matter::cluster::window_covering::config_t window_covering;
            } config_t;

            /** create
             * @brief Create the window covering
             *
             * @param node Node to create the window covering on
             * @param config Configuration for the window covering
             * @param aggregator Aggregator as parent for the window covering (optional)
             * @param priv_data Private data for the window covering (optional)
             *
             * @return Window covering
             * @note If the aggregator is provided, the window covering will be created as bridged device on the aggregator
             *
             */
            esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator = nullptr,
                                           void *priv_data = nullptr);

        } // namespace window_covering
    } // namespace endpoint
} // namespace mh_matter

#endif // WINDOW_COVERING_HPP