#include "Circle.h"

std::string diePATH;
Circle::Circle():myTexture(80, 80)
{
    //Initialize the collision box
    myBox.x = 80;
    myBox.y = 80;
    myBox.w = circleWidth;
    myBox.h = circleHeight;

    //Initialize the velocity
    myVelX = 5;
    myVelY = 5;

}

Circle::~Circle()
{
	//Nein
}

void Circle::move()
{
    //Move the dot left or right
    myBox.x += myVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( myBox.x < 0 ) || ( myBox.x + circleWidth > SCREEN_WIDTH ) )
    {
        //Inverse Path
        myVelX = -myVelX;
    }

    //Move the dot up or down
    myBox.y += myVelY;

    //If the dot went too far up or down or touched a wall
    if( ( myBox.y < 0 ) || ( myBox.y + circleHeight > SCREEN_HEIGHT ) )
    {
        //Inverse path
        myVelY = -myVelY;
    }
}

bool Circle::loadTextureFromFile(std::string path)
{
	return myTexture.loadFromFile(path);
}

void Circle::render(double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    int x = myBox.x;
    int y = myBox.y;
	myTexture.render(x, y, NULL, angle, center, flip);
}

int Circle::VelX()
{
	return myVelX;
}

int Circle::VelY()
{
	return myVelY;
}

SDL_Rect& Circle::Box()
{
	return myBox;
}

void Circle::SetVelocity(int x, int y)
{
	myVelX = x;
	myVelY = y;
}

void Circle::SwapDirections(Circle& mySecondCircle)
{
	int VelX, VelY;

	VelX = myVelX;
	VelY = myVelY;

	SetVelocity( mySecondCircle.VelX(), mySecondCircle.VelY() );
	mySecondCircle.SetVelocity(VelX, VelY);


}

void Circle::SetStartingPosition(int x, int y)
{
	myBox.x = x;
	myBox.y = y;
}

bool Circle::HandleMouseEvent( SDL_Event& e )
{
    int clicked = 1;
    //If mouse event happened
    if( e.type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Mouse is left of the circle
        if( x + CURSOR_WIDTH /2< myBox.x )
        {
            clicked = 0;
        }
        //Mouse is right of the circle
        else if( x + CURSOR_WIDTH /2>= myBox.x + myBox.w )
        {
           clicked = 0;
        }
        //Mouse above the circle
        else if( y + CURSOR_WIDTH /2 <= myBox.y )
        {
            clicked = 0;
        }
        //Mouse below the circle
        else if( y + CURSOR_WIDTH /2>= myBox.y + myBox.h )
        {
            clicked = 0;
        }

        //Mouse is outside circle
        if( clicked )
        {
            //Stop the circle form moving
            myVelX = myVelY = 0;
            myTexture.loadFromFile(diePATH);
        }
        return clicked;
    }

    if( e.type == SDL_MOUSEMOTION )
    {
         if( e.type == SDL_MOUSEBUTTONUP )
        {
            //Get mouse position
            int x, y;
            SDL_GetMouseState( &x, &y );

            //Mouse is left of the circle
            if( x < myBox.x )
            {
                clicked = 0;
            }
            //Mouse is right of the circle
            else if( x > myBox.x + myBox.w )
            {
               clicked = 0;
            }
            //Mouse above the circle
            else if( y < myBox.y )
            {
                clicked = 0;
            }
            //Mouse below the circle
            else if( y > myBox.y + myBox.h )
            {
                clicked = 0;
            }

            //Mouse is outside circle
            if( clicked )
            {
                //Stop the circle form moving
                myVelX = myVelY = 0;
                myTexture.loadFromFile("resources/circle3-cry.png");
            }
            return clicked;
        }
    }
    return 0;
}
