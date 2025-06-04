FOREACH(language ${TRANSLATION_LANGUAGES})
  INSTALL(FILES roccatleadrcontrol.${language}.1
    DESTINATION share/man/${language}/man1
    RENAME roccatleadrcontrol.1
  )
ENDFOREACH()