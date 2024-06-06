#ifndef BRIDGED_DEVICE_BASIC_INFORMATION_HPP
#define BRIDGED_DEVICE_BASIC_INFORMATION_HPP

#include <app-common/zap-generated/ids/Clusters.h>
#include <esp_matter_core.h>
#include <esp_matter.h>

namespace mh_matter
{
    namespace cluster
    {
        namespace bridged_device_basic_information
        {

            constexpr uint32_t CLUSTER_ID = chip::app::Clusters::BridgedDeviceBasicInformation::Id;
            constexpr uint8_t MAX_NAME_LENGTH = 32;

            typedef struct config
            {
                char node_label[MAX_NAME_LENGTH + 1]; // mandatory
                config(char *node_label = "node_label") { strncpy(this->node_label, node_label, MAX_NAME_LENGTH); }

            } config_t;

            esp_matter::cluster_t *create(esp_matter::endpoint_t *endpoint, config_t *config, esp_matter::cluster_flags_t flags);
        } // namespace bridged_device_basic_information
    } // namespace clusters
} // namespace mh_matter

#endif // BRIDGED_DEVICE_BASIC_INFORMATION_HPP