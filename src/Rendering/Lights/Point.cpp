#include "Point.h"

namespace Rendering::Lights {

Point::Point(glm::vec3 position, Intensity intensity, glm::vec3 color, float constant, float linear, float quadratic)
    : _position(position), _intensity(intensity), _color(color), _constant(constant), _linear(linear), _quadratic(quadratic)
{}

void Point::setMatrices(unsigned int index, p6::Shader const& program) const
{
    auto prefix    = static_cast<std::string>("pointLights[" + std::to_string(index) + "].");
    auto addPrefix = [prefix](const char* property) {
        return prefix + property;
    };

    program.set(addPrefix("position"), _position);
    program.set(addPrefix("ambient"), glm::vec3(_intensity._ambient));
    program.set(addPrefix("diffuse"), glm::vec3(_intensity._diffuse));
    program.set(addPrefix("specular"), glm::vec3(_intensity._specular));
    program.set(addPrefix("color"), glm::vec3(_color));
    program.set(addPrefix("constant"), _constant);
    program.set(addPrefix("linear"), _linear);
    program.set(addPrefix("quadratic"), _quadratic);
}

} // namespace Rendering::Lights