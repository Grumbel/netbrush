/*            _   ___              _
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

#include "globals.hpp"

DrawingContext*   draw_ctx          = 0;
DrawingParameter* client_draw_param = 0;
ScreenBuffer*     screen_buffer     = 0;
StrokeBuffer*     stroke_buffer     = 0;
WidgetManager*    widget_manager    = 0;

std::map<int, ClientState*> client_states;

ServerConnection* server = 0;

Scrollbar* horizontal_scrollbar = 0;
Scrollbar* vertical_scrollbar   = 0;

Navigation* navigation = 0;

Controller* controller = 0;

InputDevice_XInput* xinput = 0;

/* EOF */
