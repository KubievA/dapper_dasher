#include <raylib.h>

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeigt)
{
    return data.pos.y >= windowHeigt - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame, bool isInAir)
{
    // udpate running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime && !isInAir)
    {
        data.runningTime = 0.0;

        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // Window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 1080;
    windowDimensions[1] = 720;
    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Daper Dasher");
//_______________________________________________________________________

    // nebula Texture
     Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    /*
    // AnimData for nebula
    AnimData nebData{
        {0.0, 0.0, nebula.width/8, nebula.height/8},  // Rectangle rec
        {windowDimensions[0], windowDimensions[1] - nebula.height/8}, // Vector 2 pos
        0, // int frame
        1.0/12.0,  // float updateTime
        0 // float runningTime
    };
    */
    const int sizeOfNebulae{10};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 16.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 300;
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // player variables
    Texture2D Player = LoadTexture("textures/scarfy.png");
    AnimData playerData;
    playerData.rec.width = Player.width/6;
    playerData.rec.height = Player.height;
    playerData.rec.y = 0;
    playerData.rec.x = 0;
    playerData.pos.x = windowDimensions[0]/2 - playerData.rec.width/2;
    playerData.pos.y = windowDimensions[1] - playerData.rec.height;
    playerData.frame = 0; // animation frame
   // amount of time bevor we update the animation frame
    playerData.updateTime = 1.0 / 12.0;
    playerData.runningTime = 0.0;

//_______________________________________________________________________
    // acceleration due to gravity (pixel/s)/s
    const int gravity{1000};
    // nebula X velocity(pixels/second)
    int nebVel{-200};
    // is the rectangle in the air?
    bool isInAir{};
    // jump velocity (pixels/second)
    const int jumpVel{-600};

    int velocity{0};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};

    // for player collision
    bool collision{};
//_______________________________________________________________________


    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        const float dT{GetFrameTime()};

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll background
        bgX -= 20 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        // Scroll midground
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        // Scroll foreground
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 4.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 4.0, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 4.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 4.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 4.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 4.0, WHITE);

    //--------------------------------------------------------------------

        // perform ground check
        if (isOnGround(playerData, windowDimensions[1]))
        {
            // rectangle(Player) is on the grounds
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle(Player) is the air
            velocity += gravity * dT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

    //--------------------------------------------------------------------
        // update nebula position
        for(int i = 0; i < sizeOfNebulae; i++)
        {
            // update the posiion of each nebulas
            nebulae[i].pos.x += nebVel * dT;
        }

        // update Player position
        playerData.pos.y += velocity * dT;

        // update finishline
        finishLine += nebVel * dT;
    //--------------------------------------------------------------------

        // update Player running time
        playerData = updateAnimData(playerData, dT, 5, isInAir);


        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7, 0);
            /*
            // update runningTime
            nebulae[i].runningTime += dT;
            if (nebulae[i].runningTime >= nebulae[i].updateTime)
            {
                nebulae[i].runningTime = 0.0;

                // update animation frames
                nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
                nebulae[i].frame++;
                if (nebulae[i].frame > 7)
                {
                    nebulae[i].frame = 0;
                }

            }
            */
        }

        for (AnimData nebula : nebulae)
        {
            float pad{50};
            // colision rectangle for nebula
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };
            // colision rectangle for player(no need for pad for him)
            Rectangle playerRec{
                playerData.pos.x,
                playerData.pos.y,
                playerData.rec.width,
                playerData.rec.height
            };
            if (CheckCollisionRecs(nebRec, playerRec))
            {
                collision = true;
            }
        }

    //--------------------------------------------------------------------
        // Collison check
        if (collision)
        {
            DrawText("YOU LOSE", windowDimensions[0]/2 - 100, windowDimensions[1]/2, 40, RED);
        }
        else
        {
            if (playerData.pos.x >= finishLine)
            {
                DrawText("YOU WINN", windowDimensions[0]/2 - 100, windowDimensions[1]/2, 40, GREEN);
            }
            else
            {
                for(int i = 0; i < sizeOfNebulae; i++)
                {
                    // draw each nebulas
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                }
            // draw Player
            DrawTextureRec(Player, playerData.rec, playerData.pos, WHITE);
            }
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(nebula);
    UnloadTexture(Player);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}