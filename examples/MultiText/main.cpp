/*
* Shows how to use the multiline text entity
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("Multiline Text!", Ness::Sizei(800, 600));

	// create a new scene
	Ness::ScenePtr scene = renderer.create_scene();

	// create some texts
	Ness::MultiTextPtr text1 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"this is first line.\nthis is second line.\nlast line!!!", 20);
	text1->set_position(Ness::Point(20, 20));

	Ness::MultiTextPtr text2 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"see this text? over here? right here?\nthis text...\naligned to center!", 24);
	text2->set_position(Ness::Point(400, 160));
	text2->set_alignment(Ness::TEXT_ALIGN_CENTER);
	text2->set_color(Ness::Color::GREEN);

	Ness::MultiTextPtr text3 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"and this...\naligned to the right!", 24);
	text3->set_position(Ness::Point(750, 300));
	text3->set_alignment(Ness::TEXT_ALIGN_RIGHT);
	text3->set_color(Ness::Color::RED);

	Ness::MultiTextPtr text4 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"mirror text?", 24);
	text4->set_position(Ness::Point(50, 350));
	text4->flip_x();
	text4->set_color(Ness::Color(0.0f, 1.0f, 1.0f, 1.0f));

	Ness::MultiTextPtr text5 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"rotated text? cool!", 24);
	text5->set_position(Ness::Point(500, 500));
	text5->set_alignment(Ness::TEXT_ALIGN_CENTER);
	text5->set_rotation(25);
	text5->set_color(Ness::Color(1.0f, 0.0f, 1.0f, 1.0f));

	Ness::MultiTextPtr text6 = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"can also be animated!", 24);
	text6->set_position(Ness::Point(5, 500));
	text6->set_alignment(Ness::TEXT_ALIGN_LEFT);
	text6->set_color(Ness::Color(1.0f, 1.0f, 0.0f, 1.0f));
	Ness::Animators::AnimatorPtr anim = ness_make_ptr<Ness::Animators::AnimatorFaderOut>(text6, true, 0.1f);
	renderer.register_animator(anim);

	Ness::MultiTextPtr fpsshow = scene->create_multitext("../ness-engine/resources/fonts/courier.ttf", 
		"fps", 24);
	fpsshow->set_position(Ness::Point(790, 470));
	fpsshow->set_alignment(Ness::TEXT_ALIGN_RIGHT);
	fpsshow->set_color(Ness::Color(1.0f, 0.5f, 0.25f, 1.0f));

	// create the corner logo
	Ness::SpritePtr corner_logo = scene->create_sprite("../ness-engine/resources/gfx/Ness-Engine-Small.png");
	corner_logo->set_blend_mode(Ness::BLEND_MODE_BLEND);
	corner_logo->set_anchor(Ness::Point::ONE);
	corner_logo->set_opacity(0.5f);
	corner_logo->set_static(true);
	corner_logo->set_position(renderer.get_screen_size());

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	EventsPoller.add_handler(app);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// render the scene
		renderer.start_frame();
		scene->render();
		renderer.end_frame();

		fpsshow->set_text("fps:" + ness_to_string((long long)renderer.fps()));
	}

	// cleanup
	renderer.remove_scene(scene);
	return 0;
}