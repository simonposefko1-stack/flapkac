#include <raylib.h>

//trubky------------------------------------

class Pipe
{
public:
    int x;
    int medzeraY;
    int pipe_V;
    int pipeH_Y;
    int pipeD_Y;

    bool bod;

    Pipe()
    {
        x = 500;
        medzeraY = 0;
        pipe_V = 4;
        pipeH_Y = 0;
        pipeD_Y = 0;

        bod = false;
    }
};

int medzeraVelkost = 200;

void vytvorTrubky(Pipe& pipe)
{
    pipe.medzeraY = GetRandomValue(250, 750);

    pipe.pipeD_Y = pipe.medzeraY;
    pipe.pipeH_Y = (pipe.medzeraY - 480) - medzeraVelkost;
}

// skore -------------------------------------------------------
    int skore = 0;
//--------------------------------------------------------------

int main() {
    
    //CONFIG//
    double width = 550;
    double height = 900;
    InitWindow(width, height, "Flappy");
    SetTargetFPS(60);
    InitAudioDevice();
    
    Image ikona = LoadImage("./assets/sprites/ikona.png");
    SetWindowIcon(ikona);
    UnloadImage(ikona);

    bool hitboxy = false;   //<------------------- HITBOXY on/off
    bool debug = false;     //<------------------- DEBUG on/off
    bool noclip = false;    //<------------------- NOCLIP on/off 

    //zvuky
    Sound point = LoadSound("./assets/sounds/point.ogg");
    
    Sound wing = LoadSound("./assets/sounds/wing.ogg");
    
    Sound hit = LoadSound("./assets/sounds/hit.ogg");
    
    
    

    //Pozadie
    Texture2D pozadie = LoadTexture("./assets/sprites/pozadie.png");

    //Podlaha
    Texture2D podlaha = LoadTexture("./assets/sprites/podlaha.png");

    //Flappy
    Texture2D flappy = LoadTexture("./assets/sprites/flappy.png");
        //poloha
        double flappyX = 240;
        double flappyY = 450;

        //rychlost
        double flappyV = 0;

        //gravitacia
        double gravitacia = 0.5f;

        //mavnutie
        double mavnutie = -8;

        //rotacia
        double flappyR = 0;

    //tlacidlo
    Texture2D tlacidlo = LoadTexture("./assets/sprites/tlacidlo.png");

    //konec (gameOver)
    Texture2D konec = LoadTexture("./assets/sprites/konec.png");

    //trubky
    Texture2D pipeH = LoadTexture("./assets/sprites/pipeH.png"); //horna
    Texture2D pipeD = LoadTexture("./assets/sprites/pipeD.png"); //dolna


    //----------------------|HRA|----------------------------
    bool gameOver = false;
    bool hraZacala = false;

    Pipe pipe1;
    Pipe pipe2;

    pipe1.x = 700;
    pipe2.x = 1100;

    
    while(WindowShouldClose() == false)
    {
        
        //hitboxy
        if (IsKeyPressed(KEY_H))
        {
            hitboxy = !hitboxy;
        }
        
        //debug
        if (IsKeyPressed(KEY_D))
        {
            debug = !debug;
        }

        //noclip
        if (IsKeyPressed(KEY_N))
        {
            noclip = !noclip;
        }
        
        //restart hry
        if (IsKeyPressed(KEY_R))
        {
            gameOver = false;
            hraZacala = false;

            skore = 0;

            flappyX = 240;
            flappyY = 450;
            flappyV = 0;
            flappyR = 0;

            gravitacia = 0.5;

            pipe1.x = 700;
            pipe2.x = 1100;

            pipe1.bod = false;
            pipe2.bod = false;
        }

        //trubky----------------------
        if(pipe1.x < -200)
            { 
                pipe1.x = 550; 
                vytvorTrubky(pipe1);
                pipe1.bod = false;
            }

        if(pipe2.x < -200)
            { 
                pipe2.x = 550; 
                vytvorTrubky(pipe2);
                pipe2.bod = false;
            }
        
        if (pipe1.x + pipeH.width < flappyX && pipe1.bod == false)
        {
            skore++;
            pipe1.bod = true;
            PlaySound(point);
        }

        if (pipe2.x + pipeH.width < flappyX && pipe2.bod == false)
        {
            skore++;
            pipe2.bod = true;
            PlaySound(point);
        }

        
        //zaciatopk hry-------------------------------
        if(hraZacala == false && IsKeyDown(KEY_SPACE))
        {
            hraZacala = true;

            vytvorTrubky(pipe1);
            vytvorTrubky(pipe2);
        }

        
        if(hraZacala == true && gameOver == false){//gravitacia
            
            //pohyb + gravitacia
            flappyY += flappyV;
            flappyV += gravitacia;

            //mavnutie
            if (IsKeyPressed(KEY_SPACE))
            {
                flappyV = mavnutie;
                PlaySound(wing);
            }

            //rotacia flappyho
                if (flappyV < 0)
                {
                    flappyR -= 1.5;

                    if (flappyR < -20)
                        flappyR = -20;
                }
                else
                {
                    flappyR += 1;

                    if (flappyR > 90)
                        flappyR = 90;
                }

            //pohyb trubiek
            pipe1.x -= pipe1.pipe_V;
            pipe2.x -= pipe2.pipe_V;

            //
        }

        //kolizie-----------------------------------------------
        
            Rectangle flappy_hitbox = {
                (float)flappyX - flappy.width / 2,
                (float)flappyY - flappy.height / 2,
                (float)flappy.width,
                (float)flappy.height
            };

            //pipe1
            Rectangle pipe1H_hitbox = {
                (float)pipe1.x,
                (float)pipe1.pipeH_Y,
                (float)pipeH.width,
                (float)pipeH.height
            };

            Rectangle pipe1D_hitbox = {
                (float)pipe1.x,
                (float)pipe1.pipeD_Y,
                (float)pipeD.width,
                (float)pipeD.height
            };

            //pipe2
            Rectangle pipe2H_hitbox = {
                (float)pipe2.x,
                (float)pipe2.pipeH_Y,
                (float)pipeH.width,
                (float)pipeH.height
            };

            Rectangle pipe2D_hitbox = {
                (float)pipe2.x,
                (float)pipe2.pipeD_Y,
                (float)pipeD.width,
                (float)pipeD.height
            };


            //kontrola kolizie
            if (hraZacala == true && gameOver == false && noclip == false){

            if (CheckCollisionRecs(flappy_hitbox, pipe1H_hitbox) ||
                CheckCollisionRecs(flappy_hitbox, pipe1D_hitbox) ||
                CheckCollisionRecs(flappy_hitbox, pipe2H_hitbox) ||
                CheckCollisionRecs(flappy_hitbox, pipe2D_hitbox))
            {
                gameOver = true;
                PlaySound(hit);
            }
        }
        

         //-------------------------------------------------
        
         //draw-------------------------------------
        
        BeginDrawing();
        ClearBackground(RAYWHITE);

        
        
        //staticke------------------------
        DrawTexture(pozadie, 0, 0, RAYWHITE);
        DrawTexture(podlaha, 0, 800, RAYWHITE);

        //tlacidlo------------------------
        if(hraZacala == false)
        {
            DrawTexture(tlacidlo, 220, 600, RAYWHITE);
        }
        
        //---------------------------------------
        
        //flappy
        DrawTexturePro(
        flappy, //textura
        { 0, 0, (float)flappy.width, (float)flappy.height }, //source
        { (float)flappyX, (float)flappyY, (float)flappy.width, (float)flappy.height}, //destination
        { (float)flappy.width / 2, (float)flappy.height / 2}, //origin
        flappyR, //rotation <------ pouzijem aby sa flappy natacal podla toho ci pada alebo stupa
        RAYWHITE
);

        DrawTexture(pipeH, pipe1.x, pipe1.pipeH_Y, RAYWHITE);
        DrawTexture(pipeD, pipe1.x, pipe1.pipeD_Y, RAYWHITE);

        DrawTexture(pipeH, pipe2.x, pipe2.pipeH_Y, RAYWHITE);
        DrawTexture(pipeD, pipe2.x, pipe2.pipeD_Y, RAYWHITE);

        //skore-------------------------------
        DrawText(TextFormat("%i", skore), width / 2, 50, 50, BLACK);

        if(debug){
        //DrawText(TextFormat("%f", flappyV), 20, 20, 25, BLUE);
        DrawText(TextFormat("flappyV: %.1f", flappyV), 20, 20, 25, BLACK);
        DrawText(TextFormat("flappyY: %.1f", flappyY), 20, 50, 25, BLACK);
        
        DrawText(TextFormat("hraZacala: %s", hraZacala ? "true" : "false"), 20, 80, 25, BLACK);
        DrawText(TextFormat("gameOver: %s", gameOver ? "true" : "false"), 20, 110, 25, BLACK);      }

        if (noclip || debug){

            DrawText(TextFormat("noclip: %s", noclip ? "true" : "false"), 20, 150, 25, RED);

        }

        //gameover------------------------
        if(gameOver == true)
        {
            DrawTexture(konec, 90, 200, RAYWHITE);
        }
        
        if(flappyY > 850 || flappyY <= 0)
            {
            gameOver = true;
            flappyV = 0;
            gravitacia = 0;
            }
        
        if(hitboxy == true){

            DrawRectangleLines(
                    flappy_hitbox.x,
                    flappy_hitbox.y,
                    flappy_hitbox.width,
                    flappy_hitbox.height,
                    RED
                );

                DrawRectangleLines(
                    pipe1H_hitbox.x,
                    pipe1H_hitbox.y,
                    pipe1H_hitbox.width,
                    pipe1H_hitbox.height,
                    RED
                );

                DrawRectangleLines(
                    pipe1D_hitbox.x,
                    pipe1D_hitbox.y,
                    pipe1D_hitbox.width,
                    pipe1D_hitbox.height,
                    RED
                );

                DrawRectangleLines(
                    pipe2H_hitbox.x,
                    pipe2H_hitbox.y,
                    pipe2H_hitbox.width,
                    pipe2H_hitbox.height,
                    RED
                );

                DrawRectangleLines(
                    pipe2D_hitbox.x,
                    pipe2D_hitbox.y,
                    pipe2D_hitbox.width,
                    pipe2D_hitbox.height,
                    RED
                );
        }

        
        EndDrawing();//---------------------------------
    }
    
        

    return 0;
}