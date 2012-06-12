project(nepomuk-personmanager)

#find_package(KDE4 REQUIRED)
#find_package(NepomukCore REQUIRED)
#find_package(Soprano REQUIRED)

#include (SopranoAddOntology)
#include (KDE4Defaults)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${CMAKE_SOURCE_DIR}
  ${SOPRANO_INCLUDE_DIR}
  ${NEPOMUK_CORE_INCLUDE_DIR}
  )

set( SRCS
  main.cpp
  mainwindow.cpp
  person.cpp
  personmodel.cpp
  persondelegate.cpp
  persontooltip.cpp
)

kde4_add_executable(nepomukpersonmanager ${SRCS})

target_link_libraries( nepomukpersonmanager
  ${KDE4_KDEUI_LIBS}
  ${KDE4_KIO_LIBS}
  ${NEPOMUK_CORE_LIBRARY}
  ${SOPRANO_LIBRARIES}
  )

install(TARGETS nepomukpersonmanager
        DESTINATION ${BIN_INSTALL_DIR})
