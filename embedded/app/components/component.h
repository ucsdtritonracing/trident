#pragma once

#include "core/time.h"

namespace App::Components {

/**
 * @brief Base class for all components in the application.
 *
 * @note Components should be constructed with an interface structure that contains all the
 * dependencies they need. This allows for easy testing and swapping of implementations.
 */
class Component {
public:
    // Default destructor is virtual to ensure proper cleanup of derived classes.
    virtual ~Component() = default;

    // Delete the copy constructor and assignment operator to prevent copying a component.
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;

    /**
     * @brief Executes the component's logic at the given timestamp.
     * @param now The current timestamp.
     * @note This is a pure virtual function and must be implemented by derived classes.
     */
    virtual void Step(Core::Timestamp now) = 0;

protected:
    // Deleting the copy constructor removes the implicit default one, so we need to explicitly define a default constructor for derived classes.
    // constexpr so derived components can be constinit.
    constexpr Component() = default;
};

} // namespace App::Components
