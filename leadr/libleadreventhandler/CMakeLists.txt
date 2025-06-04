ADD_LIBRARY(libleadreventhandler SHARED
  leadr_dbus_server.c
  leadr_eventhandler.c
  leadr_eventhandler_channel.c
)

ADD_DEPENDENCIES(libleadreventhandler
  generated_headers
  leadr_dbus_bindings
)

SET_TARGET_PROPERTIES(libleadreventhandler
  PROPERTIES
    OUTPUT_NAME leadreventhandler
    COMPILE_FLAGS "-DG_LOG_DOMAIN=\\\"leadrEventhandler\\\" -fvisibility=hidden"
)

TARGET_LINK_LIBRARIES(libleadreventhandler
  libroccat
  libroccatleadr
  libroccathelper
  ${GAMINGGEAR0_LIBRARY}
  ${DBUS-GLIB_LIBRARIES}
  ${GLIB2_LIBRARIES}
)

INSTALL(TARGETS libleadreventhandler DESTINATION ${EVENTHANDLER_PLUGIN_DIR})
