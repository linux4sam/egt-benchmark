/*
 * Copyright (C) 2018 Microchip Technology Inc.  All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <egt/ui>
#include <hayai.hpp>
#include <hayai_main.hpp>
#include <vector>

using namespace std;
using namespace egt;
using namespace egt::experimental;

BENCHMARK(ColorFixture, RGBAInterpolation, 10, 100)
{
    ColorMap colors({Palette::red, Palette::green});
    for (auto x = 0; x < 100; x++)
        colors.interp(1.f / x);
}

BENCHMARK(ColorFixture, HSVInterpolation, 10, 100)
{
    ColorMap colors({Palette::red, Palette::green}, ColorMap::interpolation::hsv);
    for (auto x = 0; x < 100; x++)
        colors.interp(1.f / x);
}

BENCHMARK(ColorFixture, HSLInterpolation, 10, 100)
{
    ColorMap colors({Palette::red, Palette::green}, ColorMap::interpolation::hsl);
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
            m_speed.set_x(m_speed.x() * -1);
            m_speed.set_y(m_speed.y() * -1);
            return true;
        }
        return false;
    }

    void change()
    {
        m_widget->set_text(std::to_string(m_c++));
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

BENCHMARK_F(DrawFixture, MoveAnimate, 10, 500)
{
    for (auto& mover : movers)
        mover.animate();

    app.event().draw();
}

BENCHMARK_F(DrawFixture, UpdateButton, 10, 500)
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
        text.set_width(200);
        checkbox.set_width(200);
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

BENCHMARK_F(WidgetFixture, UpdateTextBox, 10, 1000)
{
    text.set_text("Test String");
    app.event().draw();
    text.set_text("Longer test string");
    app.event().draw();
}

BENCHMARK_F(WidgetFixture, UpdateCheckBox, 10, 1000)
{
    checkbox.set_checked(!checkbox.checked());
    app.event().draw();
}

class DrawShapeFixture : public ::hayai::Fixture
{
public:

    virtual void SetUp()
    {
        window.set_boxtype(Theme::boxtype::none);
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
    painter.set_line_width(width);
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
    painter.set_line_width(width);
    painter.set(color);
    painter.draw(line);
    painter.stroke();

    auto rect = line.rect();
    rect += Size(width * 2, width * 2);
    rect -= Point(width, width);
    window.damage(rect);

    app.event().draw();
}

BENCHMARK_F(DrawShapeFixture, DrawSquares, 10, 1000)
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

BENCHMARK_F(DrawShapeFixture, DrawSquaresAlpha, 10, 1000)
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
    BoxSizer sizer(orientation::flex, justification::start);
    window.add(expand(sizer));
    for (auto x = 0; x < 40; x++)
        sizer.add(std::make_shared<Button>("Press Me"));
    window.show();
    app.event().draw();
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