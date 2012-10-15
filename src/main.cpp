/*
 * Copyright (c) 2012 Tom Alexander
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */
#include "topaz.h"
#include "model.h"
#include "point.h"
#include "matrix.h"
#include "panda_node.h"
#include "egg_parser.h"
#include <stdio.h>
#include <unordered_map>
#include "lookat_camera.h"
#include "free_view_camera.h"
#include "uberlight.h"
#include "unit.h"
#include "animation.h"
#include "sphere_primitive.h"
#include "rigidbody.h"
#include "terrain.h"

using std::unordered_map;

void game_loop();
bool handle_keypress(const sf::Event & event);
bool handle_resize(const sf::Event & event);
void handle_keyboard(int milliseconds);
void handle_mouse_move();
void print_num_objects(int milliseconds);

topaz::matrix P;
topaz::lookat_camera camera;
topaz::uberlight main_light;
int time_elapsed;
int num_objects = 2;

int main(int argc, char** argv)
{
    topaz::init(argv[0]);
    P = topaz::perspective(60.0f, 800.0f/600.0f, 0.1f, 100.f);
    
    topaz::add_event_handler(&handle_keypress);
    topaz::add_event_handler(&handle_resize);
    topaz::add_pre_draw_function(&handle_keyboard);

float grid[] = {0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0};
    topaz::terrain t(4,4,grid,0.5);
    t.set_scale(4);
    // t.paint(0,0,3,3,topaz::load_texture("green-panda-model.png"));
    t.paint(1,1,2,2,1.0f,0.0f,0.0f);
    t.finalize();

    game_loop(camera, P);
  
    topaz::cleanup();
    return 0;
}

bool handle_keypress(const sf::Event & event)
{
    if (event.type != sf::Event::KeyPressed)
        return false;
    switch (event.key.code)
    {
      case sf::Keyboard::Escape:
        topaz::cleanup();
        return true;
        break;
      default:
        return false;
        break;
    }
}

void handle_keyboard(int time_elapsed)
{
    float seconds = ((float)time_elapsed) / 1000.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        camera.slide(topaz::point(10.0f*seconds, 0.0f, 0.0f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        camera.slide(topaz::point(-10.0f*seconds, 0.0f, 0.0f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        camera.slide(topaz::point(0.0f, 0.0f, 10.0f*seconds));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        camera.slide(topaz::point(0.0f, 0.0f, -10.0f*seconds));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        camera.slide(topaz::point(0.0f, 10.0f*seconds, 0.0f));
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        camera.slide(topaz::point(0.0f, -10.0f*seconds, 0.0f));
    }
}

bool handle_resize(const sf::Event & event)
{
    if (event.type != sf::Event::Resized)
        return false;
    topaz::resize_window(event.size.width, event.size.height);
    topaz::perspective(60.0f, ((float)event.size.width)/((float)event.size.height), 0.1f, 100.f);
    return true;
}

void handle_mouse_move()
{
    sf::Vector2i mouse_position = sf::Mouse::getPosition(*topaz::window);

    sf::Vector2u size = topaz::window->getSize();

    int center_window_x = size.x / 2;
    int center_window_y = size.y / 2;

    int diff_x = mouse_position.x - center_window_x;
    int diff_y = mouse_position.y - center_window_y;

    float rot_yaw = ((float)diff_x) * 0.001;
    float rot_pitch = ((float)diff_y) * 0.001;

    //c2.yaw(rot_yaw);
    //c2.pitch(rot_pitch);

    sf::Vector2i window_center(center_window_x, center_window_y);

    sf::Mouse::setPosition(window_center, *topaz::window);
}

