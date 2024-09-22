#include "main_window.h"

main_window::main_window() :
    drawable_window(false, false),
    gui_img(*this),
    add_face_button(*this),
    face_name_input(*this)
{
    gui_img.disable_overlay_editing();

    add_face_button.set_name("Add");
    add_face_button.set_click_handler([this]() {

        if (IsAdding)
        {
            IsAdding = false;
            add_face_button.set_name("Start");
            on_window_resized();
        }
        else
        {
            IsAdding = true;
            add_face_button.set_name("Stop");
            on_window_resized();
        }

        });


    set_title(title);
    set_size(430, 380);
    on_window_resized();
    show();
}

main_window::~main_window()
{
    close_window();
}

void main_window::add_overlay(const overlay_rect& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::add_overlay(const rectangle& r)
{
    add_overlay(image_display::overlay_rect(r, rgb_pixel(255, 0, 0)));
}

void main_window::add_overlay(const std::vector<rectangle>& r)
{
    add_overlay(r, rgb_pixel(255, 0, 0));
}

void main_window::add_overlay(const full_object_detection& object, const std::vector<std::string>& part_names)
{

    add_overlay(overlay_rect(object.get_rect(), rgb_pixel(255, 0, 0)));

    std::vector<overlay_circle> temp;
    temp.reserve(object.num_parts());
    for (unsigned long i = 0; i < object.num_parts(); ++i)
    {
        if (object.part(i) != OBJECT_PART_NOT_PRESENT)
        {
            if (i < part_names.size())
                temp.push_back(overlay_circle(object.part(i), 7, rgb_pixel(0, 255, 0), part_names[i]));
            else
                temp.push_back(overlay_circle(object.part(i), 7, rgb_pixel(0, 255, 0)));
        }
    }

    add_overlay(temp);
}

void main_window::add_overlay(const full_object_detection& object)
{
    std::vector<std::string> part_names;
    add_overlay(object, part_names);
}

void main_window::add_overlay(const std::vector<full_object_detection>& objects, const std::vector<std::string>& part_names)
{
    std::vector<overlay_rect> rtemp;
    rtemp.reserve(objects.size());
    for (unsigned long i = 0; i < objects.size(); ++i)
    {
        rtemp.push_back(overlay_rect(objects[i].get_rect(), rgb_pixel(255, 0, 0), part_names[i]));
    }

    add_overlay(rtemp);

    std::vector<overlay_circle> temp;
    for (unsigned long i = 0; i < objects.size(); ++i)
    {
        for (unsigned long j = 0; j < objects[i].num_parts(); ++j)
        {
            if (objects[i].part(j) != OBJECT_PART_NOT_PRESENT)
            {
                if (j < part_names.size())
                    temp.push_back(overlay_circle(objects[i].part(j), 7, rgb_pixel(0, 255, 0)));
                else
                    temp.push_back(overlay_circle(objects[i].part(j), 7, rgb_pixel(0, 255, 0)));
            }
        }
    }

    add_overlay(temp);
}

void main_window::add_overlay(const std::vector<full_object_detection>& objects)
{
    std::vector<std::string> part_names;
    add_overlay(objects, part_names);
}

void main_window::add_overlay(const overlay_line& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::add_overlay(const line& l)
{
    add_overlay(l, rgb_pixel(255, 0, 0));
}

void main_window::add_overlay(const overlay_circle& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::add_overlay(const std::vector<overlay_rect>& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::add_overlay(const std::vector<overlay_line>& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::add_overlay(const std::vector<overlay_circle>& overlay)
{
    gui_img.add_overlay(overlay);
}

void main_window::clear_overlay()
{
    gui_img.clear_overlay();
}

std::string main_window::GetPersonName()
{
    return face_name_input.text();
}

void main_window::on_window_resized()
{
    drawable_window::on_window_resized();
    unsigned long width, height;
    get_size(width, height);
    gui_img.set_size(width, height - bottom_ui_size);

    add_face_button.set_pos(10, height - bottom_ui_size + 10);
    add_face_button.set_size(50, bottom_ui_size - 20);

    face_name_input.set_pos(add_face_button.get_rect().right() + 10,
        add_face_button.get_rect().top());
    face_name_input.set_width(100);
}