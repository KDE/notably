include (SopranoAddOntology)
include (KDE4Defaults)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${NEPOMUK_INCLUDE_DIR}
  )

set( SRCS
  main.cpp
  mainwindow.cpp
  notewidget.cpp
  noteedit.cpp
  sidebar.cpp
  notesview.cpp
  notesmodel.cpp
  noteitemdelegate.cpp
  notebrowser.cpp
  mainmenu.cpp
  browsemenu.cpp
  noteinformation.cpp
  config/windowsettings.cpp

  tageditor/tagwidget.cpp
  tageditor/tageditor.cpp
  tageditor/tagcompleter.cpp
  tageditor/tagview.cpp
  tageditor/tagcloud.cpp
  tageditor/tag.cpp
  tageditor/flowlayout.cpp

  porterstemmer.cpp
)

kde4_add_kcfg_files(SRCS config/settings.kcfgc)

kde4_add_ui_files(SRCS
    config/windowsettings.ui
)

kde4_add_executable(notably ${SRCS})

target_link_libraries( notably
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${KDE4_KNOTIFYCONFIG_LIBRARY}
  ${X11_LIBRARIES}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_UTILS_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${SOPRANO_LIBRARIES}
  nepomukdatamanagement
  )

install(TARGETS notably
        DESTINATION ${BIN_INSTALL_DIR})

install(FILES notably.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

