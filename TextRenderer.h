#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

class TextRenderer {
public:
    TextRenderer();
    ~TextRenderer();

    bool init(const char* fontPath, int fontSize);
    void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
    // helper function to get centered text and title
    void renderCenteredText(SDL_Renderer* renderer, const std::string& text, int y, SDL_Color color);
	void renderScore(SDL_Renderer* renderer, int leftScore, int rightScore);    
    void clean();

private:
    TTF_Font* font;
};
