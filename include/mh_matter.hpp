#ifndef MH_MATTER_HPP
#define MH_MATTER_HPP

#include <esp_err.h>
#include <esp_log.h>

namespace mh_matter {
namespace attribute {
typedef esp_err_t (*callback_t)(uint32_t device_type, uint16_t endpoint_id, uint32_t cluster_id,
                                uint32_t attribute_id, void *val, void *priv_data);
}

namespace identify {
typedef esp_err_t (*callback_t)(uint32_t device_type, uint16_t endpoint_id, void *priv_data);
}

namespace event {
typedef enum {
  READY_TO_PAIR = 0,
  CONNECTING_TO_NETWORK = 1,
  WORKING = 2,
} event_t;

typedef void (*callback_t)(event_t event, void *priv_data);
}  // namespace event

esp_err_t set_attribute_cb(attribute::callback_t cb);

esp_err_t set_identify_cb(identify::callback_t cb);

esp_err_t start(event::callback_t cb, void *priv_data);

esp_err_t factroy_reset();
}  // namespace mh_matter

#endif  // MH_MATTER_HPP