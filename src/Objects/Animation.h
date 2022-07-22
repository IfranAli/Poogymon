class Animation {

	public:
		bool Play_animation(int animation_group);
		void Stop_animation();
		void Set_animation_speed(float speed);
		void Tick();

	
	bool currently_playing {0};

	int animation_group {0};
	int animation_group_max {0};

	int current_frame {0};

	float speed {0.2};
	float current_frame_delta {0};
};
