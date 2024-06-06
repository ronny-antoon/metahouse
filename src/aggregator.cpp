#include "endpoint/aggregator.hpp"

#include <on_error.hpp>
#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter.h>

namespace mh_matter::endpoint::aggregator
{
    esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config)
    {
        ON_NULL_PRINT_RETURN(node, nullptr, "Node is null");

        esp_matter::endpoint_t *endpoint =
            esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);
        ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the endpoint");

        esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the device type");

        esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
            endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(descriptor_cluster, nullptr, "Failed to create the descriptor cluster");

        return endpoint;
    }
} // namespace mh_matter::endpoint::aggregator