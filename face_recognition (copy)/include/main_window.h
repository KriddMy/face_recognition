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

    main_window();
    ~main_window();

    template<typename image_type>
    main_window(const image_type& img) :
        drawable_window(false, false),
        gui_img(*this),
        add_face_button(*this),
        face_name_input(*this)
    {
        set_image(img);
        main_window();
    }

    template<typename image_type>
    main_window(const image_type& img, const std::string& title) :
        drawable_window(false, false),
        gui_img(*this),
        add_face_button(*this)
    {
        gui_img.disable_overlay_editing();
        set_image(img);
        main_window();
    }


    template < typename image_type >
    void set_image(const image_type& img)
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

    template <typename pixel_type>
    void add_overlay(const point& p1, const point& p2, pixel_type p)
    {
        add_overlay(image_display::overlay_line(p1, p2, p));
    }

    template <typename pixel_type>
    void add_overlay(const rectangle& r, pixel_type p)
    {
        add_overlay(image_display::overlay_rect(r, p));
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
    
    template <typename pixel_type>
    void add_overlay(const line& l, pixel_type p)
    {
        add_overlay(image_display::overlay_line(l.p1(), l.p2(), p));
    }

    void add_overlay(const line& l); 
    void add_overlay(const rectangle& r);
    void add_overlay(const overlay_rect& overlay);
    void add_overlay(const std::vector<rectangle>& r);
    void add_overlay(const full_object_detection& object, const std::vector<std::string>& part_names);
    void add_overlay(const full_object_detection& object);
    void add_overlay(const std::vector<full_object_detection>& objects, const std::vector<std::string>& part_names);
    void add_overlay(const std::vector<full_object_detection>& objects);
    void add_overlay(const overlay_line& overlay);
    void add_overlay(const overlay_circle& overlay);
    void add_overlay(const std::vector<overlay_rect>& overlay);
    void add_overlay(const std::vector<overlay_line>& overlay);
    void add_overlay(const std::vector<overlay_circle>& overlay);

    void clear_overlay();
    std::string GetPersonName();

private:
    const int bottom_ui_size = 60;
    void on_window_resized();

    image_display gui_img;
    button add_face_button;
    text_field face_name_input;

    rectangle image_rect;
    rectangle previous_image_size;
};