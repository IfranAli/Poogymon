#include "InputContext.h" 

#include <iostream>

namespace input
{
	void ExecuteKey (InputHandler& h, KEY key);

	InputHandler handler_default;

	InputHandler* p_handler {&handler_default};
	InputHandler* p_handler_prev {&handler_default};

	uint32_t uAcceptKeysMask = 0; 
	KEY current_key = KEY::NONE;

	// Holds captured mouse x, y.
	int mouse_x {0};
	int mouse_y {0};

	// Current event.
	SDL_Event current_event;

	InputContext::InputContext() {
		std::cout << "Input Context Initialised." << std::endl;
//		input::InitInputMaps();
	}

	InputContext::~InputContext() = default;

	void InputContext::HandleInput (SDL_Event& e)
	{
		if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_w :	    current_key = KEY::UP;		    break;
				case SDLK_a :	    current_key = KEY::LEFT;	    break;
				case SDLK_s :	    current_key = KEY::DOWN;	    break;
				case SDLK_d :	    current_key = KEY::RIGHT;	    break;

				case SDLK_UP:		current_key = KEY::ARROW_UP;		break;
				case SDLK_DOWN:		current_key = KEY::ARROW_DOWN;		break;
				case SDLK_LEFT:		current_key = KEY::ARROW_LEFT;		break;
				case SDLK_RIGHT:	current_key = KEY::ARROW_RIGHT;		break;

				case SDLK_o :	    current_key = KEY::A;	            break;
				case SDLK_k :	    current_key = KEY::B;	            break;

				case SDLK_q :	    current_key = KEY::L;	            break;
				case SDLK_e :	    current_key = KEY::R;	            break;

				case SDLK_p :	    current_key = KEY::START;		    break;
				case SDLK_l :	    current_key = KEY::SELECT;		    break;
			}
		}
		else if( e.type == SDL_KEYUP && e.key.repeat == 0 ) 
		{
			switch (e.key.keysym.sym)
			{
				case SDLK_o :	current_key = KEY::A_RELEASE;		break;
				case SDLK_k :	current_key = KEY::B_RELEASE;		break;

				case SDLK_w :	current_key = KEY::UP_RELEASE;		break;
				case SDLK_a :	current_key = KEY::LEFT_RELEASE;	break;
				case SDLK_s :	current_key = KEY::DOWN_RELEASE;	break;
				case SDLK_d :	current_key = KEY::RIGHT_RELEASE;	break;

				case SDLK_q :	current_key = KEY::L_RELEASE;		break;
				case SDLK_e :	current_key = KEY::R_RELEASE;		break;

				case SDLK_p :	current_key = KEY::START_RELEASE;	break;
				case SDLK_l :	current_key = KEY::SELECT_RELEASE;	break;
			}
		}
		else if( e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_GetMouseState(&mouse_x, &mouse_y);
			if (e.button.button == SDL_BUTTON_LEFT) {
				current_key = KEY::MOUSE_L;
			}
			else if(e.button.button == SDL_BUTTON_RIGHT) {
				current_key = KEY::MOUSE_R;
			}
		}
		if (p_handler) ExecuteKey(*p_handler, current_key);
		current_key = KEY::NONE;
	}

	void SetInputHandler (InputHandler& new_handler) 
	{
		if (p_handler != &new_handler) 
		{
			p_handler_prev = p_handler;
			p_handler = &new_handler;

			uAcceptKeysMask = p_handler->mask;

			ExecuteKey(*p_handler_prev, KEY::ON_DISABLE);
			ExecuteKey(*p_handler, KEY::ON_ENABLE);

//			printf("InputContext - Switching handlers!!\n");
		}
	}

	void SetPrevHandler() {
		SetInputHandler(*p_handler_prev);
	}

	void ExecuteKey (InputHandler& h, KEY key)
	{
		if ((1 << key) & h.mask) h.fn[key]();
	}

} /* input */
