#include "endpoint/window_covering.hpp"
#include "endpoint/bridge_node.hpp"
#include "cluster/bridged_device_basic_information.hpp"

#include <on_error.hpp>
#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_command.h>
#include <esp_matter.h>

namespace mh_matter::endpoint::window_covering
{
    esp_matter::endpoint_t *create(esp_matter::node_t *node, config_t *config, esp_matter::endpoint_t *aggregator,
                                   void *priv_data)
    {
        ON_NULL_PRINT_RETURN(node, nullptr, "Node is null");
        esp_matter::endpoint_t *endpoint;
        // If the aggregator is not null, create a bridged endpoint
        if (aggregator != nullptr)
        {
            endpoint = mh_matter::endpoint::bridge_node::create_bridged_endpoint(node, aggregator, priv_data);
            ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the bridged endpoint");

            mh_matter::cluster::bridged_device_basic_information::create(
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

        esp_matter::cluster_t *groups_cluster = esp_matter::cluster::groups::create(
            endpoint, &(config->groups), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(groups_cluster, nullptr, "Failed to create the groups cluster");

        esp_matter::cluster_t *scenes_cluster = esp_matter::cluster::scenes_management::create(
            endpoint, &(config->scenes_management), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
        ON_NULL_PRINT_RETURN(scenes_cluster, nullptr, "Failed to create the scenes cluster");

        esp_matter::cluster_t *window_covering_cluster = esp_matter::cluster::window_covering::create(
            endpoint, &(config->window_covering), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER,
            ESP_MATTER_NONE_FEATURE_ID);
        ON_NULL_PRINT_RETURN(window_covering_cluster, nullptr, "Failed to create the window covering cluster");

        // Add the lift feature to the window covering cluster
        esp_matter::cluster::window_covering::feature::lift::config_t lift_config;
        err = esp_matter::cluster::window_covering::feature::lift::add(window_covering_cluster, &lift_config);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the lift feature");

        // Add the position aware lift feature to the window covering cluster
        esp_matter::cluster::window_covering::feature::position_aware_lift::config_t position_aware_lift_config;
        nullable<uint8_t> percentage = nullable<uint8_t>(0);
        nullable<uint16_t> percentage_100ths = nullable<uint16_t>(0);
        position_aware_lift_config.current_position_lift_percentage = percentage;
        position_aware_lift_config.target_position_lift_percent_100ths = percentage_100ths;
        position_aware_lift_config.current_position_lift_percent_100ths = percentage_100ths;
        err = esp_matter::cluster::window_covering::feature::position_aware_lift::add(window_covering_cluster,
                                                                                      &position_aware_lift_config);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the position aware lift feature");

        // Add the absolute position feature to the window covering cluster
        esp_matter::cluster::window_covering::feature::absolute_position::config_t absolute_position_config;
        err = esp_matter::cluster::window_covering::feature::absolute_position::add(window_covering_cluster,
                                                                                    &absolute_position_config);
        ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the absolute position feature");

        return endpoint;
    }
} // namespace mh_matter::endpoint::window_covering