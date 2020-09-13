find_path(nlohmann_json_INCLUDE_DIR NAMES nlohmann/json.hpp REQUIRED)

set(nlohmann_json_FOUND TRUE)
set(nlohmann_json_INCLUDE_DIRS ${nlohmann_json_INCLUDE_DIR})
message(STATUS "nlohmann_json_INCLUDE_DIR: ${nlohmann_json_INCLUDE_DIR}")

add_library(nlohmann_json::nlohmann_json INTERFACE IMPORTED)
set_target_properties(nlohmann_json::nlohmann_json PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${nlohmann_json_INCLUDE_DIR}"
)
