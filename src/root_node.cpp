#include "endpoint/root_node.hpp"

#include <esp_matter.h>
#include <esp_matter_core.h>

#include <on_error.hpp>

namespace mh_matter {
namespace endpoint {
namespace root_node {
esp_matter::node_t *create(config_t *config) {
  esp_matter::node_t *node = esp_matter::node::create_raw();
  ON_NULL_PRINT_RETURN(node, nullptr, "Failed to create the root node");

  esp_matter::endpoint_t *endpoint =
      esp_matter::endpoint::create(node, esp_matter::endpoint_flags::ENDPOINT_FLAG_NONE, nullptr);
  ON_NULL_PRINT_RETURN(endpoint, nullptr, "Failed to create the endpoint");

  esp_err_t err = esp_matter::endpoint::add_device_type(endpoint, DEVICE_TYPE_ID, DEVICE_TYPE_VERSION);
  ON_ERR_PRINT_RETURN(err, nullptr, "Failed to add the device type");

  esp_matter::cluster_t *descriptor_cluster = esp_matter::cluster::descriptor::create(
      endpoint, &(config->descriptor), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(descriptor_cluster, nullptr, "Failed to create the descriptor cluster");

  esp_matter::cluster_t *access_control_cluster = esp_matter::cluster::access_control::create(
      endpoint, &(config->access_control), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(access_control_cluster, nullptr, "Failed to create the access control cluster");

  esp_matter::cluster_t *basic_information_cluster = esp_matter::cluster::basic_information::create(
      endpoint, &(config->basic_information), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(basic_information_cluster, nullptr, "Failed to create the basic information cluster");

  esp_matter::cluster_t *general_commissioning_cluster = esp_matter::cluster::general_commissioning::create(
      endpoint, &(config->general_commissioning), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(general_commissioning_cluster, nullptr,
                       "Failed to create the general commissioning cluster");

  esp_matter::cluster_t *network_commissioning_cluster = esp_matter::cluster::network_commissioning::create(
      endpoint, &(config->network_commissioning), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(network_commissioning_cluster, nullptr,
                       "Failed to create the network commissioning cluster");

  esp_matter::cluster_t *general_diagnostics_cluster = esp_matter::cluster::general_diagnostics::create(
      endpoint, &(config->general_diagnostics), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(general_diagnostics_cluster, nullptr,
                       "Failed to create the general diagnostics cluster");

  esp_matter::cluster_t *administrator_commissioning_cluster =
      esp_matter::cluster::administrator_commissioning::create(
          endpoint, &(config->administrator_commissioning), esp_matter::cluster_flags::CLUSTER_FLAG_SERVER,
          ESP_MATTER_NONE_FEATURE_ID);
  ON_NULL_PRINT_RETURN(administrator_commissioning_cluster, nullptr,
                       "Failed to create the administrator commissioning cluster");

  esp_matter::cluster_t *operational_credentials_cluster =
      esp_matter::cluster::operational_credentials::create(endpoint, &(config->operational_credentials),
                                                           esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(operational_credentials_cluster, nullptr,
                       "Failed to create the operational credentials cluster");

  esp_matter::cluster_t *group_key_management_cluster = esp_matter::cluster::group_key_management::create(
      endpoint, esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(group_key_management_cluster, nullptr,
                       "Failed to create the group key management cluster");

  esp_matter::cluster_t *diagnostics_network_wifi_cluster =
      esp_matter::cluster::diagnostics_network_wifi::create(endpoint, &(config->diagnostics_network_wifi),
                                                            esp_matter::cluster_flags::CLUSTER_FLAG_SERVER);
  ON_NULL_PRINT_RETURN(diagnostics_network_wifi_cluster, nullptr,
                       "Failed to create the diagnostics network wifi cluster");

  return node;
}
}  // namespace root_node
}  // namespace endpoint
}  // namespace mh_matter