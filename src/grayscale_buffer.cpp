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

#include <assert.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include "math/rect.hpp"
#include "video.hpp"
#include "grayscale_buffer.hpp"

GrayscaleBuffer::GrayscaleBuffer(SDL_Surface* surface)
  : width(surface->w), height(surface->h)
{
  buffer = new unsigned char[width*height];

  SDL_LockSurface(surface);
  int bpp = surface->format->BytesPerPixel;

  Uint8* data = static_cast<Uint8*>(surface->pixels);
  Uint8 r, g, b, a;
  for(int i = 0; i < width*height; ++i)
    {
      //if (i % width == 0)
      //std::cout << std::endl;

      SDL_GetRGBA(*reinterpret_cast<Uint32*>(data + (i*bpp)), surface->format, &r, &g, &b, &a);

      if (0)
        std::cout << "(" << std::setw(3) << int(r) << " "
                << std::setw(3) << int(g) << " "
                << std::setw(3) << int(b) << " "
                << std::setw(3) << int(a) << ")  ";

      buffer[i] = (((255 - (r + g + b))/3 * a)/255);
    }
  //std::cout << std::endl;
  SDL_UnlockSurface(surface);
}

GrayscaleBuffer::GrayscaleBuffer(int w, int h, Uint8 c)
  : width(w), height(h)
{
  buffer = new unsigned char[w*h];
  memset(buffer, c, w*h);
}

GrayscaleBuffer::GrayscaleBuffer(int w, int h)
  : width(w), height(h)
{
  buffer = new unsigned char[w*h];
}

GrayscaleBuffer::~GrayscaleBuffer()
{
}

void
GrayscaleBuffer::clear(Uint8 c)
{
  memset(buffer, c, width*height);
}

void
GrayscaleBuffer::fill_rect(const Rect& rect_, Uint8 c)
{
  Rect rect = rect_;
  clip_to(rect, Rect(0, 0, width, height));

  // FIXME: Ugly way to clip
  if (rect.left < rect.right && rect.top < rect.bottom)
    for(int y = rect.top; y < rect.bottom; ++y)
      {
        memset(buffer + y * width + rect.left, c, rect.get_width());
      }
}

void
GrayscaleBuffer::blit(GrayscaleBuffer* source, SDL_Rect* srcrect, int of_x, int of_y)
{
  assert(!"Implement me");
}

void
GrayscaleBuffer::blit(GrayscaleBuffer* source, int of_x, int of_y, Uint8 alpha, BlendMode mode)
{
  //std::cout << "alpha: " << mode << " " << int(alpha) << std::endl;
  int start_x = std::max(of_x, 0);
  int start_y = std::max(of_y, 0);

  int end_x = std::min(of_x + source->width,  width);
  int end_y = std::min(of_y + source->height, height);

  // insert blitter here

  switch(mode)
    {
    case OPAQUE:
      for(int y = start_y; y < end_y ; ++y)
        for(int x = start_x; x < end_x ; ++x)
          {
            Uint8& f = buffer[y * width + x];
            Uint8& p = source->buffer[(y - of_y) * source->width + (x - of_x)];

            f = p;
          }
      break;

    case ADDITION:
      for(int y = start_y; y < end_y ; ++y)
        for(int x = start_x; x < end_x ; ++x)
          {
            Uint8& f = buffer[y * width + x];
            Uint8& p = source->buffer[(y - of_y) * source->width + (x - of_x)];

            f = std::max(std::min(f + p, 255), 0);
          }
      break;

    case SUBTRACT:
      for(int y = start_y; y < end_y ; ++y)
        for(int x = start_x; x < end_x ; ++x)
          {
            Uint8& f = buffer[y * width + x];
            Uint8& p = source->buffer[(y - of_y) * source->width + (x - of_x)];

            f = std::max(f - p, 0);
          }
      break;

    case ALPHA:
      for(int y = start_y; y < end_y ; ++y)
        for(int x = start_x; x < end_x ; ++x)
          {
            Uint8& f = buffer[y * width + x];
            Uint8& p = source->buffer[(y - of_y) * source->width + (x - of_x)];

            // Works, but not exactly alpha
            f = std::max(std::min(f + (alpha * p/255)/4, 255), 0);
            //f = alpha;

            //f = (((255 - p) * f) + (p * p))/255;

            // Speed worm: f = ((p)*p + p*f)/255;
          }
      break;

    default:
      assert(!"Unknown blend mode");
    }
}

Uint8
GrayscaleBuffer::interpolated_at(float x, float y) const
{
  // FIXME: implement some clever interpolation here
  if (x < 0)
    x = 0;
  else if (x > width-1)
    x = width - 1;

  if (y < 0)
    y = 0;
  else if (y > height-1)
    y = height - 1;

  return at(int(x), int(y));
}

GrayscaleBuffer*
GrayscaleBuffer::scale(float sx, float sy)
{
  // FIXME: Optimize me
  GrayscaleBuffer* gray = new GrayscaleBuffer(int(width * sx), int(height * sy));

  for(int y = 0; y < gray->height; ++y)
    for(int x = 0; x < gray->width; ++x)
      {
        gray->buffer[gray->height * y + x] = interpolated_at(x / sx, y / sy);
      }

  return gray;
}

#ifdef __TEST__
#include <stdio.h>

void print(GrayscaleBuffer& buffer)
{
  for(int y = 0; y < buffer.get_height(); ++y)
    {
      for(int x = 0; x < buffer.get_width(); ++x)
      {
        printf("%3d ", int(buffer.at(x,y)));
      }
      putchar('\n');
    }
  putchar('\n');
}

int main()
{
  GrayscaleBuffer target(20,20, 0);
  GrayscaleBuffer source(5,5, 99);

  print(target);
  print(source);

  target.blit(&source, -3, -3, GrayscaleBuffer::ADDITION);
  target.blit(&source, 5, 4, GrayscaleBuffer::ADDITION);
  target.blit(&source, 7, 5, GrayscaleBuffer::SUBTRACT);

  print(target);

}

#endif

/* EOF */
