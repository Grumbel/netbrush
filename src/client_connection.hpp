/*  $Id$
**            _   ___              _
**   _ _  ___| |_| _ )_ _ _  _ _ _| |_
**  | ' \/ -_)  _| _ \ '_| || (_-<|   |
**  |_||_\___|\__|___/_|  \_,_/__/|_|_|
**  netBrush - Copyright (C) 2006 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HEADER_CLIENT_CONNECTION_HPP
#define HEADER_CLIENT_CONNECTION_HPP

#include <string>
#include "SDL_net.h"

class ClientConnection
{
public:
  int id;
  TCPsocket   tcpsock;
  int         buffer_pos;
  std::string buffer;
  bool        invalid;
  bool        full_client;

public:
  ClientConnection(int id_, TCPsocket socket);

  bool is_invalid();
  void update();
  void send_string(const std::string& line);
  void process_line(const std::string& line);
};

#endif

/* EOF */
