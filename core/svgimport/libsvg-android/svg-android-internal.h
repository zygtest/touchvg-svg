/* libsvg-android - Render SVG documents to an Android canvas
 *
 * Copyright © 2002 University of Southern California
 * Copyright © 2010 Anton Persson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy (COPYING.LESSER) of the
 * GNU Lesser General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * Original Cairo-version:
 * Author: Carl D. Worth <cworth@isi.edu>
 *
 * Android modification:
 * Author: Anton Persson {don d0t juanton 4t gmail d0t com}
 */

#ifndef SVG_ANDROID_INTERNAL_H
#define SVG_ANDROID_INTERNAL_H

#include "svg-android.h"

#include <stdarg.h>

/* XXX: What should this actually be? */
#define SVG_ANDROID_FONT_FAMILY_DEFAULT "verdana"

extern svg_render_engine_t SVG_ANDROID_RENDER_ENGINE;

typedef struct svg_android_pt {
	double x;
	double y;
} svg_android_pt_t;

typedef enum svg_android_render_type {
	SVG_ANDROID_RENDER_TYPE_FILL,
	SVG_ANDROID_RENDER_TYPE_STROKE
} svg_android_render_type_t;

typedef struct svg_android_state {	
	svg_android_t *instance;

	// we need to store the last path point, since we cant get it from an android path...
	// this we need for the arc_to implementation...
	double last_x, last_y;
	
//	cairo_surface_t *child_surface;
	int offscreen_bitmap;
	
	int saved_canvas; // temporary canvas

	int path;
	int paint;
	int matrix;
	
	svg_color_t color;

	/* save state data for "copy_android_state" function */
	svg_fill_rule_t fill_rule;
	svg_length_t width_len;
	svg_stroke_line_cap_t line_cap;
	svg_stroke_line_join_t line_join;
	double miter_limit;
	
	svg_paint_t fill_paint;
	svg_paint_t stroke_paint;
	double fill_opacity;
	double stroke_opacity;

	char *font_family;
	double font_size;
	svg_font_style_t font_style;
	unsigned int font_weight;
	int font_dirty;

	double *dash;
	int num_dashes;
	double dash_offset;

	double opacity;

	double viewport_width;
	double viewport_height;

	int bbox;

	svg_text_anchor_t text_anchor;

	struct svg_android_state *next;
} svg_android_state_t;

struct svg_android {
	svg_t *svg;
	
	svg_android_state_t *state;
	int canvas;
	unsigned int viewport_width;
	unsigned int viewport_height;
		
	unsigned int fit_to_area; // set to !0 for "true", 0 for "false"
	unsigned int fit_to_x, fit_to_y, fit_to_w, fit_to_h;
	double fit_to_scale;
	int fit_to_MATRIX;
};
	
/* svg_android_state.c */

svg_android_status_t
_svg_android_state_create (svg_android_state_t **state, svg_android_t *svg_android);

svg_android_status_t
_svg_android_state_init (svg_android_state_t *state);

svg_android_status_t
_svg_android_state_init_copy (svg_android_state_t *state, const svg_android_state_t *other);

svg_android_status_t
_svg_android_state_deinit (svg_android_state_t *state);

svg_android_status_t
_svg_android_state_destroy (svg_android_state_t *state);

svg_android_state_t *
_svg_android_state_push (svg_android_t *instance, svg_android_state_t *state);

svg_android_state_t *
_svg_android_state_pop (svg_android_state_t *state);

/* svg_android_render.c */
svg_status_t
_svg_android_begin_group (void *closure, double opacity);

svg_status_t
_svg_android_begin_element (void *closure);

svg_status_t
_svg_android_end_element (void *closure);

svg_status_t
_svg_android_end_group (void *closure, double opacity);

svg_status_t
_svg_android_move_to (void *closure, double x, double y);

svg_status_t
_svg_android_line_to (void *closure, double x, double y);

svg_status_t
_svg_android_curve_to (void *closure,
		       double x1, double y1,
		       double x2, double y2,
		       double x3, double y3);

svg_status_t
_svg_android_quadratic_curve_to (void *closure,
				 double x1, double y1,
				 double x2, double y2);

svg_status_t
_svg_android_arc_to (void	       *closure,
		     double	rx,
		     double	ry,
		     double	x_axis_rotation,
		     int		large_arc_flag,
		     int		sweep_flag,
		     double	x,
		     double	y);

void
_svg_path_arc_segment (svg_android_t *svg_android,
		       double   xc,  double yc,
		       double   th0, double th1,
		       double   rx,  double ry,
		       double   x_axis_rotation);

svg_status_t
_svg_android_close_path (void *closure);

svg_status_t
_svg_android_set_color (void *closure, const svg_color_t *color);

svg_status_t
_svg_android_set_fill_opacity (void *closure, double fill_opacity);

svg_status_t
_svg_android_set_fill_paint (void *closure, const svg_paint_t *paint);

svg_status_t
_svg_android_set_fill_rule (void *closure, svg_fill_rule_t fill_rule);

svg_status_t
_svg_android_select_font (svg_android_t *svg_android);

svg_status_t
_svg_android_set_font_family (void *closure, const char *family);

svg_status_t
_svg_android_set_font_size (void *closure, double size);

svg_status_t
_svg_android_set_font_style (void *closure, svg_font_style_t font_style);

svg_status_t
_svg_android_set_font_weight (void *closure, unsigned int weight);

svg_status_t
_svg_android_set_opacity (void *closure, double opacity);

svg_status_t
_svg_android_set_stroke_dash_array (void *closure, double *dash, int num_dashes);

svg_status_t
_svg_android_set_stroke_dash_offset (void *closure, svg_length_t *offset);

svg_status_t
_svg_android_set_stroke_line_cap (void *closure, svg_stroke_line_cap_t line_cap);

svg_status_t
_svg_android_set_stroke_line_join (void *closure, svg_stroke_line_join_t line_join);

svg_status_t
_svg_android_set_stroke_miter_limit (void *closure, double limit);

svg_status_t
_svg_android_set_stroke_opacity (void *closure, double stroke_opacity);

svg_status_t
_svg_android_set_stroke_paint (void *closure, const svg_paint_t *paint);

svg_status_t
_svg_android_set_stroke_width (void *closure, svg_length_t *width);

svg_status_t
_svg_android_set_text_anchor (void *closure, svg_text_anchor_t text_anchor);

svg_status_t
_svg_android_transform (void *closure,
			double a, double b,
			double c, double d,
			double e, double f);

svg_status_t
_svg_android_apply_view_box (void *closure,
			     svg_view_box_t view_box,
			     svg_length_t *width,
			     svg_length_t *height);

svg_status_t
_svg_android_set_viewport_dimension (void *closure,
				     svg_length_t *width,
				     svg_length_t *height);

svg_status_t
_svg_android_render_line (void *closure,
			  svg_length_t *x1_len, svg_length_t *y1_len,
			  svg_length_t *x2_len, svg_length_t *y2_len);

svg_status_t
_svg_android_render_path (void *closure);

svg_status_t
_svg_android_render_ellipse (void *closure,
			     svg_length_t *cx,
			     svg_length_t *cy,
			     svg_length_t *rx,
			     svg_length_t *ry);

svg_status_t
_svg_android_render_rect (void 	     *closure,
			  svg_length_t *x,
			  svg_length_t *y,
			  svg_length_t *width,
			  svg_length_t *height,
			  svg_length_t *rx,
			  svg_length_t *ry);

svg_status_t
_svg_android_render_text (void 	      *closure,
			  svg_length_t  *x_len,
			  svg_length_t  *y_len,
			  const char    *utf8);

svg_status_t
_svg_android_render_image (void		*closure,
			   unsigned char	*data,
			   unsigned int	data_width,
			   unsigned int	data_height,
			   svg_length_t	*x,
			   svg_length_t	*y,
			   svg_length_t	*width,
			   svg_length_t	*height);

svg_status_t
_svg_android_push_state (svg_android_t     *svg_android,
			 int offscreen_bitmap);

svg_status_t
_svg_android_pop_state (svg_android_t *svg_android);

/* svg_android_render_helper.c */
svg_status_t
_svg_android_length_to_pixel (svg_android_t *svg_android, svg_length_t *length, double *pixel);

svg_status_t
_svg_android_set_gradient (svg_android_t *svg_android,
			   svg_gradient_t *gradient,
			   svg_android_render_type_t type);

svg_status_t
_svg_android_set_color_and_alpha (svg_android_t *svg_android,
				  svg_color_t *color,
				  double alpha);

svg_status_t
_svg_android_set_paint_and_opacity (svg_android_t *svg_android, svg_paint_t *paint, double opacity, svg_android_render_type_t type);

svg_status_t
_svg_android_set_pattern (svg_android_t *svg_android,
			  svg_element_t *pattern_element,
			  svg_android_render_type_t type);

svg_status_t
_svg_android_select_font (svg_android_t *svg_android);

void
_svg_android_copy_canvas_state (svg_android_t *svg_android);

#endif
