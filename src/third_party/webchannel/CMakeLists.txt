cmake_minimum_required(VERSION 3.0)
project(webchannel)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

set(LINK_LIBS
    Qt5::Core)

add_definitions("-DQT_NO_JSVALUE")

set(SOURCE_FILES
    qmetaobjectpublisher.cpp
    qmetaobjectpublisher_p.h
    qwebchannel.cpp
    qwebchannel.h
    qwebchannel_p.h
    qwebchannelabstracttransport.cpp
    qwebchannelabstracttransport.h
    qwebchannelglobal.h
    signalhandler_p.h
    variantargument_p.h

    QWebChannel
    QWebChannelAbstractTransport
    private/qobject_p.h

    resources.qrc
    )

add_library(webchannel
            STATIC
            ${SOURCE_FILES})
target_link_libraries(webchannel
                      ${LINK_LIBS})