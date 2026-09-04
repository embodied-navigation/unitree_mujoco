#include "keyboard_joystick.h"

#include <cassert>

int main()
{
    KeyboardJoystick joystick;

    joystick.setKey(KeyboardKey::kLT, true);
    joystick.setKey(KeyboardKey::kUp, true);
    joystick.update();
    auto remote = joystick.combine();
    assert(remote.RF_RX.btn.components.L2 == 1);
    assert(remote.RF_RX.btn.components.up == 1);

    joystick.setKey(KeyboardKey::kUp, false);
    joystick.setKey(KeyboardKey::kRT, true);
    joystick.setKey(KeyboardKey::kY, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.btn.components.L2 == 1);
    assert(remote.RF_RX.btn.components.up == 0);
    assert(remote.RF_RX.btn.components.R2 == 1);
    assert(remote.RF_RX.btn.components.Y == 1);

    joystick.setKey(KeyboardKey::kY, false);
    joystick.setKey(KeyboardKey::kA, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.btn.components.R2 == 1);
    assert(remote.RF_RX.btn.components.A == 1);
    assert(remote.RF_RX.rx == 0.0F);

    joystick.releaseAll();
    joystick.setKey(KeyboardKey::kRB, true);
    joystick.setKey(KeyboardKey::kX, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.btn.components.R1 == 1);
    assert(remote.RF_RX.btn.components.X == 1);

    joystick.setKey(KeyboardKey::kX, false);
    joystick.setKey(KeyboardKey::kA, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.btn.components.R1 == 1);
    assert(remote.RF_RX.btn.components.A == 1);
    assert(remote.RF_RX.rx == 0.0F);

    joystick.releaseAll();
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.btn.value == 0);
    assert(remote.RF_RX.lx == 0.0F);
    assert(remote.RF_RX.ly == 0.0F);
    assert(remote.RF_RX.rx == 0.0F);
    assert(remote.RF_RX.ry == 0.0F);

    joystick.setKey(KeyboardKey::kQ, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.rx == -1.0F);

    joystick.releaseAll();
    joystick.setKey(KeyboardKey::kE, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.rx == 1.0F);

    joystick.releaseAll();
    joystick.setKey(KeyboardKey::kW, true);
    joystick.setKey(KeyboardKey::kA, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.ly == 0.5F);
    assert(remote.RF_RX.lx == -0.5F);
    assert(remote.RF_RX.btn.components.A == 0);

    joystick.releaseAll();
    joystick.setKey(KeyboardKey::kS, true);
    joystick.setKey(KeyboardKey::kD, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.ly == -0.5F);
    assert(remote.RF_RX.lx == 0.5F);

    joystick.setKey(KeyboardKey::kW, true);
    joystick.setKey(KeyboardKey::kA, true);
    joystick.update();
    remote = joystick.combine();
    assert(remote.RF_RX.ly == 0.0F);
    assert(remote.RF_RX.lx == 0.0F);
}
