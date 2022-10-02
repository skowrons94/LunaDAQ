cmake_minimum_required(VERSION 3.0)
project(LunaDAQ VERSION 1.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_INSTALL_DIR /opt/LunaDAQ)

set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${UI_DIR})

find_package(CAEN REQUIRED)
find_package(Qt5 COMPONENTS Widgets Xml REQUIRED)
find_package(ROOT REQUIRED COMPONENTS RIO Net)
find_package(Boost COMPONENTS thread system serialization REQUIRED)

include(${ROOT_USE_FILE})

include_directories(
  ${HEADER_DIR}
  ${ROOT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

qt5_wrap_cpp(HEADER
  ${HEADER_DIR}/interface/SlowControlInterface.h
  ${HEADER_DIR}/interface/SlowControlChannel.h
  ${HEADER_DIR}/interface/SlowControlBoard.h
  ${HEADER_DIR}/interface/HistoSpyInterface.h
  ${HEADER_DIR}/interface/WaveSpyInterface.h
  ${HEADER_DIR}/SpyServer.h
  ${HEADER_DIR}/HistoWorker.h
  ${HEADER_DIR}/WaveWorker.h
  ${HEADER_DIR}/XDAQMessenger.h
  ${HEADER_DIR}/QRootCanvas.h
  ${HEADER_DIR}/ListFileDialog.h
  ${HEADER_DIR}/RUReader.h
  ${HEADER_DIR}/RUReaderWorker.h
  ${HEADER_DIR}/LFReader.h
  ${HEADER_DIR}/LFReaderWorker.h
)

qt5_wrap_ui(UI
  ${UI_DIR}/SlowControlInterface.ui
  ${UI_DIR}/HistoSpyInterface.ui
  ${UI_DIR}/WaveSpyInterface.ui
)

add_executable(LunaDAQ
  ${UI}
  ${HEADER}
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/interface/SlowControlInterface.cpp
  ${SRC_DIR}/interface/HistoSpyInterface.cpp
  ${SRC_DIR}/interface/WaveSpyInterface.cpp
  ${SRC_DIR}/interface/SlowControlBoard.cpp
  ${SRC_DIR}/interface/SlowControlChannel.cpp
  ${SRC_DIR}/HistoWorker.cc
  ${SRC_DIR}/WaveWorker.cc
  ${SRC_DIR}/ProjectManager.cpp
  ${SRC_DIR}/SpyServer.cc
  ${SRC_DIR}/XDAQMessenger.cpp
  ${SRC_DIR}/XDAQSpy.cc
  ${SRC_DIR}/RUReader.cc
  ${SRC_DIR}/RUReaderWorker.cc
  ${SRC_DIR}/LFReader.cc
  ${SRC_DIR}/LFReaderWorker.cc
  ${SRC_DIR}/DataFrame.cc
  ${SRC_DIR}/DataFrameBuilderPHA.cc
  ${SRC_DIR}/DataFrameBuilderPSD.cc
  ${SRC_DIR}/CAENBoards.cc
  ${SRC_DIR}/CAENConfig.cc
  ${SRC_DIR}/CAENRegister.cc
  ${SRC_DIR}/CAENRegisterBuilderPHA.cc
  ${SRC_DIR}/CAENRegisterBuilderPSD.cc
  ${SRC_DIR}/InputFormDialog.cpp
  ${SRC_DIR}/ListFileDialog.cc
  ${SRC_DIR}/QRootCanvas.cxx
  ${SRC_DIR}/Utils.cc
)

target_compile_features(LunaDAQ PUBLIC cxx_std_14)

target_link_libraries(LunaDAQ PRIVATE ${ROOT_LIBRARIES} ${Boost_LIBRARIES} ${CAEN_DGTZ_LIBRARIES} Qt5::Widgets Qt5::Xml curl jsoncpp )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/conf
  DESTINATION ${CMAKE_INSTALL_DIR})

install(TARGETS LunaDAQ
  DESTINATION ${CMAKE_INSTALL_DIR})
