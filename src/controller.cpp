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

#include <iostream>
#include "video.hpp"
#include "alpha_picker.hpp"
#include "hue_picker.hpp"
#include "saturation_value_picker.hpp"
#include "globals.hpp"
#include "drawing_parameter.hpp"
#include "widget/slider_widget.hpp"
#include "brush_widget.hpp"
#include "widget/button.hpp"
#include "widget/widget_manager.hpp"
#include "controller.hpp"

class RadiusCallback : public SliderCallback
{
public:
  void operator()(float v) {
    controller->set_generic_brush_radius(v * 100.0f + 0.1f);
  }
};

class SpikeCallback : public SliderCallback
{
public:
  void operator()(float v) {
    controller->set_generic_brush_spikes(int(v*18) + 2);
  }
};

class HardnessCallback : public SliderCallback
{
public:
  void operator()(float v) {
    controller->set_generic_brush_hardness(v);
  }
};

class AspectRatioCallback : public SliderCallback
{
public:
  void operator()(float v) {
    controller->set_generic_brush_aspect_ratio(v*19.0f + 1.0f);
  }
};

class AngleCallback : public SliderCallback
{
public:
  void operator()(float v) 
  {
    controller->set_generic_brush_angle(v * 180.0f);
  }
};

class ToolButtonCallback : public ButtonCallback
{
private:
  DrawingParameter::Tool tool;
public:
  ToolButtonCallback(DrawingParameter::Tool tool_)
    : tool(tool_)
  {
  }

  void on_press  (Button* button) 
  {
    //std::cout << "Press: " << button << std::endl;
  }

  void on_release(Button* button) 
  {
    //std::cout << "Release: " << button << std::endl;
  }
  
  void on_click  (Button* button) 
  {
    //std::cout << "Setting tool: " << tool << std::endl;
    client_draw_param->tool = tool;
  }
};

Controller::Controller()
{
  // Toolbar
    widget_manager->add(new Button(IMG_Load("data/icons/stock-tool-airbrush-22.png"), 
                                   Rect(Point(2, 2+0*34), Size(34, 34)),
                                   new ToolButtonCallback(DrawingParameter::TOOL_AIRBRUSH)));
    widget_manager->add(new Button(IMG_Load("data/icons/stock-tool-paintbrush-22.png"), 
                                   Rect(Point(2, 2+1*34), Size(34, 34)),
                                   new ToolButtonCallback(DrawingParameter::TOOL_PAINTBRUSH)));
    widget_manager->add(new Button(IMG_Load("data/icons/stock-tool-color-picker-22.png"), 
                                     Rect(Point(2, 2+2*34), Size(34, 34)),
                                     new ToolButtonCallback(DrawingParameter::TOOL_COLOR_PICKER)));

  // Color Selection
  alpha_picker            = new AlphaPicker(Rect(Point(screen->w-128, 128+24), Size(128, 24)));
  saturation_value_picker = new SaturationValuePicker(Rect(Point(screen->w-128, 0), Size(128, 128)));
  hue_picker              = new HuePicker(Rect(Point(screen->w-128, 128), Size(128, 24)));
  color_display           = new ColorDisplay(Rect(Point(0, screen->h - 34), Size(34, 34)));

  widget_manager->add(saturation_value_picker);
  widget_manager->add(hue_picker);
  widget_manager->add(alpha_picker);
  widget_manager->add(color_display);

  // Brush Slider
  radius_slider = new SliderWidget(Rect(Point(screen->w-128, 128+24+24+128+24*(0)), Size(128, 24)),
                                   new RadiusCallback());
  
  spike_slider = new SliderWidget(Rect(Point(screen->w-128, 128+24+24+128+24*(1)), Size(128, 24)),
                                  new SpikeCallback());
  
  hardness_slider = new SliderWidget(Rect(Point(screen->w-128, 128+24+24+128+24*(2)), Size(128, 24)),
                                     new HardnessCallback());
  
  aspect_ratio_slider = new SliderWidget(Rect(Point(screen->w-128, 128+24+24+128+24*(3)), Size(128, 24)),
                                         new AspectRatioCallback());
  
  angle_slider = new SliderWidget(Rect(Point(screen->w-128, 128+24+24+128+24*(4)), Size(128, 24)),
                                  new AngleCallback());
  widget_manager->add(radius_slider);
  widget_manager->add(spike_slider);
  widget_manager->add(hardness_slider);
  widget_manager->add(aspect_ratio_slider);
  widget_manager->add(angle_slider);

}


void
Controller::set_color(const Color& color)
{
  client_draw_param->color = color;
  color_display->set_color(color);
}

void
Controller::set_color_hue(Uint8 hue_)
{
  hue = hue_;
  set_color(Color::from_hue(hue));
  saturation_value_picker->set_color(client_draw_param->color);
  alpha_picker->set_color(client_draw_param->color);
}

void
Controller::set_color_value_saturation(Uint8 value_, Uint8 saturation_)
{
  value = value_;
  saturation = saturation_;

  Color color = Color::from_hue(hue);
  color.apply_value_saturation(value, saturation);
  alpha_picker->set_color(color);
  set_color(color);
}

void
Controller::set_generic_brush_shape(BrushShape shape)
{
  client_draw_param->generic_brush.shape = shape;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush_radius(float radius)
{
  client_draw_param->generic_brush.radius = radius;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush_spikes(int spikes)
{
  client_draw_param->generic_brush.spikes = spikes;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush_hardness(float hardness)
{
  client_draw_param->generic_brush.hardness = hardness;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush_aspect_ratio(float aspect_ratio)
{
  client_draw_param->generic_brush.aspect_ratio = aspect_ratio;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush_angle(float angle)
{
  client_draw_param->generic_brush.angle = angle;
  set_generic_brush(client_draw_param->generic_brush);
}

void
Controller::set_generic_brush(const GenericBrush& brush)
{
  // FIXME: Shouldn't allocate stuff outside of class
  delete client_draw_param->brush_buffer;
  client_draw_param->brush_buffer = brush.generate();
  brush_widget->set_brush(client_draw_param->brush_buffer);

  update_mouse_cursor();
}

void
Controller::update_mouse_cursor()
{
  if (client_draw_param->generic_brush.radius < 5.0f)
    return ;

  GrayscaleBuffer* brush = client_draw_param->brush_buffer;
  
  int w     = brush->get_width();
  int pitch = brush->get_width()/8 + 1;
  int h     = brush->get_height();
  int len   = pitch * h;
  Uint8* data = new Uint8[len];
  Uint8* mask = new Uint8[len];

  memset(data, 0, len);
  memset(mask, 0, len);

  for(int y = 1; y < brush->get_height()-1; ++y)
    for(int x = 1; x < brush->get_width()-1; ++x)
      {
        int threshold = 64;

        Uint8 check = 0;
        if (((x / 1) % 2) ^ (y / 1) % 2)
          check = 1;


        if ((brush->at(x-1, y) < threshold && 
             brush->at(x+1, y) > threshold)
            ||
            (brush->at(x-1, y) > threshold && 
             brush->at(x+1, y) < threshold)
            ||
            (brush->at(x, y-1) < threshold && 
             brush->at(x, y+1) > threshold)
            ||
            (brush->at(x, y-1) > threshold && 
             brush->at(x, y+1) < threshold)
            )
          { // black
            data[y * pitch + x/8] |= (check << (7 - (x%8)));
            mask[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
          }
      }
  
  if (w > 7 && h > 7)
    {
      int y = h / 2;
      int x = 0;
      for(x = w/2 - 3; x <= w/2 + 3; ++x)
        {
          data[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
          mask[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
        }

      x = w / 2;
      for(int y = h/2 - 3; y <= h/2 + 3; ++y)
        {
          data[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
          mask[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
        }


      y = h / 2;
      for(x = w/2 - 1; x <= w/2 + 1; ++x)
        {
          data[y * pitch + x/8] ^= (0x01 << (7 - (x%8)));
          mask[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
        }
      
      x = w / 2;
      for(int y = h/2 - 1; y <= h/2 + 1; ++y)
        {
          data[y * pitch + x/8] ^= (0x01 << (7 - (x%8)));
          mask[y * pitch + x/8] |= (0x01 << (7 - (x%8)));
        }
    }

  SDL_Cursor* cursor = SDL_CreateCursor(data, mask, pitch*8, h, 
                                        w/2, h/2);
  SDL_SetCursor(cursor);

  delete[] mask;
  delete[] data;
}

/* EOF */
