#include "AppPch.h"

//=============================================================================
void Initialize ()
{

    const uint WIDTH = 128;
    const uint HEIGHT = 128;
    const char FILENAME[] = "noise.ppm";

    FILE * pFile;
    int rv = fopen_s(&pFile, FILENAME, "w");
    if (rv != 0) {
        printf("Bad file: %s", FILENAME);
        return;
    }

    fprintf(pFile, "P3\n%d %d\n%d\n", WIDTH, HEIGHT, 255);

    for (uint y = 0; y < HEIGHT; ++y)
    {
        for (uint x = 0; x < WIDTH; ++x)
        {
            const Vector2 pos = Vector2(float32(x), float32(y));
            const float32 temp = Math::Noise::Perlin::Get(pos);
            const float32 rain = Math::Noise::Perlin::Get(pos + Vector2(150.0f, 567.0f));


            Color color;
            if (temp > 0.75f && rain <= 0.25f)
                color = Color::Yellow;
            else if (Math::IsInRange(temp, 0.25f, 0.75f) && rain > 0.5f)
                color = Color::Green;
            else if (temp <= 0.5f && rain <= 0.5f)
                color = Color::Red;
            else if (temp <= 0.4f && rain > 0.5f)
                color = Color::White;
            else
                color = Color(0.5f, 0.25f, 0.0f);



            fprintf(pFile, "%d %d %d ",
                FloatToUint(temp * 255.0f),
                FloatToUint(temp * 255.0f),
                FloatToUint(temp * 255.0f)
            );
        }
        fprintf(pFile, "\n");
    }

    fclose(pFile);
}

//=============================================================================
void Uninitialize ()
{

}

//=============================================================================
int PLATFORM_MAIN ()
{
    DebugBreakOnAllocation(71);

    Initialize();

    CApplication app;
    app.Go();

    Uninitialize();

    return 0;
}