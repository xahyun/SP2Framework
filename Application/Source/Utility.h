#pragma once
#include "Vertex.h"
#include "Application.h"
#include "Mtx44.h"

Position operator*(const Mtx44& lhs, const Position& rhs);