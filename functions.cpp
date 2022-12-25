/* Text Input */

//Enable text input
SDL_StartTextInput();

bool textInputHandling(SDL_Event& e, std::string &inputText)
{
	bool renderText = false;

	if( e.type == SDL_KEYDOWN )
	{
		//Handle backspace
		if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
		{
			//lop off character
			inputText.pop_back();
			renderText = true;
		}
		//Handle copy
		else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
		{
			SDL_SetClipboardText( inputText.c_str() );
		}
		//Handle paste
		else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
		{
			inputText = SDL_GetClipboardText();
			renderText = true;
		}
	}
	//Special text input event
	else if( e.type == SDL_TEXTINPUT )
	{
		//Not copy or pasting
		if( !( ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' ) && ( e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) && SDL_GetModState() & KMOD_CTRL ) )
		{
			//Append character
			inputText += e.text.text;
			renderText = true;
		}
	}

	return renderText;
}

//Disable text input
SDL_StopTextInput();

/* Text Input */

/* Mouse Event Handling */

bool Circle::HandleMouseEvent( SDL_Event& e )
{
	//If mouse event happened
	if( e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is on the circle
		bool clicked = true;

		//Mouse is left of the circle
		if( x < myBox.x )
		{
			clicked = false;
		}
		//Mouse is right of the circle
		else if( x > myBox.x + myBox.w )
		{
			clicked = false;
		}
		//Mouse above the circle
		else if( y < myBox.y )
		{
			clicked = false;
		}
		//Mouse below the circle
		else if( y > myBox.y + myBox.h )
		{
			clicked = false;
		}

		//Mouse is outside circle
		if( clicked )
		{
			//Stop the circle form moving
			myVelX = myVelY = 0;
			myTexture.loadFromFile("circle_destroyed.png");
		}
	}

	return clicked;
}

/* Mouse Event Handling */

/*Generating sound upon event*/

//Init()
if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
{
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
}

//Make Global Variables Each Track and sound
gMusicVariable = Mix_LoadWAV( "21_sound_effects_and_music/scratch.wav" );
			 //=Mix_LoadMUS( "21_sound_effects_and_music/beat.wav" );

/*Function to play a sound*/
Mix_PlayMusic( gMusicVariable, -1 );

 //Stop the music
Mix_HaltMusic();