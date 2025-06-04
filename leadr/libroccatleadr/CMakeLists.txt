ADD_LIBRARY(libroccatleadr SHARED
  leadr_config.c
  leadr_control_unit.c
  leadr_dbus_services.c
  leadr_device.c
  leadr_device_state.c
  leadr_gfx.c
  leadr_info.c
  leadr_macro.c
  leadr_profile_buttons.c
  leadr_profile.c
  leadr_profile_settings.c
  leadr_rmp.c
  leadr_rmp_accessors.c
  leadr_rmp_conversions.c
  leadr_rmp_high_level.c
  leadr_rmp_light_info.c
  leadr_rmp_macro_key_info.c
  leadr_sensor.c
  leadr_talk.c
  leadr_xcelerator.c
)

ADD_DEPENDENCIES(libroccatleadr
  generated_headers
  leadr_dbus_bindings
)

TARGET_LINK_LIBRARIES(libroccatleadr
  libroccat
  libroccathelper
  ${DBUS-GLIB_LIBRARIES}
  ${USB1_LIBRARIES}
  ${GAMINGGEAR0_LIBRARY}
  ${GLIB2_LIBRARIES}
)

SET_TARGET_PROPERTIES(libroccatleadr
  PROPERTIES
    VERSION ${V_MAJOR}.${V_MINOR}.${V_PATCH}
    SOVERSION ${V_MAJOR}
    OUTPUT_NAME roccatleadr
    COMPILE_FLAGS "-DG_LOG_DOMAIN=\\\"leadr\\\""
)

INSTALL(TARGETS libroccatleadr DESTINATION ${LIBDIR})
