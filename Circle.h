#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "Texture.h"

class Circle
{

    public:

        Circle();
        ~Circle();
        //Move the circle around
        void move();

        //Load the texture from image file

        bool loadTextureFromFile(std::string path);

        //render the texture to a specified position with options otherwise to with default
        void render(double =0.0, SDL_Point* =NULL, SDL_RendererFlip =SDL_FLIP_NONE);

        // Accessor Methods
        SDL_Rect& Box();

        int VelX();

        int VelY();

    	//Utility Methods
    	void SetVelocity(int VelX, int VelY);

        void SwapDirections(Circle& mySecondCircle);

        void SetStartingPosition(int x, int y);

        bool HandleMouseEvent(SDL_Event& e);

    private:
        Texture myTexture;

        //The Collision Box
        SDL_Rect myBox;

        signed int     myVelX;
        signed int     myVelY;
        double  myAngle;

};


#endif // CIRCLE_H_INCLUDED
