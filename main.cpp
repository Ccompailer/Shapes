#include <iostream>
#include <memory>
#include <valarray>
#include <random>
#include "Shapes.h"

constexpr double eps = 1e-6;

std::ostream& operator<<(std::ostream& os, const Point3D& p)
{
    return os << "P{ " << p.x << ", " << p.y << ", " << p.z << " }";
}

std::ostream& operator<<(std::ostream& os, const Vector3D& v)
{
    return os << "V{ " << v.x << ", " << v.y << ", " << v.z << " }";
}

class Circle : public Shape
{
    double rad{};

public:

    Circle() = default;
    Circle(const Point3D& point, double _rad) : Shape(point), rad(_rad) {};
    virtual ~Circle() {};

    Point3D curve_point(double t) const override
    {
        return Point3D {
            rad * std::cos(t) + center.x,
            rad * std::sin(t) + center.y,
            center.z
        };
    }

    double radius() const { return rad; }

    Vector3D curve_vector(double t) const override
    {
        return Vector3D {
            -1 * rad * std::sin(t),
            rad * std::cos(t)
        };
    }
};

class Ellipse : public Shape
{
    double radius_x{};
    double radius_y{};

public:

    Ellipse() = default;
    Ellipse(const Point3D& center, double rad_x, double rad_y)
    : Shape(center), radius_x(rad_x), radius_y(rad_y){};

    virtual ~Ellipse(){}

    Vector3D curve_vector(double t) const override
    {
        return Vector3D {
            -1 * radius_x * sin(t),
            radius_y * cos(t)
        };
    }

    Point3D curve_point(double t) const override
    {
        return Point3D {
            radius_x * cos(t) + center.x,
            radius_y * sin(t) + center.y,
            center.z
        };
    }
};

class Helix : public Shape
{
    double _radius{};
    double _step{};

public:

    Helix() = default;
    Helix(const Point3D& center, double radius, double step)
    : Shape(center), _radius(radius), _step(step){}

    virtual ~Helix(){}

    Point3D curve_point(double t) const override
    {
        return Point3D {
            _radius * t * cos(t) + center.x,
            _radius * t * sin(t) + center.y,
            _radius * t + center.z
        };
    }

    Vector3D curve_vector(double t) const override
    {
        return Vector3D {
            _radius * (std::cos(t) - t * std::sin(t)),
            _radius * (std::sin(t) + t * std::cos(t)),
            _step
        };
    }
};

using PShape = std::shared_ptr<Shape>;

class GenerationShapes
{
    enum shape_t { circle_t , helix_t, ellipse_t };

    static std::random_device rd;

    std::uniform_real_distribution<double> udd{ -1, 1 };
    std::uniform_int_distribution<> usd{ circle_t, helix_t };

public:

    void set_diapason(double min, double max)
    {
        if ((std::abs(max) + std::abs(min)) < 2 * eps)
            return;
        udd = decltype(udd){ min, max };
    }

    PShape circle()
    {
        double radius{};
        while (radius < eps)
            radius = std::abs(udd(rd));

        return std::make_shared<Circle>(Point3D{ udd(rd), udd(rd) }, radius);
    }

    PShape ellipse()
    {
        double radius_x{};
        while (radius_x < eps)
            radius_x = std::abs(udd(rd));

        double radius_y{};
        while (radius_y < eps)
            radius_y = std::abs(udd(rd));

        return std::make_shared<Ellipse>(Point3D
        {
            udd(rd), udd(rd)
        }, radius_x, radius_y);
    }

    PShape helix()
    {
        double radius{};
        while (radius < eps)
            radius = std::abs(udd(rd));

        double step{};
        while (std::abs(step) < eps)
            step = udd(rd);

        return std::make_shared<Helix>(Point3D
        {
            udd(rd), udd(rd), udd(rd)
        }, radius, step);
    }

    PShape random_shape()
    {
        switch (usd(rd))
        {
            case circle_t:
                return circle();
            case ellipse_t:
                return ellipse();
            case helix_t:
                return helix();
            default:
                return circle();
        }
    }

};

std::random_device GenerationShapes::rd;

int main() {
    system("chcp 65001");

    constexpr size_t sz = 10;
    const double pi = std::acos(-1);

    std::vector<PShape> shape_vec;

    {
        GenerationShapes s_gen;
        s_gen.set_diapason(-10, 10);
        for (size_t i{}; i < sz; ++i)
            shape_vec.emplace_back(s_gen.random_shape());
    }

    using PCircle = std::shared_ptr<Circle>;

    std::vector<PCircle> circle_vec;

    double radius_sum{};

    for (auto& ps : shape_vec)
    {
        const std::type_info& ti{ typeid(*ps) };
        std::cout << ti.name() << ' '
                  << ps->curve_point(pi/4) << ' '
                  << ps->curve_vector(pi/4) << std::endl;

        if (ti.hash_code() == typeid(Circle).hash_code())
        {
            circle_vec.emplace_back(std::dynamic_pointer_cast<Circle>(ps));
            radius_sum += circle_vec.back()->radius();
            std::cout << "to another vector -->\n";
        }
        std::cout << std::endl;
    }
    std::sort(circle_vec.begin(), circle_vec.end(),
              [](const auto& lh,
                    const auto& rh)
    {
        return lh->radius() < rh->radius();
    });

    for (auto& pc : circle_vec)
        std::cout << pc->radius() << std::endl;
    std::cout << "sum = " << radius_sum << std::endl;

}
