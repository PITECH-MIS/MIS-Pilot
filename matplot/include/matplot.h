//
// Created by Alan Freitas on 2020-07-03.
//

#ifndef MATPLOTPLUSPLUS_MATPLOT_H
#define MATPLOTPLUSPLUS_MATPLOT_H

// Common / util
#include "util/common.h"
#include "util/concepts.h"
#include "util/geodata.h"
#include "util/handle_types.h"
#include "util/type_traits.h"

// Backends
#include "backend/backend_interface.h"
#include "backend/backend_registry.h"
#include "backend/gnuplot.h"
// #include "backend/opengl_3.h" // Don't include opengl by default

// Figure and axes
#include "core/axes_type.h"
#include "core/axis_type.h"
#include "core/figure_type.h"

// Axes objects
#include "axes_objects/bars.h"
#include "axes_objects/box_chart.h"
#include "axes_objects/circles.h"
#include "axes_objects/contours.h"
#include "axes_objects/error_bar.h"
#include "axes_objects/filled_area.h"
#include "axes_objects/function_line.h"
#include "axes_objects/histogram.h"
#include "axes_objects/labels.h"
#include "axes_objects/line.h"
#include "axes_objects/matrix.h"
#include "axes_objects/network.h"
#include "axes_objects/parallel_lines.h"
#include "axes_objects/stair.h"
#include "axes_objects/string_function.h"
#include "axes_objects/surface.h"
#include "axes_objects/vectors.h"

// Free-standing functions
#include "freestanding/axes_functions.h"
#include "freestanding/axes_lim.h"
#include "freestanding/histcounts.h"
#include "freestanding/plot.h"

#endif // MATPLOTPLUSPLUS_MATPLOT_H
