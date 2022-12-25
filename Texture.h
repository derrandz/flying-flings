#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include "header.h"
//Texture wrapper class
class Texture
{
	public:
		//Initializes variables
		Texture(int=1024, int=600);

		//Deallocates memory
		~Texture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Creates image from font string
		// #ifdef _SDL_TTF_H
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		// #endif
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();

		//Gets image dimentions
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* texture;

		//Image dimensions
		int width;
		int height;
};



#endif // TEXTURE_H_INCLUDED
