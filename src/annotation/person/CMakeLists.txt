kde4_add_plugin(notably_personannotationplugin
  personannotationplugin.cpp
  )

target_link_libraries(notably_personannotationplugin
  ${NEPOMUK_LIBRARIES}
  ${SOPRANO_LIBRARIES}
  ${SOPRANO_CLIENT_LIBRARIES}
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  nepomukannotation
)

install(TARGETS notably_personannotationplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES personannotationplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR}/nepomukannotationplugins)
