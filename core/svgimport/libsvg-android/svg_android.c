/* libsvg-android - Render SVG documents to an Android canvas
 *
 * Copyright � 2002 University of Southern California
 * Copyright � 2010 Anton Persson
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
 *
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "svg-android-internal.h"

svg_render_engine_t SVG_ANDROID_RENDER_ENGINE = {
	/* hierarchy */
	_svg_android_begin_group,
	_svg_android_begin_element,
	_svg_android_end_element,
	_svg_android_end_group,
	/* path creation */
	_svg_android_move_to,
	_svg_android_line_to,
	_svg_android_curve_to,
	_svg_android_quadratic_curve_to,
	_svg_android_arc_to,
	_svg_android_close_path,
	/* style */
	_svg_android_set_color,
	_svg_android_set_fill_opacity,
	_svg_android_set_fill_paint,
	_svg_android_set_fill_rule,
	_svg_android_set_font_family,
	_svg_android_set_font_size,
	_svg_android_set_font_style,
	_svg_android_set_font_weight,
	_svg_android_set_opacity,
	_svg_android_set_stroke_dash_array,
	_svg_android_set_stroke_dash_offset,
	_svg_android_set_stroke_line_cap,
	_svg_android_set_stroke_line_join,
	_svg_android_set_stroke_miter_limit,
	_svg_android_set_stroke_opacity,
	_svg_android_set_stroke_paint,
	_svg_android_set_stroke_width,
	_svg_android_set_text_anchor,
	/* transform */
	_svg_android_transform,
	_svg_android_apply_view_box,
	_svg_android_set_viewport_dimension,
	/* drawing */
	_svg_android_render_line,
	_svg_android_render_path,
	_svg_android_render_ellipse,
	_svg_android_render_rect,
	_svg_android_render_text,
	_svg_android_render_image
};

svg_android_status_t svgAndroidDestroy(svg_android_t *svg_android) {
	svg_android_status_t status;

	//_svg_android_pop_state (svg_android);

	status = svg_destroy (svg_android->svg);

	free (svg_android);
	return status;
}

svg_android_t *svgAndroidCreate(void) {
	svg_android_t *svg_android;	
	
	svg_android = (svg_android_t *)malloc (sizeof (svg_android_t));

	if (svg_android != NULL) {
		svg_android->canvas = NULL;
		svg_android->state = NULL;
				
		if(svg_create (&(svg_android)->svg)) {
			free(svg_android);
			svg_android = NULL;
		}
	}

	return svg_android;
}

svg_status_t svgAndroidRender
(svg_android_t *svg_android, int android_canvas)
{
	_svg_android_push_state (svg_android, NULL);
	svg_android->state->viewport_width = ANDROID_GET_WIDTH(svg_android);
	svg_android->state->viewport_height = ANDROID_GET_HEIGHT(svg_android);

	svg_android->fit_to_area = 0;
	return svg_render (svg_android->svg, &SVG_ANDROID_RENDER_ENGINE, svg_android);
}

svg_status_t svgAndroidRenderToArea(svg_android_t *svg_android, int android_canvas, int x, int y, int w, int h) {
	_svg_android_push_state (svg_android, NULL);

	svg_android->state->viewport_width = w;
	svg_android->state->viewport_height = h;
	
	svg_android->fit_to_area = -1;
	svg_android->fit_to_x = x;
	svg_android->fit_to_y = y;
	svg_android->fit_to_w = w;
	svg_android->fit_to_h = h;	
	svg_android->fit_to_MATRIX = NULL;
	
	return svg_render (svg_android->svg, &SVG_ANDROID_RENDER_ENGINE, svg_android);
}
