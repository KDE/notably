project(nepomuknotes)

find_package(KDE4 REQUIRED)
find_package(Nepomuk REQUIRED)

include(SopranoAddOntology)
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
  titlebar.cpp
  tageditor.cpp
  tagcompleter.cpp
  sidebar.cpp
  notesview.cpp
  notesmodel.cpp
  noteitemdelegate.cpp
  config/windowsettings.cpp

  porterstemmer.cpp
)

kde4_add_kcfg_files(SRCS config/settings.kcfgc)

kde4_add_ui_files(SRCS
    config/windowsettings.ui
)

kde4_add_executable(nnotes ${SRCS})

target_link_libraries( nnotes
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

install(TARGETS nnotes
        DESTINATION ${BIN_INSTALL_DIR})
