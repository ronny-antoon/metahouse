#include "endpoint/bridge_node.hpp"

#include <on_error.hpp>
#include <esp_matter_core.h>
#include <esp_matter.h>

namespace metahouse::endpoint::bridge_node
{

    esp_matter::endpoint_t *create_bridged_endpoint(esp_matter::node_t *node, esp_matter::endpoint_t *aggregator,
                                                    void *priv_data)
    {
        ON_NULL_PRINT_RETURN(node, nullptr, "Node is null");
        ON_NULL_PRINT_RETURN(aggregator, nullptr, "Aggregator is null");

        esp_matter::endpoint_t *endpoint = esp_matter::endpoint::create(
            node, esp_matter::endpoint_flags::ENDPOINT_FLAG_BRIDGE | esp_matter::endpoint_flags::ENDPOINT_FLAG_DESTROYABLE,
            priv_data);
        ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the endpoint");

        esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the device type");

        err = esp_matter::endpoint::set_parent_endpoint(endpoint, aggregator);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to set the parent endpoint");

        return endpoint;
    }
} // namespace metahouse::endpoint::bridge_node