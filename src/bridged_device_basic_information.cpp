#include "cluster/bridged_device_basic_information.hpp"

#include <on_error.hpp>
#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_attribute.h>
#include <esp_matter.h>

namespace mh_matter::cluster::bridged_device_basic_information
{

    esp_matter::cluster_t *create(esp_matter::endpoint_t *endpoint, config_t *config, esp_matter::cluster_flags_t flags)
    {
        esp_matter::cluster::bridged_device_basic_information::config_t default_config;
        esp_matter::cluster_t *bridged_device_basic_information_cluster =
            esp_matter::cluster::bridged_device_basic_information::create(endpoint, &(default_config),
                                                                          esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(bridged_device_basic_information_cluster, nullptr, "Failed to create the bridged device basic information cluster");

        esp_matter::cluster::bridged_device_basic_information::attribute::create_node_label(
            bridged_device_basic_information_cluster, config->node_label, strlen(config->node_label));

        return bridged_device_basic_information_cluster;
    }

} // namespace mh_matter::clusters::bridged_device_basic_information