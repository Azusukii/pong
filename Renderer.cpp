#include "Renderer.h"
#include "Constants.h"

Renderer::Renderer(SDL_Renderer* sdlRenderer) : renderer(sdlRenderer) {
    std::cout << "Renderer constructor called with renderer: " << (renderer ? "valid" : "invalid") << std::endl;
}

Renderer::~Renderer() {}

void Renderer::clear() {
    if (!renderer) {
        std::cerr << "Error: Renderer is null in clear()" << std::endl;
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::render(const Ball& ball, const Paddle& leftPaddle, const Paddle& rightPaddle) {
    if (!renderer) {
        std::cerr << "Error: Renderer is null in render()" << std::endl;
        return;
    }
    // render the ball and paddles
    std::cout << "Rendering ball at: (" << ball.rect.x << ", " << ball.rect.y << ")" << std::endl;
    ball.render(renderer);
    
    std::cout << "Rendering left paddle at: (" << leftPaddle.rect.x << ", " << leftPaddle.rect.y << ")" << std::endl;
    leftPaddle.render(renderer);
    
    std::cout << "Rendering right paddle at: (" << rightPaddle.rect.x << ", " << rightPaddle.rect.y << ")" << std::endl;
    rightPaddle.render(renderer);

    // render center dividing line
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100); // white with a lil transparency
    for (int y = 0; y < Constants::SCREEN_HEIGHT; y += 20) {
		SDL_Rect lineSegment = { Constants::SCREEN_WIDTH / 2 - 2, y, 4, 10 };
		SDL_RenderFillRect(renderer, &lineSegment);
    }
}

void Renderer::present() {
    if (!renderer) {
        std::cerr << "Error: Renderer is null in present()" << std::endl;
        return;
    }
    SDL_RenderPresent(renderer);
}