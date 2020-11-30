#include <windows.h>
#include <tchar.h>
#include "resources.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <atlstr.h>
#include "Blackjack_Game.h"

using namespace std;

/*
windows.h Documentation :
	Window Class Structure : https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassw
	Window Class Type : https://docs.microsoft.com/en-us/windows/win32/winmsg/about-window-classes
	Window Class Registering : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
	Window Creation : https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindoww
	Window Keyboard Events : https://docs.microsoft.com/en-us/windows/win32/inputdev/using-keyboard-input


	Menu WM_COMMAND : https://docs.microsoft.com/en-us/windows/win32/menurc/wm-command
*/

/*
	Table Green Image Path : ..\\tableGreen.bmp
	Fire Icon Image Path : ..\\FireIcon32x32.bmp
	Sources for learning :
	Microsoft Win32 API Documentation : https://docs.microsoft.com/en-us/windows/win32/api/winuser/
	ThePentamollisProject : https://www.youtube.com/watch?v=8GCvZs55mEM&t=5s&ab_channel=ThePentamollisProject
	Stackoverflow Win32 Api : https://riptutorial.com/Download/win32-api.pdf
	Compiler : ISO C++17 Standard

	Requirements :
	System - Windows ... might work on other Operating Systems
*/


/* Declarations */
HWND hWnd;
TCHAR buff[8192];
Blackjack* game;
Dealer* dealer;
Player* player;
Deck* deck;
int bet;
vector<HWND>gameWindow, mainWindow, drawnHands, helpWindow;

// Main Components
HWND hPlayButton, hMainHelpButton, hDisplayInfo, hTitle;
// Game Components
HWND hMenuButton, hDealerHand, hDealerValueLabel, hHitButton, hDoubleDownButton, hStayButton,
hBalanceLabel, hPlayerHand, hPlayerValueLabel, hBetButton, hBetTextField, hBetLabel;
HMENU hMenu; // Menu Handle

// Help Components
HWND hHelpDisplay, hExitButton, hHelpTitle, hStartText,
hObjectiveText, hPlayText, hHitText, hDoubleText, hStayText, hBustText, hBlackjackText,
hPayoutText, hAceText, hFaceText, hHelpButton;

/* Intialization */
int mainButWid = 120;
int mainButHei = 80;

const int MAXCHARINPUT = 9;

int windowHeight = 600;
int windowWidth = 800;

/* Functions */
void AddBetControls(HWND);
void AddGameButtons(HWND); // Adds Hit, Double Down, and Stay Buttons for gameplay
LPCWSTR AddLPCWSTR(LPCWSTR, LPCWSTR); // Adds two LPCWSTR together
void AddMenu(); // Menu Creation Function
void AddMainControls(HWND); // GUI Design Function
void AddGameControls(HWND); // Game GUI
void ClearBetButtons(); // Destroys bet buttons
void ClearGameButtons(); // Destroys game buttons
void ClearWindow(vector<HWND>); // Destroys window within Vector
void CreateHelpWindow(HWND); // Creates Help Window
void DrawCards(Dealer*, HWND); // Draws out the cards in hand
void DrawCards(Player*, HWND); // Draws out the cards in hand
void StopGame(HWND); // Stops the game
void IntToStrBuffer(int); // Integer into a LPCWSTR
void StartGame(); // Provides game display
void StrToStrBuffer(string); // String to Buffer
void UpdateBalance(); // Updates the balance label
void UpdateGame(); // Updates the Entire Game 
void UpdateHands(); // Updates the Hands of both players
void UpdateScores(); // Updates Player and Dealer's Score
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM); // Process Function 


void AddBetButtons(HWND hWnd) {
	hBetButton = CreateWindow(L"Button", L"Bet : ",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 3.25), (int)windowHeight / 3, 100, 50, hWnd, (HMENU)BETBUTTON, NULL, NULL);

	hBetTextField = CreateWindow(L"Edit", L"",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD | SS_CENTER | ES_NUMBER,
		(int)(windowWidth / 2), (int)windowHeight / 3, 100, 50, hWnd, NULL, NULL, NULL);
}

void AddGameButtons(HWND hWnd) { // Add Game Play Buttons
	hHitButton = CreateWindow(L"Button", L"Hit",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)windowWidth / 4, (int)windowHeight / 3, 100, 50, hWnd, (HMENU)HITBUTTON, NULL, NULL);
	if (player->getBalance() / 2 >= bet) {
		hDoubleDownButton = CreateWindow(L"Button", L"Double Down",
			WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
			(int)windowWidth / 2.25, (int)windowHeight / 3 - 20, 100, 50, hWnd, (HMENU)DDBUTTON, NULL, NULL);
		gameWindow.push_back(hDoubleDownButton);
	}
	hStayButton = CreateWindow(L"Button", L"Stay",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)windowWidth / 1.6, (int)windowHeight / 3, 100, 50, hWnd, (HMENU) STAYBUTTON, NULL, NULL);

	IntToStrBuffer(bet);
	hBetLabel = CreateWindow(L"Static", AddLPCWSTR(L"Betting :\t", buff),
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)windowWidth / 2.25, (int)windowHeight / 3 + 55, 100, 30, hWnd, NULL, NULL, NULL);

	gameWindow.push_back(hBetLabel);
	gameWindow.push_back(hHitButton);
	gameWindow.push_back(hStayButton);

}


void AddGameControls(HWND hWnd) { // Game Window
	// Top Design
	hMenuButton = CreateWindow(L"Button", L"\u21D0 Main Menu", 
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		25, 25, 100, 40, hWnd, (HMENU) BACKTOMENU , NULL, NULL);

	hDealerHand = CreateWindow(L"Static", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER, 
		(int)(windowWidth / 2) - mainButWid - 50, 25, 350, 100, hWnd, NULL, NULL, NULL);

	hDealerValueLabel = CreateWindow(L"Static", L"Dealer Score :\n0",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth - 180, 25, 100, 40, hWnd, NULL, NULL, NULL); 

	// Bottom Design
	hBalanceLabel = CreateWindow(L"Static", L"Balance :\n0",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		25, windowHeight - 300, 100, 40, hWnd, NULL, NULL, NULL);

	hPlayerHand = CreateWindow(L"Static", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		(int)(windowWidth / 2) - mainButWid - 50, windowHeight - 300, 350, 100, hWnd, NULL, NULL, NULL);

	hPlayerValueLabel = CreateWindow(L"Static", L"Player Score :\n0",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth - 180, windowHeight - 300, 100, 40, hWnd, NULL, NULL, NULL);

	// Dealer Side
	gameWindow.push_back(hMenuButton);

	gameWindow.push_back(hDealerHand);
	gameWindow.push_back(hDealerValueLabel);
	// Player Side
	gameWindow.push_back(hBalanceLabel);
	gameWindow.push_back(hPlayerHand);
	gameWindow.push_back(hPlayerValueLabel);
};


/* Adds items to GUI Handle */
void AddMainControls(HWND hWnd) { // Main Window
	// Top Design
	hTitle = CreateWindow(L"Static", L"Blackjack",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 50, mainButWid, mainButHei, hWnd, NULL, NULL, NULL);

	// Mid Design
	hPlayButton = CreateWindow(L"Button", L"\tPlay\t",
		WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 20, 150, mainButWid, mainButHei, hWnd, (HMENU)PLAYGAME, NULL, NULL);

	hMainHelpButton = CreateWindow(L"Button", L"\tHelp\t",
		WS_VISIBLE | WS_OVERLAPPED | WS_BORDER | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 20, 300, mainButWid, mainButHei, hWnd, (HMENU)HELPMENU, NULL, NULL);

	mainWindow.push_back(hPlayButton);
	mainWindow.push_back(hMainHelpButton);
	mainWindow.push_back(hTitle);
};

/* Creates menu */
void AddMenu(HWND hWnd) {
	hMenu = CreateMenu(); // Creates menu object
	// AppendMenu(Menu Instance, Usage Type, Arguement, String info);
	AppendMenu(hMenu, MF_STRING, HELPMENU, L"Help - F1");
	//AppendMenu(hMenu, MF_STRING, HIGHSCOREMENU, L"Highscores - F2");
	AppendMenu(hMenu, MF_STRING, EXITMENU, L"Exit - ESC");
	// SetMenu(Window Handle , Menu Instance);
	SetMenu(hWnd, hMenu); // Sets menu for window // 
}

LPCWSTR AddLPCWSTR(LPCWSTR a, LPCWSTR b) { // Combines two LPCWSTR into a new LPCWSTR 
	wstring strTemp = wstring(a) + b;
	LPCWSTR lpstr = strTemp.c_str();
	return lpstr;
}

// Clears window contents based on Handle Vector
void ClearWindow(vector<HWND> hWndVect) {
	int size = hWndVect.size();
	for (int i = 0; i < size; i++) {
		DestroyWindow(hWndVect.back());
		hWndVect.pop_back();
	}
}

void ClearBetButtons() {
	DestroyWindow(hBetButton);
	DestroyWindow(hBetTextField);
}

void ClearGameButtons() {
	DestroyWindow(hDoubleDownButton);
	DestroyWindow(hStayButton);
	DestroyWindow(hHitButton);
}

void CreateHelpWindow(HWND hWnd) {
	hHelpDisplay = CreateWindow(L"Static", L"",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		0, 0, 800, 600, hWnd, NULL, NULL, NULL);

	hHelpButton = CreateWindow(L"Button", L"Exit Help",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .75, windowHeight * .75, 100, 40, hWnd, (HMENU) EXITHELP, NULL, NULL);

	hStartText = CreateWindow(L"Static", L"Start : Enter a bet amount in the center of the interface that is less than or equal to your balance to begin and click the bet button.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .075, 50, 200, 100, hWnd, NULL, NULL, NULL);

	hObjectiveText = CreateWindow(L"Static", L"Objective : Achieving a higher score than the dealer by either beating the dealer's hand or busting the dealer.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .075, 160, 200, 80, hWnd, NULL, NULL, NULL);

	hFaceText = CreateWindow(L"Static", L"Face Cards : Face cards are a King, Queen, and Jack, each face card is valued at 10.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .075, 250, 200, 60, hWnd, NULL, NULL, NULL);

	hAceText = CreateWindow(L"Static", L"Aces : An Ace is valued at 1 and 11. If your score is less than or equal to 11 then the value of the Ace will be 11.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .075, 325, 200, 100, hWnd, NULL, NULL, NULL);

	hHelpTitle = CreateWindow(L"Static", L"How to Play",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .075 , 15, 80, 30, hWnd, NULL, NULL, NULL);

	hHitText = CreateWindow(L"Static", L"Hit : Gives a new card to your hand.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 50, mainButWid + 50, 40, hWnd, NULL, NULL, NULL);

	hDoubleText = CreateWindow(L"Static", L"Double Down : Doubles your bet and provides a final hit as long as you have never hit during the hand. Must have enough money in your balance in order to double down.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 100, 200, 115, hWnd, NULL, NULL, NULL);

	hStayText = CreateWindow(L"Static", L"Stay : Your hand is kept safe and play switches to the dealer to hit and stay.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 225, mainButWid + 50, 55, hWnd, NULL, NULL, NULL);

	hBustText = CreateWindow(L"Static", L"Bust : Your hand exceeds 21 resulting in a loss.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 290, mainButWid + 50, 40, hWnd, NULL, NULL, NULL);

	hBlackjackText = CreateWindow(L"Static", L"Blackjack : Your opening hand is exactly 21.",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		(int)(windowWidth / 2) - mainButWid + 50, 350, mainButWid + 50, 40, hWnd, NULL, NULL, NULL);

	hPayoutText = CreateWindow(L"Static", L"Payout : \n\n\nBet x Modifier + Balance\n\n\n2.5x - Blackjack\n\n\n1.5x - Win\n\n\n1x - Push (Tie)\n\n\n0x - Loss",
		WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
		windowWidth * .6875, 50, 200, 300, hWnd, NULL, NULL, NULL);


	helpWindow.push_back(hHelpButton);
	helpWindow.push_back(hHelpDisplay);
	helpWindow.push_back(hHelpTitle);
	helpWindow.push_back(hObjectiveText);
	helpWindow.push_back(hStartText);
	helpWindow.push_back(hHitText);
	helpWindow.push_back(hDoubleText);
	helpWindow.push_back(hStayText);
	helpWindow.push_back(hBustText);
	helpWindow.push_back(hBlackjackText);
	helpWindow.push_back(hFaceText);
	helpWindow.push_back(hAceText);
	helpWindow.push_back(hPayoutText);
}

void DrawCards(Dealer* dealer) {
	vector<Card*>* cards = dealer->getHand()->getCards();
	int size = cards->size();
	for (int i = 0; i < size; i++) {
		StrToStrBuffer(cards->at(i)->getValueStr());
		drawnHands.push_back(CreateWindow(L"Static", buff, WS_VISIBLE | WS_OVERLAPPED | WS_CHILD | SS_CENTERIMAGE | SS_CENTER | WS_BORDER, 10 + (55 * i), 10, 50, 80, hDealerHand, NULL, NULL, NULL));
		if (game->waitingAction()) { return; }
	}
}

void DrawCards(Player* player) {
	vector<Card*>* cards = player->getHand()->getCards();
	int size = cards->size();
	for (int i = 0; i < size; i++) {
		StrToStrBuffer(cards->at(i)->getValueStr());
		drawnHands.push_back(CreateWindow(L"Static", buff, WS_VISIBLE | WS_OVERLAPPED | WS_CHILD | SS_CENTERIMAGE | SS_CENTER | WS_BORDER, 10 + (55 * i), 10, 50, 80, hPlayerHand, NULL, NULL, NULL));
	}
}

void IntToStrBuffer(int number) {
	wsprintfW(buff, L"%d", number); // Integer into the buffer
}

void StartGame() {
	game->runGame();
}

void StopGame(HWND hWnd) {
	game->stay();
	if (!game->isBust(player) && !game->isBlackjack(player)) {
		if (game->getScore(player) == game->getScore(dealer)) {
			SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Players Push");
			player->deposit(bet);
		}
		else if (game->is21(dealer)) {
			SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Dealer has 21");
		}
		else if (game->isBust(dealer)) {
			SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Dealer has busted");
			player->deposit(bet * 1.5);
		}
		else {
			if (game->getScore(player) > game->getScore(dealer)) {
				SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"You Won the hand");
				player->deposit(bet * 1.5);
			}
			else {
				SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"You lost the hand");
			}
		}
	}

	if (player->getBalance() == 0) {
		player->setBalance(500);
		SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Went Broke! Resetting!");
	}
	UpdateGame();
	ClearGameButtons();
	AddBetButtons(hWnd);
}

void StrToStrBuffer(string str) {
	_tcscpy_s(buff, CA2T(str.c_str()));
}

void UpdateBalance() {
	IntToStrBuffer(player->getBalance());
	SendMessage(hBalanceLabel, WM_SETTEXT, NULL, (LPARAM)AddLPCWSTR(L"Balance :\n", buff));
}

void UpdateGame() {
	UpdateBalance();
	UpdateHands();
	UpdateScores();
}

void UpdateHands() {
	ClearWindow(drawnHands);
	DrawCards(player);
	DrawCards(dealer);
}

void UpdateScores() {
	IntToStrBuffer(game->getScore(player));
	SendMessage(hPlayerValueLabel, WM_SETTEXT, NULL, (LPARAM)AddLPCWSTR(L"Score :\n", buff));

	int dealerScore = game->getScore(dealer);
	if (game->waitingAction() && dealer->getHand()->getCards()->size() > 0) {
		Card* card = dealer->getHand()->getCards()->at(0);
		dealerScore = card->getValue();
		if (dealerScore == ACE) {
			dealerScore += 10;
		}
	}
	StrToStrBuffer(to_string(dealerScore));
	SendMessage(hDealerValueLabel, WM_SETTEXT, NULL, (LPARAM)AddLPCWSTR(L"Score :\n", buff));
}

/* GUI Main Function */
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
	game = new Blackjack();
	dealer = game->getDealer();
	player = game->getPlayer();
	deck = game->getDeck();
	WNDCLASS wc = { 0 }; // WNDCLASSW is a structure
	LPCWSTR title = L"Blackjack"; // Long Pointer Constant Wide (UTF-16) String 
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW; // Background 
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Sets Cursor IDI_HAND
	wc.hInstance = hInst; // Instance of window
	wc.lpszClassName = L"windowClass"; // Class name
	wc.lpfnWndProc = WindowProcedure; // Pointer to the function // Controller of window handle
	if (!RegisterClassW(&wc)) { // Registers the window class
		return -1;
	}
	// WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX makes the window non resizable
	CreateWindow(wc.lpszClassName, title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 100, 100, windowWidth, windowHeight, NULL, NULL, NULL, NULL);
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, NULL, NULL) >= 0) { // Keeps the window running
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

/* Event Paths / Control Path */
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_CREATE: // On window creation
		AddMainControls(hWnd);
		AddMenu(hWnd);
		hDisplayInfo = CreateWindow(L"Static", L"F1 - Help\t\tBackspace - Main Menu\t\tESC - EXIT",
			WS_VISIBLE | WS_OVERLAPPED | WS_CHILD,
			100, windowHeight - 100, windowWidth, 100, hWnd, NULL, NULL, NULL);
		UpdateBalance();
		break;
	case WM_DESTROY: // Makes GetMessage Function return false, closing the window
		PostQuitMessage(0);
		exit(0);
		return 0;
	case WM_COMMAND: // When menu or buttons are pressed on
	case WM_KEYDOWN: // User Keyboard Events
		switch (wp) {
		case VK_ESCAPE: // ESC Quit Window
		case EXITMENU: // Exits Application
			DestroyWindow(hWnd);
			break;
		case HELPMENU:
		case VK_F1:
			CreateHelpWindow(hWnd);
			EnableWindow(hBetButton, 0);
			EnableWindow(hBetTextField, 0);
			EnableWindow(hMenuButton, 0);
			EnableWindow(hPlayButton, 0);
			EnableWindow(hMainHelpButton, 0);
			EnableWindow(hHitButton, 0);
			EnableWindow(hDoubleDownButton, 0);
			EnableWindow(hStayButton, 0);
			break;
		case EXITHELP:
			ClearWindow(helpWindow);
			EnableWindow(hBetButton, 1);
			EnableWindow(hBetTextField, 1);
			EnableWindow(hMenuButton, 1);
			EnableWindow(hPlayButton, 1);
			EnableWindow(hMainHelpButton, 1);
			EnableWindow(hHitButton, 1);
			EnableWindow(hDoubleDownButton, 1);
			EnableWindow(hStayButton, 1);
			break;
		case VK_BACK: // Backspace Main Menu
		case BACKTOMENU: // Main Window
			game->clearHands();
			ClearWindow(gameWindow);
			AddMainControls(hWnd);
			ClearBetButtons();
			break;
		case PLAYGAME: // Game window
			if (player->getBalance() == 0) {
				player->setBalance(500);
				SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Went Broke! Resetting!");
			}
			ClearWindow(mainWindow);
			AddGameControls(hWnd);
			AddBetButtons(hWnd);
			UpdateGame();
			SendMessage(hBetTextField, EM_SETLIMITTEXT, MAXCHARINPUT, 0); // Sets character limit
			break;
		case BETBUTTON:
			GetWindowText(hBetTextField, buff, 1024); // Sets bet text field to buff
			if (buff[0] != _T('\0')) { // Buff must be a string of numbers since the windows input is locked onto numbers
				if (stoi(buff) > player->getBalance()) {
					SendMessage(hBetTextField, WM_SETTEXT, NULL, (LPARAM)L""); // Clears text field input if greater than balance
					break;
				}
				bet = stoi(buff); // Stores the buff for later usage
				ClearBetButtons();
				AddGameButtons(hWnd);
				game->setBet(bet);
				StartGame();
				UpdateGame();
				if (game->isBlackjack(dealer) && game->isBlackjack(player)) {
					SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Players Push");
					player->deposit(bet);
					StopGame(hWnd);
				}
				else if (game->isBlackjack(dealer)) {
					SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Dealer has blackjack");
					StopGame(hWnd);
				}
				else if (game->isBlackjack(player)) {
					SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Player has blackjack");
					player->deposit(bet * 2.5);
					StopGame(hWnd);
				}
			};
			break;
		case HITBUTTON:
			dealer->dealCard(player, deck->removeTopCard());
			if (game->is21(player)) {
				SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Player has 21");
				player->deposit(bet * 1.5);
				StopGame(hWnd);
			}
			else if (game->isBust(player)) {
				SendMessage(hBetLabel, WM_SETTEXT, NULL, (LPARAM)L"Player has busted");
				StopGame(hWnd);
			}
			DestroyWindow(hDoubleDownButton);
			UpdateGame();
			break;
		case STAYBUTTON:
			StopGame(hWnd);
			break;
		case DDBUTTON:
			player->withdraw(bet);
			bet *= 2;
			dealer->dealCard(player, deck->removeTopCard());
			StopGame(hWnd);
			break;
		}
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}