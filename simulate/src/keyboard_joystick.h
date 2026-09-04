#pragma once

#include <array>
#include <atomic>
#include <cstddef>

#include <unitree/dds_wrapper/common/unitree_joystick.hpp>

enum class KeyboardKey : std::size_t
{
    kLT,
    kRT,
    kRB,
    kUp,
    kDown,
    kLeft,
    kRight,
    kX,
    kY,
    kW,
    kS,
    kA,
    kD,
    kQ,
    kE,
    kCount,
};

class KeyboardJoystick : public unitree::common::UnitreeJoystick
{
public:
    KeyboardJoystick()
    {
        LT.smooth = 1.0F;
        RT.smooth = 1.0F;
        lx.smooth = 1.0F;
        ly.smooth = 1.0F;
        rx.smooth = 1.0F;
        ry.smooth = 1.0F;
    }

    void setKey(KeyboardKey key, bool pressed)
    {
        keys_[static_cast<std::size_t>(key)].store(
            pressed, std::memory_order_relaxed);
    }

    void releaseAll()
    {
        for (auto& key : keys_)
            key.store(false, std::memory_order_relaxed);
    }

    void update() override
    {
        LT(pressed(KeyboardKey::kLT) ? 1.0F : 0.0F);
        RT(pressed(KeyboardKey::kRT) ? 1.0F : 0.0F);
        RB(pressed(KeyboardKey::kRB));
        up(pressed(KeyboardKey::kUp));
        down(pressed(KeyboardKey::kDown));
        left(pressed(KeyboardKey::kLeft));
        right(pressed(KeyboardKey::kRight));
        X(pressed(KeyboardKey::kX));
        Y(pressed(KeyboardKey::kY));
        // A is normally the keyboard left-strafe command. While either FSM
        // modifier is held it acts as the gamepad A button instead, enabling
        // both RT+A and RB+A transitions without injecting a yaw command.
        const bool a_is_button =
            (pressed(KeyboardKey::kRT) || pressed(KeyboardKey::kRB)) &&
            pressed(KeyboardKey::kA);
        A(a_is_button);
        constexpr float kLinearCommandSpeed = 0.5F;
        constexpr float kYawCommandSpeed = 1.0F;
        lx(kLinearCommandSpeed *
           (static_cast<float>(pressed(KeyboardKey::kD)) -
            static_cast<float>(pressed(KeyboardKey::kA) && !a_is_button)));
        ly(kLinearCommandSpeed *
           (static_cast<float>(pressed(KeyboardKey::kW)) -
            static_cast<float>(pressed(KeyboardKey::kS))));
        rx(kYawCommandSpeed *
           (static_cast<float>(pressed(KeyboardKey::kE)) -
            static_cast<float>(pressed(KeyboardKey::kQ))));
        ry(0.0F);
    }

private:
    bool pressed(KeyboardKey key) const
    {
        return keys_[static_cast<std::size_t>(key)].load(
            std::memory_order_relaxed);
    }

    std::array<std::atomic_bool,
               static_cast<std::size_t>(KeyboardKey::kCount)> keys_{};
};
