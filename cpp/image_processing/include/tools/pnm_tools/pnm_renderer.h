#ifndef H_INCLUDE_TOOLS_PNM_TOOLS_PNM_RENDERER_H_H
#define H_INCLUDE_TOOLS_PNM_TOOLS_PNM_RENDERER_H_H

#include "structures/geometric/point2d.h"
#include "structures/pnm_format/pnm_image_base.h"

namespace Marvin
{
namespace PNM_Format
{

namespace Renderer
{

template <pnm_suitable T>
void draw_point(const Marvin::Geometric::Point2d& point,
    PNMImageTemplated<T>& image, const T& color)
{
    const auto x {static_cast<PNMImageTemplated<T>::size_type>(point.x())};
    const auto y {static_cast<PNMImageTemplated<T>::size_type>(point.y())};
    if ((x >= 0) && (x < image.width()) && (y >= 0) && (y < image.height()))
    {
        image[y, x] = color;
    }
}

} // namespace Renderer

} // namespace PNM_Format
} // namespace Marvin

#endif // H_INCLUDE_TOOLS_PNM_TOOLS_PNM_RENDERER_H_H
