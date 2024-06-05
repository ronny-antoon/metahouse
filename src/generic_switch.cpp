#include "endpoint/generic_switch.hpp"
#include "endpoint/bridge_node.hpp"
#include "cluster/bridged_device_basic_information.hpp"

#include <on_error.hpp>
#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_command.h>
#include <esp_matter.h>

namespace metahouse::endpoint::generic_switch
{
    esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator,
                                   void *priv_data)
    {
        ON_NULL_PRINT_RETURN(node, nullptr, "Node is null");
        esp_matter::endpoint_t *endpoint;
        // If the aggregator is not null, create a bridged endpoint
        if (aggregator != nullptr)
        {
            endpoint = metahouse::endpoint::bridge_node::create_bridged_endpoint(node, aggregator, priv_data);
            ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the bridged endpoint");

            metahouse::cluster::bridged_device_basic_information::create(
                endpoint, &(config->bridged_device_basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        }
        else
        { // If the aggregator is null, create a standalone endpoint
            endpoint = esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, priv_data);
            ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the endpoint");
        }

        esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the device type");

        esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
            endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(descriptor_cluster, nullptr, "Failed to create the descriptor cluster");

        esp_matter::cluster_t *identify_cluster = esp_matter::cluster::identify::create(
            endpoint, &(config->identify), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(identify_cluster, nullptr, "Failed to create the identify cluster");
        esp_matter::command_t *trigger_effect_command =
            esp_matter::cluster::identify::command::create_trigger_effect(identify_cluster);
        ON_NULL_PRINT_RETURN(trigger_effect_command, nullptr, "Failed to create the trigger effect command");

        esp_matter::cluster_t *switch_cluster = esp_matter::cluster::switch_cluster::create(
            endpoint, &(config->switch_cluster), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(switch_cluster, nullptr, "Failed to create the switch cluster");

        // Add Single press features to the switch cluster
        err = esp_matter::cluster::switch_cluster::feature::momentary_switch::add(switch_cluster);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the momentary switch feature");
        err = esp_matter::cluster::switch_cluster::feature::momentary_switch_release::add(switch_cluster);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the momentary switch release feature");

        // Add Long press features to the switch cluster
        err = esp_matter::cluster::switch_cluster::feature::momentary_switch_long_press::add(switch_cluster);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the momentary switch long press feature");

        // Add Multi press features to the switch cluster(Double press)
        esp_matter::cluster::switch_cluster::feature::momentary_switch_multi_press::config_t doublepress;
        err = esp_matter::cluster::switch_cluster::feature::momentary_switch_multi_press::add(switch_cluster, &doublepress);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the momentary switch multi press feature");

        return endpoint;
    }
} // namespace metahouse::endpoint::generic_switch