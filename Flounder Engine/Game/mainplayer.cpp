#include "mainplayer.h"

namespace flounder {
	mainplayer::mainplayer()
	{
		m_position = new vector3();
		m_rotation = new vector3();
	}

	mainplayer::~mainplayer()
	{
		delete m_position;
		delete m_rotation;
	}

	void mainplayer::update()
	{
	}
}