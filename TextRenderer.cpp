#include "TextRenderer.h"
#include "Constants.h"
#include <iostream>

TextRenderer::TextRenderer() : normalFont(nullptr), titleFont(nullptr) {}

TextRenderer::~TextRenderer() {
	clean();
}

bool TextRenderer::init(const char* normalFontPath, const char* titleFontPath, int normalSize, int titleSize) {
	// load normal font
	normalFont = TTF_OpenFont(normalFontPath, normalSize);
	if (!normalFont) {
		std::cerr << "Normal font loading messed up! Error: " << TTF_GetError() << std::endl;
		return false;
	}
	// load title font
	titleFont = TTF_OpenFont(titleFontPath, titleSize);
	if (!titleFont) {
		std::cerr << "Title font loading messed up! Error: " << TTF_GetError() << std::endl;
		return false;
	}
	return true;
}

// render normal font
void TextRenderer::renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
	if (!normalFont) {
		std::cerr << "Normal font not loaded!" << std::endl;
		return;
	}
	// create a surface from text
	SDL_Surface* textSurface = TTF_RenderText_Solid(normalFont, text.c_str(), color);
	if (!textSurface) {
		std::cerr << "Text rendering messed up! Error: " << TTF_GetError() << std::endl;
		return;
	}
	// create texture from surface
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		std::cerr << "Texture creation messed up! Error: " << SDL_GetError() << std::endl;
		return;
	}
	// create rectangle and render
	SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

	// clean up
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

// render title font
void TextRenderer::renderTitleText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color) {
	if (!titleFont) {
		std::cerr << "Title font not loaded!" << std::endl;
		return;
	}
	// create a surface from text
	SDL_Surface* textSurface = TTF_RenderText_Solid(titleFont, text.c_str(), color);
	if (!textSurface) {
		std::cerr << "Title text rendering messed up! Error: " << TTF_GetError() << std::endl;
		return;
	}
	// create texture from surface
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	if (!textTexture) {
		std::cerr << "Title texture creation messed up! Error: " << SDL_GetError() << std::endl;
		return;
	}
	// create rectangle and render
	SDL_Rect textRect = { (Constants::SCREEN_WIDTH - textSurface->w) / 2, y, textSurface->w, textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	// clean up
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
void TextRenderer::renderScore(SDL_Renderer* renderer, int leftScore, int rightScore) {
	// create score text and center it
	std::string scoreText = std::to_string(leftScore) + " - " + std::to_string(rightScore);
	renderCenteredText(renderer, scoreText, 20, { 255,255,255,255 });
}
 
void TextRenderer::renderCenteredText(SDL_Renderer* renderer, const std::string& text, int y, SDL_Color color) {
	// calculate text dimensions for centering
	int textWidth, textHeight;
	TTF_SizeText(normalFont, text.c_str(), &textWidth, &textHeight);

	// horizontal center
	int x = (Constants::SCREEN_WIDTH - textWidth) / 2;

	renderText(renderer, text, x, y, color);
}

void TextRenderer::renderTitleCenteredText(SDL_Renderer* renderer, const std::string& text, int y, SDL_Color color) {
	// calculate text dimensions for centering
	int textWidth, textHeight;
	TTF_SizeText(titleFont, text.c_str(), &textWidth, &textHeight);
	// horizontal center
	int x = (Constants::SCREEN_WIDTH - textWidth) / 2;
	renderTitleText(renderer, text, x, y, color);
}
void TextRenderer::clean() {
	if (normalFont) {
		TTF_CloseFont(normalFont);
		normalFont = nullptr;
	}
	if (titleFont) {
		TTF_CloseFont(titleFont);
		titleFont = nullptr;
	}
}
