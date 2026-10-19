#version 330

flat in float variation;
in float grassheight;
out vec4 finalColor;

void main()
{
    finalColor = vec4(
        (70.0 / 255.0) * variation,
        (140.0 / 255.0) * variation,
        (50.0 / 255.0) * variation,
        variation * grassheight
    );
}