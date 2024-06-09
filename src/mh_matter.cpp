#include <app/server/CommissioningWindowManager.h>
#include <app/server/Server.h>
#include <esp_matter.h>

#include <mh_matter.hpp>
#include <on_error.hpp>

namespace mh_matter {
uint32_t get_device_type(uint16_t endpoint_id) {
  uint32_t *deviceTypeIdArray = nullptr;
  uint8_t deviceTypeIdArraySize = 0;
  esp_matter::node_t *node = esp_matter::node::get();
  esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(node, endpoint_id);
  deviceTypeIdArray = esp_matter::endpoint::get_device_type_ids(endpoint, &deviceTypeIdArraySize);
  if (deviceTypeIdArraySize <= 0 || deviceTypeIdArray == nullptr) {
    return 0;
  }

  // If the device is a Matter Aggregator, Bridged Node or Root Node, the device type is the second element in
  // the array
  if (deviceTypeIdArraySize != 1 && (deviceTypeIdArray[0] == ESP_MATTER_AGGREGATOR_DEVICE_TYPE_ID ||
                                     deviceTypeIdArray[0] == ESP_MATTER_BRIDGED_NODE_DEVICE_TYPE_ID ||
                                     deviceTypeIdArray[0] == ESP_MATTER_ROOT_NODE_DEVICE_TYPE_ID)) {
    return deviceTypeIdArray[1];
  } else {
    return deviceTypeIdArray[0];
  }
}
attribute::callback_t attribute_cb_external = nullptr;
esp_err_t attribute_cb_internal(esp_matter::attribute::callback_type type, uint16_t endpoint_id,
                                uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val,
                                void *priv_data) {
  if (type == esp_matter::attribute::callback_type_t::PRE_UPDATE) {
    return attribute_cb_external(get_device_type(endpoint_id), endpoint_id, cluster_id, attribute_id, val,
                                 priv_data);
  }

  return ESP_OK;
}
esp_err_t set_attribute_cb(attribute::callback_t cb) {
  attribute_cb_external = cb;
  return esp_matter::attribute::set_callback(attribute_cb_internal);
}

identify::callback_t identify_cb_external = nullptr;
esp_err_t identify_cb_internal(esp_matter::identification::callback_type type, uint16_t endpoint_id,
                               uint8_t effect_id, uint8_t effect_variant, void *priv_data) {
  if (type == esp_matter::identification::callback_type_t::START) {
    return identify_cb_external(get_device_type(endpoint_id), endpoint_id, priv_data);
  }
  return ESP_OK;
}

esp_err_t set_identify_cb(identify::callback_t cb) {
  identify_cb_external = cb;
  return esp_matter::identification::set_callback(identify_cb_internal);
}

event::callback_t event_cb_external = nullptr;
void *event_cb_priv_data = nullptr;
void event_cb_internal(const chip::DeviceLayer::ChipDeviceEvent *event, intptr_t arg) {
  switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
      ESP_LOGW(__FILENAME__, "Interface IP Address changed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      event_cb_external(event::event_t::WORKING, event_cb_priv_data);
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
      ESP_LOGW(__FILENAME__, "Commissioning complete");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      event_cb_external(event::event_t::CONNECTING_TO_NETWORK, event_cb_priv_data);
      break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
      ESP_LOGW(__FILENAME__, "Commissioning failed, fail safe timer expired");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
      ESP_LOGW(__FILENAME__, "Commissioning session started");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
      ESP_LOGW(__FILENAME__, "Commissioning session stopped");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
      ESP_LOGW(__FILENAME__, "Commissioning window opened");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      event_cb_external(event::event_t::READY_TO_PAIR, event_cb_priv_data);
      break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
      ESP_LOGW(__FILENAME__, "Commissioning window closed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricRemoved: {
      ESP_LOGW(__FILENAME__, "Fabric removed successfully");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      if (chip::Server::GetInstance().GetFabricTable().FabricCount() == 0) {
        chip::CommissioningWindowManager &commissionMgr =
            chip::Server::GetInstance().GetCommissioningWindowManager();
        constexpr auto kTimeoutSeconds = chip::System::Clock::Seconds16(300);
        if (!commissionMgr.IsCommissioningWindowOpen()) {
          CHIP_ERROR err = commissionMgr.OpenBasicCommissioningWindow(
              kTimeoutSeconds, chip::CommissioningWindowAdvertisement::kDnssdOnly);
          if (err != CHIP_NO_ERROR) {
            ON_ERR_PRINT(ESP_ERR_NOT_FINISHED, "Failed to open commissioning window, err:%" CHIP_ERROR_FORMAT,
                         err.Format());
          }
        }
      }
      break;
    }

    case chip::DeviceLayer::DeviceEventType::kFabricWillBeRemoved:
      ESP_LOGW(__FILENAME__, "Fabric will be removed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricUpdated:
      ESP_LOGW(__FILENAME__, "Fabric is updated");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;

    case chip::DeviceLayer::DeviceEventType::kFabricCommitted:
      ESP_LOGW(__FILENAME__, "Fabric is committed");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      ESP_LOGW(__FILENAME__, "---------------------------------");
      break;
    default:
      break;
  }
}

esp_err_t start(event::callback_t cb = nullptr, void *priv_data = nullptr) {
  event_cb_external = cb;
  event_cb_priv_data = priv_data;
  return esp_matter::start(event_cb_internal);
}

esp_err_t factroy_reset() { return ESP_OK; }
}  // namespace mh_matter