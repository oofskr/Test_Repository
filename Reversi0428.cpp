#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <conio.h>

std::atomic<bool> gotInput(false);
std::atomic<bool> timesUp(false);

const bool ShowValid = true;
const bool PreviewEat = true;
const int TimerSecond = 15;

char board[8][8];
int dx[] = { 0, 0, 1, -1, 1, 1, -1, -1 };
int dy[] = { 1, -1, 0, 0, 1, -1, 1, -1 };
std::vector<std::vector<std::vector<int>>> playback;
bool CancelMove = 0; // To determine the need of showing "Invalid move." (Will change after cancel a move at previewing stage)
bool BotMoving = 0; // To determine the need of previewing what will be eaten

void BoardOutput(char board[8][8]) {
	std::cout << "  0 1 2 3 4 5 6 7" << std::endl;
	for (int i = 0; i < 8; i++) {
		std::cout << i;
		for (int j = 0; j < 8; j++) {
			std::cout << ' ' << board[j][i];
		}
		std::cout << std::endl;
	}
}

void Flip(char board[8][8], std::vector<std::vector<int>> visited, char player) {
	for (size_t i = 0; i < visited.size(); i++) {
		int x = visited[i][0];
		int y = visited[i][1];
		board[x][y] = player;
	}
}

bool Move(int x, int y, char player, bool flip) { // flip = 0 -> previewing
	if (board[x][y] != '.') {
		return 0;
	}
	bool valid = 0;
	char opp = (player == 'B') ? 'W' : 'B'; // Opponent
	for (int i = 0; i < 8; i++) {
		std::vector<std::vector<int>> visited; // Use for Flip()
		int mx = x + dx[i], my = y + dy[i]; // Set point to check nearby block
		bool oppInBetween = 0;
		while (mx >= 0 && mx < 8 && my >= 0 && my < 8 && board[mx][my] == opp) {
			visited.push_back({ mx, my });
			mx += dx[i];
			my += dy[i];
			oppInBetween = 1;
		}
		if (mx >= 0 && mx < 8 && my >= 0 && my < 8 && board[mx][my] == player && oppInBetween == 1) {
			valid = 1;
			if (flip) {
				// Previewing what can eat
				if (PreviewEat && !BotMoving) {
					char board_previewEat[8][8];
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							board_previewEat[i][j] = board[i][j];
						}
					}
					board_previewEat[x][y] = 'X';			// The 'X' can be changed to "player" variable or other char
					Flip(board_previewEat, visited, 'X');	// The 'X' can be changed to "player" variable or other char
					std::cout << "\nPreview:" << std::endl;
					BoardOutput(board_previewEat);
					std::cout << "Confirm to put at (" << x << ", " << y << ")?" << std::endl;
					std::cout << "Enter '1' to confirm, enter '0' to cancel." << std::endl;

					// If you want to add countdown function, add at here...

					std::cin >> CancelMove;
					CancelMove = (CancelMove) ? 0 : 1; // Exchange from input
					if (CancelMove) return 0;

				}
				board[x][y] = player;
				Flip(board, visited, player);
			}
		}
	}
	return valid;
}

bool HasValidMove(char player) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (Move(j, i, player, false)) return 1;
		}
	}
	return 0;
}

void CountScore() {
	int b_score = 0, w_score = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[j][i] == 'B' ? b_score++ : w_score++;
		}
	}
	std::cout << "Black: " << b_score << ", White: " << w_score << std::endl;
	if (b_score > w_score) {
		std::cout << "Black Wins!" << std::endl;
	}
	else if (w_score > b_score) {
		std::cout << "White Wins!" << std::endl;
	}
	else {
		std::cout << "Tie!" << std::endl;
	}
	std::cout << "\nEnter '8 8' to reset the game, enter '9 9' to end the game." << std::endl;
}

void PrintBoard(char& player) { // Print the board and check valid move
	bool game_end = false;
	std::cout << "\n";
	if (!HasValidMove('W') && !HasValidMove('B')) {
		game_end = true;
	}
	if (!HasValidMove(player) && !game_end) {
		std::string p = (player == 'W') ? "White" : "Black";
		std::cout << p << " has no valid move." << std::endl;
		player = (player == 'W') ? 'B' : 'W'; // Change player
	}
	char board_tmp[8][8];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board_tmp[i][j] = board[i][j];
		}
	}
	if (ShowValid) { // Preview
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (Move(j, i, player, false)) board_tmp[j][i] = '!';
			}
		}
	}
	BoardOutput(board_tmp);
	if (game_end) {
		CountScore();
		return;
	}
	std::string currentplayer = (player == 'B') ? "Black's turn" : "White's turn";
	std::cout << currentplayer << std::endl;
}

void Reset(char& player) {
	player = 'B';
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[j][i] = '.';
		}
	}
	board[3][3] = board[4][4] = 'W';
	board[3][4] = board[4][3] = 'B';
	PrintBoard(player);
}

void input_timer(int& x, int& y, int seconds) {
	gotInput = false;
	auto start = std::chrono::steady_clock::now();
	std::string input = "";
	int lastCountDown = seconds;
	std::cout << "Enter move (x y): ";
	while (true) {
		auto now = std::chrono::steady_clock::now();
		int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
		int remaining = seconds - elapsed;
		if (remaining <= 0) {
			std::cout << "\rTime's up!                              \n";
			break;
		}

		if (remaining != lastCountDown) {
			lastCountDown = remaining;
			std::cout << "\rEnter move (x y): " << input << " [" << remaining << "s] " << std::flush;
		}

		if (_kbhit()) {
			char ch = _getch();
			if (ch == '\r') {
				std::cout << std::endl;
				if (sscanf_s(input.c_str(), "%d %d", &x, &y) == 2) {
					gotInput = true;
				}
				else {
					std::cout << "Invalid input format. Try again.\n";
					input.clear();
					std::cout << "Enter move (x y): ";
				}
				break;
			}
			else if (ch == '\b') {
				if (!input.empty()) {
					input.pop_back();
				}
			}
			else if (isprint(ch)) {
				input += ch;
			}

			std::cout << "\rEnter move (x y): " << input << " [" << remaining << "s] " << std::flush;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

int main() {
	char botMode;
	std::cout << "Enter 'L' to play with local player, enter 'B' to play with bot!\n";
	while (std::cin >> botMode) {
		if (botMode == 'L') {
			std::cout << "Enter '8 8' to reset the game, enter '9 9' to end the game." << std::endl;
			char player;
			Reset(player); // The default player is black
			int x, y;
			int match = 0; // load&save
			int step = 0;
			playback.resize(1);
			while (true) {
				x = -1; y = -1;
				input_timer(x, y, TimerSecond);
				if (!gotInput) {
					std::cout << "Timeout! Auto random move!\n";
					bool moved = false;
					for (int i = 0; i < 8 && !moved; i++) {
						for (int j = 0; j < 8 && !moved; j++) {
							if (Move(j, i, player, false)) {
								x = j;
								y = i;
								moved = true;
							}
						}
					}
					if (!moved) {
						std::cout << "No valid move!\n";
						player = (player == 'W') ? 'B' : 'W';
						PrintBoard(player);
						continue;
					}
				}
				if (x == 8 && y == 8) {
					match++;
					playback.resize(match + 1);
					std::cout << "Reset.\n\n";
					Reset(player);
					continue;
				}
				else if (x == 9 && y == 9) {
					return 0;
				}
				else if (x == 10 && y == 10) {
					std::cout << "input playback\n";
					std::cin >> x;
					if (x > playback.size()) {
						std::cout << "Invalid input";
						continue;
					}
					Reset(player);
					for (int i = 0; i < playback[x].size(); i++) {
						Move(playback[x][i][0], playback[x][i][1], player, true);
						player = (player == 'W') ? 'B' : 'W';
						if (i != playback[x].size() - 1)
							PrintBoard(player);
					}
				}
				else if (Move(x, y, player, true)) {
					player = (player == 'W') ? 'B' : 'W';
					playback[match].push_back({ x,y });
				}
				else if (!CancelMove) {
					std::cout << "Invalid move.\n";
					continue;
				}
				CancelMove = 0;
				PrintBoard(player);
			}
		}
		else if (botMode == 'B') {
			char humanPlayer;
			std::cout << "Do you want to go 1st as Black (B) or go 2nd as White (W)?\n(Enter B or W): ";
			std::cin >> humanPlayer;
			humanPlayer = toupper(humanPlayer);
			if (humanPlayer != 'B' && humanPlayer != 'W') {
				humanPlayer = 'B';
				std::cout << "Invalid choice. You are Black by default.\n";
			}
			std::cout << "Enter '8 8' to reset the game, enter '9 9' to end the game." << std::endl;
			char player;
			Reset(player); // The default player is black
			int x, y;
			int match = 0; // load&save
			int step = 0;
			playback.resize(1);
			while (true) {
				x = -1; y = -1;
				PrintBoard(player);

				if (player == humanPlayer) {
					input_timer(x, y, TimerSecond);

					if (!gotInput) {
						std::cout << "Timeout! Auto random move!\n";
						bool moved = false;
						for (int i = 0; i < 8 && !moved; i++) {
							for (int j = 0; j < 8 && !moved; j++) {
								if (Move(j, i, player, false)) {
									x = j;
									y = i;
									moved = true;
								}
							}
						}
						if (!moved) {
							std::cout << "No valid move!\n";
							player = (player == 'W') ? 'B' : 'W';
							continue;
						}
					}

					if (x == 8 && y == 8) {
						match++;
						playback.resize(match + 1);
						std::cout << "Reset.\n\n";
						Reset(player);
						continue;
					}
					else if (x == 9 && y == 9) {
						return 0;
					}
					else if (x == 10 && y == 10) {
						std::cout << "input playback\n";
						std::cin >> x;
						if (x > playback.size()) {
							std::cout << "Invalid input";
							continue;
						}
						Reset(player);
						for (int i = 0; i < playback[x].size(); i++) {
							Move(playback[x][i][0], playback[x][i][1], player, true);
							player = (player == 'W') ? 'B' : 'W';
							if (i != playback[x].size() - 1)
								PrintBoard(player);
						}
						continue;
					}
					else if (Move(x, y, player, true)) {
						playback[match].push_back({ x, y });
						player = (player == 'W') ? 'B' : 'W';
					}
					else if (!CancelMove) {
						std::cout << "Invalid move.\n";
						continue;
					}
					CancelMove = 0;
				}
				else {
					// BOT
					BotMoving = 1;
					std::cout << "BOT is thinking...\n";
					bool moved = false;
					for (int i = 0; i < 8 && !moved; i++) {
						for (int j = 0; j < 8 && !moved; j++) {
							if (Move(j, i, player, false)) {
								x = j;
								y = i;
								moved = true;
							}
						}
					}
					if (moved) {
						Move(x, y, player, true);
						playback[match].push_back({ x, y });
						player = (player == 'W') ? 'B' : 'W';
					}
					else {
						std::cout << "BOT has no valid move.\n";
						player = (player == 'W') ? 'B' : 'W';
					}
					BotMoving = 0;
				}
			}
		}
		else {
			std::cout << "Invalid input!!!\n";
		}
	}
}