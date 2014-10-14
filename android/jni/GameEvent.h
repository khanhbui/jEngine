class GameEvent{
public:
	enum {
		GAME_EVENT_MAINMENU = 1,
		GAME_EVENT_GAMEPLAY = 2,
		GAME_EVENT_GAMEOVER = 3,
		GAME_EVENT_SOUND_COIN = 4,

		ENGINE_EVENT_NEW_SCORE = 5,
		ENGINE_EVENT_LEADERBOARD = 6,
		ENGINE_EVENT_RATE_BUTTON = 7,
		ENGINE_EVENT_FINISH_TUTORIAL = 8
	};
};
