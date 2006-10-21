/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <algorithm>
#include "video.hpp"
#include "globals.hpp"
#include "drawing_parameter.hpp"
#include "brushmask.hpp"
#include "brush_widget.hpp"

BrushWidget::BrushWidget(const Rect& rect_)
  : Widget(rect_)
{
  surface = create_surface(rect_.get_width(), rect_.get_height());
  
}

BrushWidget::~BrushWidget()
{
  SDL_FreeSurface(surface);
}

void
BrushWidget::on_mouse_motion(const MouseMotionEvent& motion)
{
  set_dirty(true);
}

void
BrushWidget::on_mouse_button(const MouseButtonEvent& button)
{
}

void
BrushWidget::draw(SDL_Surface* target)
{
  SDL_Rect pos;
  pos.x = get_rect().left;
  pos.y = get_rect().top;

  SDL_BlitSurface(surface, 0, target, &pos);
}

void 
BrushWidget::set_brush(GrayscaleBuffer* brushmask)
{
  SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
  SDL_LockSurface(surface);
  Uint8* data = static_cast<Uint8*>(surface->pixels);

  int x_of = std::max(0, (surface->w - brushmask->get_width())/2);
  int y_of = std::max(0, (surface->h - brushmask->get_height())/2);
  
  for(int y = 0; y < std::min(brushmask->get_height(), surface->h); ++y)
    for(int x = 0; x < std::min(brushmask->get_width(), surface->w); ++x)
      {
        if (0)
          { // Checkboard fun
            Uint8 check = 64;
            if (((x / 8) % 2) ^ (y / 8) % 2)
              check = 128+64;

            Uint8 c = brushmask->at(x,y);

            data[3*((y+y_of) * surface->w + (x+x_of))+0] = ((255-c) * check + c*client_draw_param->color.r)/255;
            data[3*((y+y_of) * surface->w + (x+x_of))+1] = ((255-c) * check + c*client_draw_param->color.g)/255;
            data[3*((y+y_of) * surface->w + (x+x_of))+2] = ((255-c) * check + c*client_draw_param->color.b)/255;
          }
        else
          {
            Uint8 c = 255 - brushmask->at(x,y);
            data[3*((y+y_of) * surface->w + (x+x_of))+0] = c;
            data[3*((y+y_of) * surface->w + (x+x_of))+1] = c;
            data[3*((y+y_of) * surface->w + (x+x_of))+2] = c;
          }
      }
      
  SDL_UnlockSurface(surface); 
  set_dirty(true);
}

/* EOF */
