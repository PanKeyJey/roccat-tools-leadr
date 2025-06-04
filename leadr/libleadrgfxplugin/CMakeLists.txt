ADD_LIBRARY(libleadrgfxplugin SHARED
  leadrgfxplugin.c
)

TARGET_LINK_LIBRARIES(libleadrgfxplugin
  libroccat
  libroccathelper
  ${DBUS_LIBRARIES}
  ${GLIB2_LIBRARIES}
)

SET_TARGET_PROPERTIES(libleadrgfxplugin
  PROPERTIES
    OUTPUT_NAME leadrgfxplugin
    COMPILE_FLAGS "-DG_LOG_DOMAIN=\\\"leadrGfxplugin\\\" -fvisibility=hidden"
)

INSTALL(TARGETS libleadrgfxplugin DESTINATION ${GAMINGGEAR0_FX_PLUGIN_DIR})
