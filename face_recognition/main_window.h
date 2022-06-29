#pragma once
#include <dlib/gui_widgets.h>

using namespace dlib;

class main_window : public drawable_window
{
public:

    bool IsAdding = false;

    const std::string title = "camera 0";

    typedef image_display::overlay_rect overlay_rect;
    typedef image_display::overlay_line overlay_line;
    typedef image_display::overlay_circle overlay_circle;

    main_window(

    ) :
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

    template < typename image_type >
    main_window(
        const image_type& img
    ) :
        drawable_window(false, false),
        gui_img(*this),
        add_face_button(*this),
        face_name_input(*this)
    {
        set_image(img);

        main_window();
    }

    template < typename image_type >
    main_window(
        const image_type& img,
        const std::string& title
    ) :
        drawable_window(false, false),
        gui_img(*this),
        add_face_button(*this)
    {
        gui_img.disable_overlay_editing();
        set_image(img);

        main_window();
    }

    ~main_window()
    {
        close_window();
    }

    template < typename image_type >
    void set_image(
        const image_type& img
    )
    {
        const unsigned long padding = scrollable_region_style_default().get_border_size();
        auto_mutex M(wm);
        gui_img.set_image(img);

        if (previous_image_size != get_rect(img))
        {
            const rectangle r = gui_img.get_image_display_rect();
            if (image_rect != r)
            {
                set_size(r.width() + padding * 2, r.height() + padding * 2 + bottom_ui_size);

                on_window_resized();

                image_rect = r;
            }
            previous_image_size = get_rect(img);
        }
    }

    void add_overlay(
        const overlay_rect& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    template <typename pixel_type>
    void add_overlay(const rectangle& r, pixel_type p)
    {
        add_overlay(image_display::overlay_rect(r, p));
    }

    void add_overlay(const rectangle& r)
    {
        add_overlay(image_display::overlay_rect(r, rgb_pixel(255, 0, 0)));
    }

    template <typename pixel_type>
    void add_overlay(const rectangle& r, pixel_type p, const std::string& l)
    {
        add_overlay(image_display::overlay_rect(r, p, l));
    }

    template <typename pixel_type>
    void add_overlay(const std::vector<rectangle>& r, pixel_type p)
    {
        std::vector<overlay_rect> temp;
        temp.resize(r.size());
        for (unsigned long i = 0; i < temp.size(); ++i)
            temp[i] = overlay_rect(r[i], p);

        add_overlay(temp);
    }

    void add_overlay(const std::vector<rectangle>& r)
    {
        add_overlay(r, rgb_pixel(255, 0, 0));
    }

    void add_overlay(
        const full_object_detection& object,
        const std::vector<std::string>& part_names
    )
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

    void add_overlay(
        const full_object_detection& object
    )
    {
        std::vector<std::string> part_names;
        add_overlay(object, part_names);
    }

    void add_overlay(
        const std::vector<full_object_detection>& objects,
        const std::vector<std::string>& part_names
    )
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

    void add_overlay(
        const std::vector<full_object_detection>& objects
    )
    {
        std::vector<std::string> part_names;
        add_overlay(objects, part_names);
    }

    void add_overlay(
        const overlay_line& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    template <typename pixel_type>
    void add_overlay(const line& l, pixel_type p)
    {
        add_overlay(image_display::overlay_line(l.p1(), l.p2(), p));
    }

    void add_overlay(const line& l)
    {
        add_overlay(l, rgb_pixel(255, 0, 0));
    }

    void add_overlay(
        const overlay_circle& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    template <typename pixel_type>
    void add_overlay(const point& p1, const point& p2, pixel_type p)
    {
        add_overlay(image_display::overlay_line(p1, p2, p));
    }

    void add_overlay(
        const std::vector<overlay_rect>& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    void add_overlay(
        const std::vector<overlay_line>& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    void add_overlay(
        const std::vector<overlay_circle>& overlay
    )
    {
        gui_img.add_overlay(overlay);
    }

    void clear_overlay(
    )
    {
        gui_img.clear_overlay();
    }

    std::string GetPersonName() {
        return face_name_input.text();
    }

private:
    const int bottom_ui_size = 60;

    void on_window_resized(

    )
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

    image_display gui_img;
    button add_face_button;
    text_field face_name_input;

    rectangle image_rect;
    rectangle previous_image_size;
};