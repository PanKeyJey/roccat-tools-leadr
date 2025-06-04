list_contains(DEVICES leadr contains)
IF(contains)
  add_dbus_bindings(leadr)

  INCLUDE_DIRECTORIES(
    ${CMAKE_SOURCE_DIR}/leadr/libroccatleadr
    ${CMAKE_BINARY_DIR}/leadr
  )

  ADD_SUBDIRECTORY(libroccatleadr)

  ADD_SUBDIRECTORY(roccatleadrconfig)
  ADD_SUBDIRECTORY(roccatleadrcontrol)
  ADD_SUBDIRECTORY(libleadreventhandler)
  ADD_SUBDIRECTORY(libleadrgfxplugin)
ENDIF()
