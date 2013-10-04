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
 */

#include <stdlib.h>
#include <string.h>

#include "svg-android-internal.h"

svg_android_status_t
_svg_android_state_create (svg_android_state_t **_state, svg_android_t *svg_android)
{
	*_state = malloc (sizeof (svg_android_state_t));
	if (*_state == NULL)
		return SVG_ANDROID_STATUS_NO_MEMORY;

	svg_android_state_t *state = *_state;
	
	state->instance = svg_android;

	/******** set initial values, the missing stuff is filled in by copy and/or init *****/
	
	/* trust libsvg to set all of these to reasonable defaults:
	   state->fill_paint;
	   state->stroke_paint;
	   state->fill_opacity;
	   state->stroke_opacity;
	*/

	state->path = NULL;
	state->paint = NULL;
	state->matrix = NULL;
	state->offscreen_bitmap = NULL;
	state->saved_canvas = NULL;

	state->font_family = NULL;
	state->font_size = 1.0;
	state->font_style = SVG_FONT_STYLE_NORMAL;
	state->font_weight = 400;
	state->font_dirty = 1;

	state->dash = NULL;
	state->num_dashes = 0;
	state->dash_offset = 0;

	state->opacity = 1.0;

	state->bbox = 0;

	state->text_anchor = SVG_TEXT_ANCHOR_START;

	state->next = NULL;
	
	return SVG_ANDROID_STATUS_SUCCESS;
}

svg_android_status_t
_svg_android_state_init (svg_android_state_t *state)
{
	state->path = ANDROID_PATH_CREATE(state->instance);
	state->path =
		(*(state->instance->env))->NewGlobalRef(
			state->instance->env, state->path);

	if(state->paint == NULL) {
		state->paint = ANDROID_PAINT_CREATE(state->instance);
		state->paint =
			(*(state->instance->env))->NewGlobalRef(
				state->instance->env, state->paint);
	}
	
	if(state->matrix == NULL) {
		state->matrix = ANDROID_IDENTITY_MATRIX(state->instance);
		state->matrix =
			(*(state->instance->env))->NewGlobalRef(
				state->instance->env, state->matrix);
	}
	DEBUG_ANDROID("-----------------------------");
	DEBUG_ANDROID1("Created global refs for path at %p", state->path);
	DEBUG_ANDROID1("Created global refs for paint at %p", state->paint);
	DEBUG_ANDROID1("Created global refs for matrix at %p", state->matrix);
		
	// this might already be set by copy
	if(state->font_family == NULL) {
		state->font_family = strdup (SVG_ANDROID_FONT_FAMILY_DEFAULT);
		if (state->font_family == NULL)
			return SVG_ANDROID_STATUS_NO_MEMORY;
	}
	
	return SVG_ANDROID_STATUS_SUCCESS;
}

svg_android_status_t
_svg_android_state_init_copy (svg_android_state_t *state, const svg_android_state_t *other)
{
	if(other == NULL) // Nothing to copy => don't copy
		return SVG_ANDROID_STATUS_SUCCESS;

	// copy all fields as-is
	*state = *other;

	/* We don't need our own child_surface or saved cr at this point. */
	state->offscreen_bitmap = NULL;
	state->saved_canvas = NULL;

	// copy paint
	state->paint = ANDROID_PAINT_COPY(state->instance, other->paint);
	state->paint =
		(*(state->instance->env))->NewGlobalRef(
			state->instance->env, state->paint);

	// copy matrix
	state->matrix = ANDROID_MATRIX_COPY(state->instance, other->matrix);
	state->matrix =
		(*(state->instance->env))->NewGlobalRef(
			state->instance->env, state->matrix);
	
	DEBUG_ANDROID("-----------------------------");
	DEBUG_ANDROID1("COPY created global refs for paint at %p", state->paint);
	DEBUG_ANDROID1("COPY created global refs for paint at %p", state->matrix);

	// We need to duplicate the string
	if (other->font_family)
		state->font_family = strdup ((char *) other->font_family);

	// XXX anton: are these not copied already?!
	state->viewport_width = other->viewport_width;
	state->viewport_height = other->viewport_height;

	// Create a copy of the dash 
	if (other->dash) {
		state->dash = malloc (state->num_dashes * sizeof(double));
		if (state->dash == NULL)
			return SVG_ANDROID_STATUS_NO_MEMORY;
		memcpy (state->dash, other->dash, state->num_dashes * sizeof(double));
	}

	return SVG_ANDROID_STATUS_SUCCESS;
}

svg_android_status_t
_svg_android_state_deinit (svg_android_state_t *state)
{
	DEBUG_ANDROID("-----------------------------");
	DEBUG_ANDROID1("FREEING global refs for path at %p", state->path);
	DEBUG_ANDROID1("FREEING global refs for paint at %p", state->paint);
	DEBUG_ANDROID1("FREEING global refs for matrix at %p", state->matrix);

	(*(state->instance->env))->DeleteGlobalRef(state->instance->env, state->path);
	(*(state->instance->env))->DeleteGlobalRef(state->instance->env, state->paint);
	(*(state->instance->env))->DeleteGlobalRef(state->instance->env, state->matrix);

	if (state->offscreen_bitmap) {
		state->offscreen_bitmap = NULL;
	}

	if (state->saved_canvas) {
		state->saved_canvas = NULL;
	}

	if (state->font_family) {
		free (state->font_family);
		state->font_family = NULL;
	}

	if (state->dash) {
		free (state->dash);
		state->dash = NULL;
	}

	state->next = NULL;

	return SVG_ANDROID_STATUS_SUCCESS;
}

svg_android_status_t
_svg_android_state_destroy (svg_android_state_t *state)
{
	_svg_android_state_deinit (state);

	(*(state->instance->env))->PopLocalFrame(state->instance->env, NULL);
	(*(state->instance->env))->ExceptionClear(state->instance->env);

	free (state);

	state = NULL;

	return SVG_ANDROID_STATUS_SUCCESS;
}

svg_android_state_t *
_svg_android_state_push (svg_android_t *instance, svg_android_state_t *state)
{
	svg_android_state_t *new;

	// create basic state
	_svg_android_state_create (&new, instance);
	if (new == NULL)
		return NULL;

	// if pushing from a previous state, copy relevant data (this will do nothing if state == NULL)
	_svg_android_state_init_copy (new, state);

	// initialize the rest
	_svg_android_state_init (new);

	// point to next
	new->next = state;
	
	DEBUG_ANDROID("-------STATE PUSH!! ----------------------");
	DEBUG_ANDROID1("RETURNING global refs for path at %p", new->path);
	DEBUG_ANDROID1("RETURNING global refs for paint at %p", new->paint);
	DEBUG_ANDROID1("RETURNING global refs for matrix at %p", new->matrix);

	(*(instance->env))->PushLocalFrame(instance->env, 32);

	return new;
}

svg_android_state_t *
_svg_android_state_pop (svg_android_state_t *state)
{
	svg_android_state_t *next;

	if (state == NULL)
		return NULL;

	DEBUG_ANDROID("-------STATE POP!! ----------------------");
	DEBUG_ANDROID1("CLEARING global refs for path at %p", state->path);
	DEBUG_ANDROID1("CLEARING global refs for paint at %p", state->paint);
	DEBUG_ANDROID1("CLEARING global refs for matrix at %p", state->matrix);

	next = state->next;

	_svg_android_state_destroy (state);

	return next;
}
