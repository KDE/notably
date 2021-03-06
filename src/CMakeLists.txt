include (SopranoAddOntology)
include (KDE4Defaults)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${NEPOMUK_CORE_INCLUDE_DIR}
  )

set( SRCS
  main.cpp
  mainwindow.cpp
  notewidget.cpp
  noteedit.cpp
  notedocument.cpp
  annotationgrouptextobject.cpp
  annotationgroupwidget.cpp
  sidebar.cpp
  notesview.cpp
  notesmodel.cpp
  noteitemdelegate.cpp
  notebrowser.cpp
  mainmenu.cpp
  browsemenu.cpp
  noteinformation.cpp
  config/windowsettings.cpp

  resourcemodel.cpp
  simpleresourcemodel.cpp

  tags/tagwidget.cpp
  tags/tageditor.cpp
  tags/tagcompleter.cpp
  tags/tagview.cpp
  tags/tagcloud.cpp
  tags/tag.cpp
  tags/flowlayout.cpp

  person/person.cpp
  person/personmodel.cpp
  person/persondelegate.cpp
  person/persondelegate.cpp
  person/persontooltip.cpp
  person/persongrid.cpp
  person/personcompleter.cpp

  annotation/annotation.cpp
  annotation/annotationplugin.cpp
  annotation/annotationpluginfactory.cpp
  annotation/annotationpluginwrapper.cpp
  annotation/annotationrequest.cpp

  annotation/textannotation.cpp
  annotation/textannotationgroup.cpp
  annotation/annotator.cpp

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
  ${NEPOMUK_CORE_LIBRARY}
  ${SOPRANO_LIBRARIES}
  )

install(TARGETS notably
        DESTINATION ${BIN_INSTALL_DIR})

install(FILES notably.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

add_subdirectory( annotation )
add_subdirectory( person )

