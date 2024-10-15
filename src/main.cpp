/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <egt/ui>
#include <hayai.hpp>
#include <hayai_main.hpp>
#include <vector>

#include <libinput.h>
#include <libudev.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

using namespace std;
using namespace egt;
using namespace egt::experimental;

BENCHMARK(ColorFixture, RGBAInterpolation, 10, 1000)
{
    ColorMap colors({Palette::red, Palette::green});
    for (auto x = 0; x < 100; x++)
        colors.interp(1.f / x);
}

BENCHMARK(ColorFixture, HSVInterpolation, 10, 1000)
{
    ColorMap colors({Palette::red, Palette::green}, ColorMap::Interpolation::hsv);
    for (auto x = 0; x < 100; x++)
        colors.interp(1.f / x);
}

BENCHMARK(ColorFixture, HSLInterpolation, 10, 1000)
{
    ColorMap colors({Palette::red, Palette::green}, ColorMap::Interpolation::hsl);
    for (auto x = 0; x < 100; x++)
        colors.interp(1.f / x);
}

template<class T>
struct WidgetMover
{
    WidgetMover(T* widget, const Point& speed,
                const Rect& bounding)
        : m_widget(widget),
          m_speed(speed),
          m_bounding(bounding)
    {}

    bool animate()
    {
        m_widget->move(m_widget->point() + m_speed);
        if (!m_bounding.contains(m_widget->box()))
        {
            m_speed.x(m_speed.x() * -1);
            m_speed.y(m_speed.y() * -1);
            return true;
        }
        return false;
    }

    void change()
    {
        m_widget->text(std::to_string(m_c++));
    }

    T* m_widget{nullptr};
    Point m_speed;
    Rect m_bounding;
    int m_c{};
};

class DrawFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        window.show();
        app.event().poll();

        int height = 0;
        int speed = 1;
        const auto NUM_BUTTONS = 15;
        for (auto i = 0; i < NUM_BUTTONS; i++)
        {
            auto button = std::make_shared<Button>("Fast Mover");
            window.add(button);
            auto y = detail::clamp(height,
                                   1,
                                   window.height() - button->height() - 1);
            button->move(Point(window.center().x(), y));
            movers.emplace_back(button.get(), Point(speed, 0), window.box());

            height += 30;
            if (height > window.height() - button->height() - 1)
                height = 0;

            if (++speed > 3)
                speed = 1;
        }
    }

    virtual void TearDown()
    {
        window.remove_all();
        movers.clear();
    }

    Application app;
    TopWindow window;
    vector<WidgetMover<Button>> movers;
};

BENCHMARK_F(DrawFixture, MoveAnimate, 10, 50)
{
    for (auto& mover : movers)
        mover.animate();

    app.event().draw();
}

BENCHMARK_F(DrawFixture, UpdateButton, 10, 50)
{
    for (auto& mover : movers)
        mover.change();

    app.event().draw();
}

class WidgetFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        center(sizer);
        text.width(200);
        checkbox.width(200);
        window.show();
    }

    virtual void TearDown()
    {
    }

    Application app;
    TopWindow window;
    VerticalBoxSizer sizer{window};
    TextBox text{sizer, "off"};
    CheckBox checkbox{sizer, "CheckBox"};
};

BENCHMARK_F(WidgetFixture, UpdateTextBox, 10, 100)
{
    text.text("Test String");
    app.event().draw();
    text.text("Longer test string");
    app.event().draw();
}

BENCHMARK_F(WidgetFixture, UpdateCheckBox, 10, 100)
{
    checkbox.checked(!checkbox.checked());
    app.event().draw();
}

class DrawShapeFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        window.fill_flags().clear();
        window.show();
    }

    virtual void TearDown()
    {
    }

    Application app;
    TopWindow window;

    std::random_device r;
    std::default_random_engine e1{r()};
    std::uniform_int_distribution<int> uniform_dist_width{0, window.width()};
    std::uniform_int_distribution<int> uniform_dist_height{0, window.height()};
    std::uniform_int_distribution<int> uniform_dist_rgb{0, 255};
};

BENCHMARK_F(DrawShapeFixture, DrawLines, 10, 1000)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Line line(Point(uniform_dist_width(e1), uniform_dist_height(e1)),
              Point(uniform_dist_width(e1), uniform_dist_height(e1)));

    auto width = 1;
    painter.line_width(width);
    painter.set(color);
    painter.draw(line);
    painter.stroke();

    auto rect = line.rect();
    rect += Size(width * 2, width * 2);
    rect -= Point(width, width);
    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawLinesAlpha, 10, 1000)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Line line(Point(uniform_dist_width(e1), uniform_dist_height(e1)),
              Point(uniform_dist_width(e1), uniform_dist_height(e1)));

    auto width = 1;
    painter.line_width(width);
    painter.set(color);
    painter.draw(line);
    painter.stroke();

    auto rect = line.rect();
    rect += Size(width * 2, width * 2);
    rect -= Point(width, width);
    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawSquares, 10, 500)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Rect rect(Point(uniform_dist_width(e1), uniform_dist_height(e1)),
              Size(100,100));

    painter.set(color);
    painter.draw(rect);
    painter.fill();

    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawSquaresAlpha, 10, 500)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Rect rect(Point(uniform_dist_width(e1), uniform_dist_height(e1)),
              Size(100,100));

    painter.set(color);
    painter.draw(rect);
    painter.fill();

    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawCircles, 10, 1000)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Circle circle(Point(uniform_dist_width(e1), uniform_dist_height(e1)), 50);

    painter.set(color);
    painter.draw(circle);
    painter.fill();

    auto rect = circle.rect();
    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawCirclesAlpha, 10, 1000)
{
    Painter painter{window.screen()->context()};
    Color color(uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1), uniform_dist_rgb(e1));
    Circle circle(Point(uniform_dist_width(e1), uniform_dist_height(e1)), 50);

    painter.set(color);
    painter.draw(circle);
    painter.fill();

    auto rect = circle.rect();
    window.damage(rect);

    app.event().draw();
}

class DrawAppFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        egt::add_search_path(BENCHMARKDATA);

        egt_logo.autoresize(false);
        egt_logo.clear();
        egt_logo.fill_flags(egt::Theme::FillFlag::blend);
        egt_logo.box(egt::Rect(16, 16, 64, 64));
        egt_logo.background(egt::Image("file:mgs_logo_icon.png"));
        window.add(egt_logo);

        cancel_ok.autoresize(false);
        cancel_ok.show_label(false);
        cancel_ok.fill_flags().clear();
        cancel_ok.border_radius(0);
        cancel_ok.padding(0);
        cancel_ok.box(egt::Rect(16, 128, 64, 64));
        cancel_ok.switch_image(egt::Image("file:cancel.png"), false);
        cancel_ok.switch_image(egt::Image("file:ok.png"), true);
        window.add(cancel_ok);

        pause_play.autoresize(false);
        pause_play.show_label(false);
        pause_play.fill_flags().clear();
        pause_play.border_radius(0);
        pause_play.padding(0);
        pause_play.box(egt::Rect(96, 128, 64, 64));
        pause_play.switch_image(egt::Image("file:pause.png"), false);
        pause_play.switch_image(egt::Image("file:play.png"), true);
        window.add(pause_play);

        up.autoresize(false);
        up.clear();
        up.border_radius(0);
        up.box(egt::Rect(314, 385, 75, 75));
        up.background(egt::Image("file:up.png"), egt::Palette::GroupId::normal);
        up.background(egt::Image("file:up2.png"), egt::Palette::GroupId::active);
        window.add(up);

        down.autoresize(false);
        down.clear();
        down.border_radius(0);
        down.box(egt::Rect(412, 385, 75, 75));
        down.background(egt::Image("file:down.png"), egt::Palette::GroupId::normal);
        down.background(egt::Image("file:down2.png"), egt::Palette::GroupId::active);
        window.add(down);

        window.background(egt::Image("file:background.png"));
        window.show();
    }

    virtual void TearDown()
    {
    }

    Application app;
    TopWindow window;
    egt::Label egt_logo;
    egt::CheckBox cancel_ok;
    egt::CheckBox pause_play;
    egt::Button up;
    egt::Button down;
};

BENCHMARK_F(DrawAppFixture, DrawApp, 10, 200)
{
    window.damage();
    app.event().draw();
}

BENCHMARK_F(DrawAppFixture, DrawButton, 10, 200)
{
    pause_play.checked(!pause_play.checked());
    app.event().draw();
}

BENCHMARK(Startup, Basic, 10, 10)
{
    Application app;
    TopWindow window;
    egt::Button button(window, "Press Me");
    egt::center(button);
    window.show();
    app.event().draw();
}

BENCHMARK(Startup, Complex, 10, 10)
{
    Application app;
    TopWindow window;
    BoxSizer sizer(Orientation::flex, Justification::start);
    window.add(expand(sizer));
    for (auto x = 0; x < 40; x++)
        sizer.add(std::make_shared<Button>("Press Me"));
    window.show();
    app.event().draw();
}

BENCHMARK(Startup, Cairo, 10, 10)
{
	cairo_t* cr;
	cairo_surface_t* surface;

	surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 640, 480);
	cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_destroy(cr);
}

int open_restricted(const char* path, int flags, void* user_data)
{
	int fd = open(path, flags);
	return fd < 0 ? -errno : fd;
}

void close_restricted(int fd, void* user_data)
{
	close(fd);
}

struct libinput_interface interface =
{
	open_restricted,
	close_restricted,
};

BENCHMARK(Startup, Libinput, 10, 10)
{
	struct libinput* li;
	struct udev* udev = udev_new();

	li = libinput_udev_create_context(&interface, NULL, udev);

	libinput_udev_assign_seat(li, "seat0");

        libinput_unref(li);
        udev_unref(udev);
}

class InputFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        auto sizer = std::make_shared<BoxSizer>(Orientation::flex, Justification::middle);
        window.add(expand(sizer));

        for (auto x = 0; x < 42; x++)
            sizer->add(std::make_shared<Button>("Input Test"));
        window.show();
        app.event().draw();

        x = 0;
        y = 0;
    }

    virtual void TearDown()
    {
        window.remove_all();
    }

    Application app;
    TopWindow window;

    struct TestInput : public Input
    {
        using Input::Input;

        inline void dis(Event& event)
        {
            Input::dispatch(event);
        }
    } input;

    int x{window.size().width() / 2};
    int y{window.size().height() / 2};
};

BENCHMARK_F(InputFixture, MouseMove, 20, 25000)
{
    Event down(EventId::raw_pointer_down, Pointer(DisplayPoint(x, y)));
    input.dis(down);

    Event move(EventId::raw_pointer_move, Pointer(DisplayPoint(x, y)));
    input.dis(move);

    Event up(EventId::raw_pointer_up, Pointer(DisplayPoint(x, y)));
    input.dis(up);
}

int main(int argc, char** argv)
{
    hayai::MainRunner runner;

    int result = runner.ParseArgs(argc, argv);
    if (result)
    {
        return result;
    }

    return runner.Run();
}
