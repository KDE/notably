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
  noteedit.cpp
)

kde4_add_executable(nnotes ${SRCS})

target_link_libraries( nnotes
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${NEPOMUK_LIBRARIES}
  ${NEPOMUK_QUERY_LIBRARIES}
  ${SOPRANO_LIBRARIES}
  nepomukdatamanagement
  )

install(TARGETS nnotes
        DESTINATION ${BIN_INSTALL_DIR})
