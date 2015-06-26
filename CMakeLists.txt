#           _   ___              _
#  _ _  ___| |_| _ )_ _ _  _ _ _| |_
# | ' \/ -_)  _| _ \ '_| || (_-<|   |
# |_||_\___|\__|___/_|  \_,_/__/|_|_|
# netBrush - Copyright (C) 2015 Ingo Ruhnke <grumbel@gmx.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.0)
project(netbrush VERSION 0.2.4)

set(WARNINGS 0 CACHE BOOL "Switch on extra warnings")

if(WARNINGS)
  set(WARNINGS_CXX_FLAGS
    -pedantic -Wall -Wextra -Wno-c++0x-compat -Wnon-virtual-dtor
    -Werror -Wcast-qual -Winit-self -Wno-unused-parameter
    # -Weffc++ -Wshadow -Wconversion
    )
  # -Winline -Wfloat-equal -Wunreachable-code
endif()

find_package(X11)
find_package(PNG)
find_package(SDL)
find_package(SDL_image)
find_package(SDL_net)

set(NETBRUSH_BINDIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Install location for binaries")
set(NETBRUSH_DATADIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}" CACHE PATH "Install location for datafiles")

file(GLOB NETBRUSH_COMMON_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/command_line.cpp
  src/command_line_generic.cpp)

file(GLOB NETBRUSH_SERVER_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/server.cpp
  src/client_connection.cpp)

file(GLOB NETBRUSH_CLIENT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/alpha_picker.cpp
  src/brush_widget.cpp
  src/brushmask.cpp
  src/client.cpp
  src/client_state.cpp
  src/debug.cpp
  src/drawing_context.cpp
  src/drawing_parameter.cpp
  src/generic_brush.cpp
  src/globals.cpp
  src/grayscale_buffer.cpp
  src/hue_picker.cpp
  src/math/matrix.cpp
  src/math/origin.cpp
  src/math/quaternion.cpp
  src/math/rect.cpp
  src/math/vector.cpp
  src/saturation_value_picker.cpp
  src/screen_buffer.cpp
  src/server_connection.cpp
  src/stroke.cpp
  src/stroke_buffer.cpp
  src/video.cpp
  src/widget/button.cpp
  src/widget/scrollbar.cpp
  src/widget/slider_widget.cpp
  src/widget/widget.cpp
  src/widget/widget_manager.cpp
  src/navigation.cpp
  src/graphic_context_state.cpp
  src/controller.cpp
  src/airbrush_tool.cpp
  src/paintbrush_tool.cpp
  src/scroll_tool.cpp
  src/colorpicker_tool.cpp
  src/region_tool.cpp
  src/color_display.cpp
  src/color.cpp
  src/SDL_tty.c
  src/text_view.cpp
  src/rect_tool.cpp
  src/circle_tool.cpp
  src/line_tool.cpp
  src/surface_graphic_context.cpp
  src/SDL_gfx/SDL_gfxPrimitives.c
  src/input_device_xinput.cpp)

add_compile_options(-std=c++11)
add_definitions(-DDATADIR="${NETBRUSH_DATADIR}")
include_directories(src/)

add_library(netbrush-common STATIC ${NETBRUSH_COMMON_SOURCES})
target_compile_options(netbrush-common PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(netbrush-common SYSTEM PUBLIC ${SDL_INCLUDE_DIR} ${SDL_NET_INCLUDE_DIRS})

add_executable(netbrush-server ${NETBRUSH_SERVER_SOURCES})
target_compile_options(netbrush-server PRIVATE ${WARNINGS_CXX_FLAGS})
target_include_directories(netbrush-server SYSTEM PUBLIC ${SDL_INCLUDE_DIR} ${SDL_NET_INCLUDE_DIRS})
target_link_libraries(netbrush-server netbrush-common ${SDL_LIBRARY} ${SDL_NET_LIBRARIES})

add_executable(netbrush-client ${NETBRUSH_CLIENT_SOURCES})
target_compile_options(netbrush-client PRIVATE ${WARNINGS_CXX_FLAGS})
target_compile_definitions(netbrush-client PUBLIC ${PNG_DEFINITIONS})
target_include_directories(netbrush-client SYSTEM PUBLIC
  ${SDL_INCLUDE_DIR} ${SDL_IMAGE_INCLUDE_DIRS} ${SDL_NET_INCLUDE_DIRS}
  ${X11_INCLUDE_DIR} ${X11_Xinput_INCLUDE_PATH}
  ${PNG_INCLUDE_DIRS})
target_link_libraries(netbrush-client
  netbrush-common
  ${PNG_LIBRARIES}
  ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARIES} ${SDL_NET_LIBRARIES}
  ${X11_LIBRARIES} ${X11_Xinput_LIB})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${NETBRUSH_DATADIR})
install(TARGETS netbrush-client netbrush-server RUNTIME DESTINATION ${NETBRUSH_BINDIR})

# EOF #
