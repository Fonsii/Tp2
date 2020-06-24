#pragma once
#include "Controller.h"
#include <memory>

int main() {
	std::unique_ptr<Controller> c(new Controller());
	c->start();
}