#include "Circle.h"
#include <stdio.h>
#include <string>
#include <pthread.h>

SDL_Window* 	 gWindow = NULL; //The window we'll be rendering to
SDL_Renderer* gRenderer = NULL; //The window renderer
TTF_Font *gFont = NULL;
Texture Background, ScoreTextureSpriteSheet, LaunchingDragon;
Texture YouWinTexture, GameOverTexture;
Texture endTexture, Names;
Mix_Music *gMusic = NULL,*gEndMusic = NULL;
Mix_Chunk *ShotSound = NULL;

SDL_Rect ScoreTextureClips[ 10 ];
int gLevelVar = 1;
int nbrClick = 0;
int before=0;
std::string gPATH = "";

bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "FLYING FLINGS",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH,
                                    SCREEN_HEIGHT,
                                    SDL_WINDOW_SHOWN );


        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

void close()
{

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool LoadMenuMedia(int i, Texture& myTexture)
{
    bool returnValue = true;
    switch(i)
    {
    case 1:
        if(!myTexture.loadFromFile("resources/HOMEJ.PNG"))
        {
            returnValue = false;
        }
        break;

    case 2:
        if(!myTexture.loadFromFile("resources/HOMEO.PNG"))
        {
            returnValue = false;
        }
        break;
    case 3:
        if(!myTexture.loadFromFile("resources/HOMEQ.PNG"))
        {
            returnValue = false;
        }
        break;

    default:
        break;
    }
    return returnValue;
}

bool LoadOptions(int i, Texture& myTexture)
{
    bool returnValue = true;

    switch(i)
    {
    case 1:
        if(!myTexture.loadFromFile("resources/HOMEO1.PNG"))
        {
            returnValue = false;
        }
        break;

    case 2:
        if(!myTexture.loadFromFile("resources/HOMEO2.PNG"))
        {
            returnValue = false;
        }
        break;

    case 3:
        if(!myTexture.loadFromFile("resources/HOMEO3.PNG"))
        {
            returnValue = false;
        }
        break;
    }

    return returnValue;
}

bool LoadNiveau(int i, Texture& myTexture)
{
    bool returnValue = true;

    switch(i)
    {
    case 1:
        if(!myTexture.loadFromFile("resources/CF1.PNG"))
        {
            returnValue = false;
        }
        break;

    case 2:
        if(!myTexture.loadFromFile("resources/CF2.PNG"))
        {
            returnValue = false;
        }
        break;
    }

    return returnValue;
}

bool menu()
{
    Texture myTexture;
    SDL_Event key;

    bool menu 	 = true,
          options = false,
          levels  = false;

    bool running = true;
    int i = 1;

    while(running)
    {
        if( menu )
        {
            if( !(running = LoadMenuMedia(i, myTexture)) )
            {
                printf("\nCould not load menu media files.");
            }
        }
        else if ( options )
        {
            if( !(running = LoadOptions(i, myTexture)) )
            {
                printf("\nCould not load Option media files.");
            }
        }
        else if( levels )
        {
            if( !(running = LoadNiveau(i, myTexture)) )
            {
                printf("\nCould not load Niveau media files.");
            }
        }

        if( running )
        {
            //kClear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            myTexture.render(0, 0);

            SDL_RenderPresent( gRenderer );
        }
        else
        {
            break;
        }

        SDL_WaitEvent(&key);

        if( menu )
        {
            if(key.type == SDL_KEYDOWN)
            {
                switch( key.key.keysym.sym )
                {
                case SDLK_UP:
                    if(i == 1)
                        i=3;
                    else
                        i--;
                    break;

                case SDLK_DOWN:
                    if(i == 3)
                        i=1;
                    else
                        i++;
                    break;

                case SDLK_RETURN:

                    switch(i)
                    {
                    case 1:
                        i=0;
                        return true;
                        break;
                    case 2:
                        menu 	= false;
                        options = true;

                        break;
                    case 3:
                        i = 0;
                        return false;
                        break;
                    default:
                        break;
                    }

                    break;

                default:
                    break;
                }
            }
        }
        else if( options )
        {
            if(key.type == SDL_KEYDOWN)
            {
                switch( key.key.keysym.sym )
                {
                    case SDLK_UP:

                        if(i == 1)
                        {
                           i=3;
                        }
                        else
                        {
                            i--;
                        }

                    break;

                    case SDLK_DOWN:

                        if(i == 1)
                        {
                           i=3;
                        }
                        else
                        {
                            i++;
                        }

                    break;

                    case SDLK_RETURN:
                        options = false;
                        levels = true;
                        menu = false;
                        gLevelVar = i;
                        i=1;
                    break;

                default:
                    break;
                }
            }
        }
        else if( levels )
        {
            if(key.type == SDL_KEYDOWN)
            {
                switch( key.key.keysym.sym )
                {
                    case SDLK_UP:
                        if(i == 1)
                            i=2;
                        else
                            i--;
                    break;

                    case SDLK_DOWN:
                        if(i == 2)
                            i=1;
                        else
                            i++;
                    break;

                    case SDLK_RETURN:

                        switch(i)
                        {
                            case 1:
                                gPATH = "resources/box.png";
                                diePATH = "resources/box-cry.png";
                                return true;
                            break;

                            case 2:
                                gPATH = "resources/circle1.png";
                                diePATH = "resources/circle-cry.png";
                                return true;
                            break;

                            default:
                                break;
                        }

                    break;

                default:
                    break;
                }
            }
        }

    }
}

bool HandleTextInput(SDL_Event& e, std::string &inputText)
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
        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
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

bool loadMedia()
{
    bool success = true;

    //play MUsic
    gMusic = Mix_LoadMUS( "resources/beat.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    if( !Background.loadFromFile("resources/myBackground.png"))
    {
        success = false;
    }

    if( !ScoreTextureSpriteSheet.loadFromFile("resources/score.png"))
    {
        success = false;
    }
    else
    {
        for(int i = 0; i < 10; i++)
        {
            ScoreTextureClips[ i ].x = i*90;
            ScoreTextureClips[ i ].y = 0;
            ScoreTextureClips[ i ].w = 90;
            ScoreTextureClips[ i ].h = 90;
        }
    }

    gFont = TTF_OpenFont( "resources/score.ttf", 92);
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Erlibror: %s\n", TTF_GetError() );
        success = false;
    }

    gMusic = Mix_LoadMUS("resources/theme.mp3");
    if( gMusic == NULL)
    {
         printf( "Failed to load load sound! SDL_ttf Erlibror: %s\n", Mix_GetError() );
    }

    gEndMusic = Mix_LoadMUS("resources/end.mp3");
    if( gEndMusic == NULL)
    {
         printf( "Failed to load load sound! SDL_ttf Erlibror: %s\n", Mix_GetError() );
    }

    ShotSound = Mix_LoadWAV("resources/shot.wav");
    if( ShotSound == NULL)
    {
         printf( "Failed to load load sound! SDL_ttf Erlibror: %s\n", Mix_GetError() );
    }
    return success;
}

bool cursorOnOff(Texture& CursorTexture, SDL_Event& e, int& MouseX, int& MouseY)
{
    bool returnValue = false;

    MouseX = e.motion.x;
    MouseY = e.motion.y;

    if(e.type == SDL_MOUSEBUTTONUP)
    {
        CursorTexture.loadFromFile("resources/sniperOnC.png");
        returnValue = true;
    }
    else
    {
        CursorTexture.loadFromFile("resources/sniper.png");
        returnValue = false;
    }

    return returnValue;
}

void EndingScene()
{
   int  y= 600;
   int x = 0;
   SDL_Event e;

    while(y != 0)
    {
         while( SDL_PollEvent( &e ) != 0 )
        {
            if(e.type == SDL_KEYDOWN)
            {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                {
                    y = 1;
                }
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );
        SDL_ShowCursor(SDL_DISABLE);

        endTexture.render(0,0);
        Names.render(x, y);

        y-=1;
        //Update screen
        SDL_RenderPresent( gRenderer );
    }
    SDL_Delay(5000);
}

void play()
{
    Mix_PlayMusic(gMusic, -1);
}

int threadFunction( void* data )
{

    Mix_PlayMusic(gMusic, -1);
    return 0;
}

int main( int argc, char* args[] )
{
    int Timer=0;
    Texture CursorTexture;

    int MouseX, MouseY;

    int iscore = 0;

    bool renderText;

    int OneClicked, TwoClicked, ThreeClicked, FourClicked, FiveClicked, SixClicked, SevenClicked, EightClicked, NineClicked, TenClicked;

        OneClicked  = TwoClicked = ThreeClicked = FourClicked = FiveClicked = SixClicked = SevenClicked = EightClicked = NineClicked = TenClicked=0;

    SDL_Color black = {0, 0, 0};

    Circle myCircleOne, myCircleTwo, myCircleThree, myCircleFour, myCircleFive, myCircleSix, myCircleSeven, myCircleEight, myCircleNine, myCircleTen;


    gPATH = "resources/box.png";
    diePATH = "resources/box-cry.png";

    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
    menu:
                          myCircleOne.SetStartingPosition(0, 0);
    myCircleTwo.SetStartingPosition(920,0);
    myCircleFour.SetStartingPosition(0, 520);
    myCircleThree.SetStartingPosition(300, 0);
    myCircleFive.SetStartingPosition(100,   100);

   /* myCircleSix.SetStartingPosition(100,   100);
    myCircleSeven.SetStartingPosition(180,   180);
    myCircleEight.SetStartingPosition(300,   100);
    myCircleNine.SetStartingPosition(100,  300);
    myCircleTen.SetStartingPosition(100,   80);*/

    //Make them stay still
   myCircleSix.SetVelocity(0, 0);
    myCircleSeven.SetVelocity(0, 0);
    myCircleEight.SetVelocity(0, 0);
    myCircleNine.SetVelocity(0, 0);
    myCircleTen.SetVelocity(0, 0);

    /*The objects to be launched*/
    myCircleSix.SetStartingPosition(0,   520);
    myCircleSeven.SetStartingPosition(0,   520);
    myCircleEight.SetStartingPosition(0,   520);
    myCircleNine.SetStartingPosition(0,   520);
    myCircleTen.SetStartingPosition(0,   520);
        if( !loadMedia() )
        {
            printf("\nCould not load Media");
        }
        else
        {
            SDL_ShowCursor(SDL_ENABLE);
            SDL_SetWindowGrab(gWindow, SDL_TRUE);

            if( !myCircleOne.loadTextureFromFile( gPATH )
                    || !myCircleTwo.loadTextureFromFile( gPATH )
                    || !myCircleThree.loadTextureFromFile( gPATH )
                    || !myCircleFour.loadTextureFromFile( gPATH )
                    || !myCircleFive.loadTextureFromFile( gPATH )
                    || !myCircleSix.loadTextureFromFile( gPATH )
                    || !myCircleSeven.loadTextureFromFile( gPATH )
                    || !myCircleEight.loadTextureFromFile( gPATH )
                    || !myCircleNine.loadTextureFromFile( gPATH )
                    || !myCircleTen.loadTextureFromFile( gPATH )
                    || !CursorTexture.loadFromFile("resources/sniper.png")
                    || !endTexture.loadFromFile("resources/end.png")
                    || !Names.loadFromFile("resources/create.png"))
            {
                printf( "Failed to load the textures!\n" );
            }

            else
            {
                //Main loop flag
                bool quit = false;

                int data = 101;
                SDL_Thread* threadID = SDL_CreateThread( threadFunction, "LazyThread", (void*)data );

                //Event handler
                SDL_Event e;

                if( menu() )
                {
                    quit = false;
                    //Reload the images after user's selection
                    if( !myCircleOne.loadTextureFromFile( gPATH )
                        || !myCircleTwo.loadTextureFromFile( gPATH )
                        || !myCircleThree.loadTextureFromFile( gPATH )
                        || !myCircleFour.loadTextureFromFile( gPATH )
                        || !myCircleFive.loadTextureFromFile( gPATH )
                        || !myCircleSix.loadTextureFromFile( gPATH )
                        || !myCircleSeven.loadTextureFromFile( gPATH )
                        || !myCircleEight.loadTextureFromFile( gPATH )
                        || !myCircleNine.loadTextureFromFile( gPATH )
                        || !myCircleTen.loadTextureFromFile( gPATH )
                        || !LaunchingDragon.loadFromFile("resources/TTL.png")
                        || !CursorTexture.loadFromFile("resources/sniper.png")
                        || !YouWinTexture.loadFromFile("resources/youwin.png")
                        || !GameOverTexture.loadFromFile("resources/gameover.png"))
                    {
                        printf( "Failed to load the textures!\n" );
                    }
                    before   = SDL_GetTicks()/1000;
                    nbrClick = 70/gLevelVar;
                    iscore=0;
                    OneClicked  = TwoClicked = ThreeClicked = FourClicked = FiveClicked = SixClicked = SevenClicked = EightClicked = NineClicked = TenClicked=0;
                    myCircleFive.SetVelocity(2.5*gLevelVar, 2.5*gLevelVar);
                    myCircleFour.SetVelocity(2.5*gLevelVar, 2.5*gLevelVar);
                    myCircleOne.SetVelocity(2.5*gLevelVar, 2.5*gLevelVar);
                    myCircleThree.SetVelocity(2.5*gLevelVar, 2.5*gLevelVar);
                    myCircleTwo.SetVelocity(2.5*gLevelVar, 2.5*gLevelVar);

                    //While application is running
                    while( !quit )
                    {
                        //Handle events on queue
                        while( SDL_PollEvent( &e ) != 0 )
                        {
                            cursorOnOff(CursorTexture, e, MouseX, MouseY);

                            OneClicked      += myCircleOne.HandleMouseEvent(e);
                            TwoClicked      += myCircleTwo.HandleMouseEvent(e);
                            ThreeClicked    += myCircleThree.HandleMouseEvent(e);
                            FourClicked     += myCircleFour.HandleMouseEvent(e);
                            FiveClicked     += myCircleFive.HandleMouseEvent(e);
                            SixClicked     += myCircleSix.HandleMouseEvent(e);
                            SevenClicked     += myCircleSeven.HandleMouseEvent(e);
                            EightClicked     += myCircleEight.HandleMouseEvent(e);
                            NineClicked     += myCircleNine.HandleMouseEvent(e);
                            TenClicked     += myCircleTen.HandleMouseEvent(e);

                            //User requests quit
                            switch( e.type )
                            {
                                case SDL_QUIT:
                                    quit = true;
                                break;

                                case SDL_KEYDOWN:
                                    if( e.key.keysym.sym == SDLK_ESCAPE )
                                    {
                                        quit = true;
                                    }
                                break;

                                case SDL_MOUSEBUTTONUP:
                                    nbrClick--;
                                    Mix_PlayChannel(-1, ShotSound, 0);
                                break;
                                default:
                                    break;
                            }

                        }

                        //Clear screen
                        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        SDL_RenderClear( gRenderer );
                        SDL_ShowCursor(SDL_DISABLE);

                        if( OneClicked == 1 )
                        {
                            OneClicked =2,
                            iscore+=10;
                        }

                        else if( TwoClicked == 1 )
                        {
                            TwoClicked =2;
                            iscore+=10;
                        }

                        else if( ThreeClicked == 1 )
                        {
                            ThreeClicked = 2;
                            iscore+=10;
                        }

                        else if( FourClicked == 1 )
                        {
                            FourClicked = 2;
                            iscore+=10;
                        }

                        else if( FiveClicked == 1 )
                        {
                            FiveClicked = 2;
                            iscore+=10;
                        }
                        else if( SixClicked == 1 )
                        {
                            SixClicked =2;
                            iscore+=10;
                        }

                        else if( SevenClicked == 1 )
                        {
                            SevenClicked = 2;
                            iscore+=10;
                        }

                        else if( EightClicked == 1 )
                        {
                            EightClicked = 2;
                            iscore+=10;
                        }

                        else if( NineClicked == 1 )
                        {
                            NineClicked = 2;
                            iscore+=10;
                        }
                        else if( TenClicked == 1 )
                        {
                            TenClicked = 2;
                            iscore+=10;
                        }

                        if( Timer == 10)
                        {
                            myCircleSix.SetStartingPosition(0,520);
                            myCircleSix.SetVelocity(1*gLevelVar, 2.5*gLevelVar);
                        }
                        else if( Timer == 15)
                        {
                            myCircleSeven.SetStartingPosition(0,520);
                            myCircleSeven.SetVelocity(5*gLevelVar,1*gLevelVar);
                        }
                        else if(Timer == 20)
                        {
                            myCircleEight.SetStartingPosition(0,520);
                            myCircleEight.SetVelocity(4.5*gLevelVar, 2*gLevelVar);
                        }
                        else if(Timer == 25)
                        {
                            myCircleNine.SetStartingPosition(0,520);
                            myCircleNine.SetVelocity(8.5*gLevelVar, 8.5*gLevelVar);
                        }
                        else if(Timer == 30)
                        {
                            myCircleTen.SetStartingPosition(0,520);
                            myCircleTen.SetVelocity(4.5*gLevelVar, 1.5*gLevelVar);
                        }

                        Background.render(0,0);

                        myCircleOne.move();
                        myCircleTwo.move();
                        myCircleThree.move();
                        myCircleFour.move();
                        myCircleFive.move();

                        myCircleSix.move();
                        myCircleSeven.move();
                        myCircleEight.move();
                        myCircleNine.move();
                        myCircleTen.move();

                        myCircleOne.render();
                        myCircleTwo.render();
                        myCircleThree.render();
                        myCircleFour.render();
                        myCircleFive.render();

                        myCircleSix.render();
                        myCircleSeven.render();
                        myCircleEight.render();
                        myCircleNine.render();
                        myCircleTen.render();

                        if(MouseX > SCREEN_WIDTH - 80)
                        {
                            MouseX = SCREEN_WIDTH - 80;
                        }
                        else if(MouseX < 0)
                        {
                            MouseX = 0;
                        }
                        if(MouseY > SCREEN_HEIGHT - 80)
                        {
                            MouseY = SCREEN_HEIGHT - 80;
                        }
                        else if(MouseY < 0)
                        {
                            MouseY = 0;
                        }

                        LaunchingDragon.render(-10, 500);

                        CursorTexture.render( MouseX, MouseY);
                        ScoreTextureSpriteSheet.loadFromRenderedText( "SCORE:" , black);
                        ScoreTextureSpriteSheet.render(0, 0);
                        std::stringstream score;
                        score << iscore;

                        //set score
                        ScoreTextureSpriteSheet.loadFromRenderedText( score.str() , black);
                        ScoreTextureSpriteSheet.render(246, 0);

                        //set timer
                        Timer = (SDL_GetTicks()/1000)-before;
                        std::stringstream getTimer;
                        getTimer << 60-Timer;

                        ScoreTextureSpriteSheet.loadFromRenderedText( getTimer.str() , black);
                        ScoreTextureSpriteSheet.render(940, 0);

                        //set nbr Click
                        std::stringstream Clicks;
                        Clicks << nbrClick;
                        ScoreTextureSpriteSheet.loadFromRenderedText( Clicks.str() , black);
                        ScoreTextureSpriteSheet.render(940, 550);

                        if( getTimer.str() == "0" )
                        {
                            GameOverTexture.render((SCREEN_WIDTH - 827)/2, (SCREEN_WIDTH - 137)/2);
                            quit = true;
                        }

                        if(iscore == 100)
                        {
                            YouWinTexture.render((SCREEN_WIDTH - 653)/2, (SCREEN_WIDTH - 137)/2);
                            quit = true;

                        }

                        if( nbrClick == 0 && iscore < 100)
                        {
                            GameOverTexture.render((SCREEN_WIDTH - 827)/2, (SCREEN_WIDTH - 137)/2);
                            quit = true;
                        }


                        //Update screen
                        SDL_RenderPresent( gRenderer );
                    }
                    SDL_WaitThread( threadID, NULL );

                    SDL_Delay(3000);
                    goto menu;
                }
                else
                {
                    Mix_PlayMusic(gEndMusic, -1);
                    EndingScene();
                }
            }

            //Free resources and close SDL
            close();

        }
    }

    return 0;
}
