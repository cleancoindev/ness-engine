/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

/**
* A scene node that render shadows. 
* basically will create a white canvas in the size of the screen and will render it with MOD effect. you can add shadow sprites to it
* which are like regular sprites that will be merged on the shadow canvas and make it darker.
* 
* Author: Ronen Ness
* Since: 11/1014
*/

#pragma once
#include "basic_node.h"
#include "../entities/canvas.h"

namespace Ness
{
	/*
	* A Shadow is a type of sprite useable for the shadow node.
	* its like a regular sprite only with default blending mode of MODULATE and with some internal mechanism used by the
	* shadow node. its API is almost the same as the regular sprite API.
	*/
	class Shadow : public Sprite
	{
	private:
		bool m_need_redraw;

	public:
		// create the shadow object
		NESSENGINE_API Shadow(Renderer* renderer, const String& TextureFile, const Color& color = Ness::Color::BLACK) 
			: Sprite(renderer, TextureFile) 
		{
			set_color(color);
			set_blend_mode(BLEND_MODE_MOD);
			m_need_redraw = false;
		}

		// set need transformations update + redraw
		NESSENGINE_API virtual void transformations_update()
		{
			m_need_transformations_update = true;
			m_need_redraw = true;
		}

		// return if this shadow needs redraw
		NESSENGINE_API inline bool need_redraw() const {return m_need_redraw;}

		// set no longer need redraw
		NESSENGINE_API inline void set_need_redraw(bool need) {m_need_redraw = need;}

		// need to update transformations when changing visible so that the shadow node will re-render
		NESSENGINE_API inline void set_visible(bool Visible) 
		{
			if (m_visible == Visible)
				return;
			m_need_transformations_update = true;
			m_need_redraw = true;
			m_visible = Visible;
		}
	};

	NESSENGINE_API typedef SharedPtr<Shadow> ShadowPtr;

	// a special node that creates shadow effects.
	// how this works:
	// this node creates a canvas the size of the screen, and fill it with the ambient shadow color (default is white, meaning no shadow color. use set_ambient_shadow() to change)).
	// every 'shadow' you add to this node is a sprite rendered with modulate effect over the canvas, meaning it makes the canvas darker.
	// when this node is rendered, the canvas is rendered all over the screen with mod effect, so the shadows make the screen darker.
	// note: the shadow node has optimization that the canvas is re-rendered only when one of the shadows or the shadownode itself changes.
	// note2: the shadow node acts like a regular renderable node, with rendering order and even z-value. so make sure to add it last to affect all objects that should be below it.
	class ShadowNode : public BaseNode
	{
	private:
		CanvasPtr m_canvas;			// the shadows canvas
		bool m_need_update;			// true when need to re-render the shadow canvas
		bool m_always_update;		// if true, will always re-render the entire shadow canvas
		Point m_last_camera_pos;	// last camera position

	public:
		// create the znode
		NESSENGINE_API ShadowNode(Renderer* renderer);

		// set / get ambient color (ambient == the shadow color that will be used on empty areas with no shadows)
		NESSENGINE_API inline void set_ambient_shadow(const Color& color) {m_canvas->set_clean_color(color); m_need_update = true;}
		NESSENGINE_API inline const Color& get_ambient_shadow() const {return m_canvas->get_clean_color();}

		// when the shadownode updates, we need to re-render
		NESSENGINE_API virtual void transformations_update();

		// create a shadow
		NESSENGINE_API ShadowPtr create_shadow(const String& shadowTexture, const Color& color = Color::BLACK);

		// return all shadows currently in screen
		NESSENGINE_API void get_shadows_in_screen(Containers::Vector<ShadowPtr>& out_list, const CameraPtr& camera = NullCamera) const;

		// render the shadow node
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// by default, the shadow node only refreash when one of the shadows changes.
		// if you want from any reason to disable this optimization and always recalculate the shadows, 
		// set this to true. this is not recommended, for the optimization should not affect you in any way and is very useful.
		NESSENGINE_API inline void set_always_refresh(bool enabled) {m_always_update = enabled;}

		// add/remove object
		NESSENGINE_API void add(const RenderablePtr& object);
		NESSENGINE_API void remove(const RenderablePtr& object);

		// return the shadow node canvas
		NESSENGINE_API const CanvasPtr& get_target_canvas() const {return m_canvas;}
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<ShadowNode> ShadowNodePtr;
};