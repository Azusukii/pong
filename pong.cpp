#include "pong.hpp"

Pong::Pong(): m_left_paddle(0, (400/2) - 50), m_right_paddle(400-50, (400/2) - 50) {
	SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_RESIZABLE, &m_game_window, &m_game_window_renderer);
	SDL_RenderSetLogicalSize(m_game_window_renderer, 400, 400);
}

void Pong::game_loop() { // dis da game loop
	bool running = true;
	while (running) {
		while (SDL_PollEvent(&m_game_window_event)) {
			switch (m_game_window_event.type) {
			case SDL_QUIT:
				running = false;
				break;
			}
			m_left_paddle.handle_input(m_game_window_event);
			m_right_paddle.handle_input(m_game_window_event);
	}
		update(1.0 / 60.0);
		draw();
	}
}

void Pong::update(double delta_time) {

}

void Pong::draw() {
	SDL_RenderClear(m_game_window_renderer);
	SDL_RenderPresent(m_game_window_renderer);
}