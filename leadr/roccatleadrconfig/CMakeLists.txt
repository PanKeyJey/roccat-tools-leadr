ADD_EXECUTABLE(roccatleadrconfig
  leadrconfig.c
  leadrconfig_window.c
  leadr_2d_positional.c
  leadr_buttons_frame.c
  leadr_color_frame.c
  leadr_configuration_dialog.c
  leadr_cpi_selector.c
  leadr_dcu_frame.c
  leadr_info_dialog.c
  leadr_key_combo_box.c
  leadr_light_effects_frame.c
  leadr_profile_page.c
  leadr_tcu_calibration_assistant.c
  leadr_tcu_dcu_dialog.c
  leadr_tcu_frame.c
  leadr_xcelerator_calibration_assistant.c
)

TARGET_LINK_LIBRARIES(roccatleadrconfig
  libroccatleadr
  libroccat
  libroccatwidget
  ${GTK_LIBRARIES}
  ${GAMINGGEAR0_LIBRARY}
  ${GAMINGGEAR0_WIDGET_LIBRARY}
  ${DBUS-GLIB_LIBRARIES}
)

INSTALL(TARGETS roccatleadrconfig
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES roccatleadrconfig.desktop
  DESTINATION share/applications
)
