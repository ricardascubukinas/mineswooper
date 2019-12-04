#ifndef CON_LIB_H
#define CON_LIB_H

/// Spalvos
#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_ORANGE 3
#define COLOR_BLUE 4
#define COLOR_MAGENTA 5
#define COLOR_CYAN 6
#define COLOR_GRAY 7

/*#define FOREGROUND_WHITE		(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW		(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN			(FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA		(FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		0

#define FOREGROUND_INTENSE_RED	        (FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE	        (FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN	        (FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)

#define BACKGROUND_WHITE		(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BACKGROUND_YELLOW		(BACKGROUND_RED | BACKGROUND_GREEN)
#define BACKGROUND_CYAN	        	(BACKGROUND_BLUE | BACKGROUND_GREEN)
#define BACKGROUND_MAGENTA		(BACKGROUND_RED | BACKGROUND_BLUE)
#define BACKGROUND_BLACK		0

#define BACKGROUND_INTENSE_RED   	(BACKGROUND_RED | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_GREEN	(BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_BLUE	        (BACKGROUND_BLUE | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_WHITE	(BACKGROUND_WHITE | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_YELLOW	(BACKGROUND_YELLOW | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_CYAN	        (BACKGROUND_CYAN | BACKGROUND_INTENSITY)
#define BACKGROUND_INTENSE_MAGENTA	(BACKGROUND_MAGENTA | BACKGROUND_INTENSITY)
*/
// Išvalo ekraną
void con_clear();

// Nuskaito vieną klavišą.
// Gražina 0, jei nėra ko daugiau skaityti
int con_read_key();

// Nustato fono ir teksto spalvą
// * bg - fono spalva (COLOR_*)
// * fg - teksto spalva (COLOR_*)
void con_set_color(int bg, int fg);

// Nustato dabartinę išvedimo poziciją. x, y - koordinatės:
// * viršutinis kairys terminalo kampas yra (0, 0)
// * x-ai didėja iš kairės į dešinę
// * y-ai didėja iš viršaus į apačią
void con_set_pos(int x, int y);

// Nustato cursoriaus rodymo rėžimą.
// * Jei show == 0, cursorių paslepia
// * Jei show == 1, cursorių rodo
void con_show_cursor(int show);

// Padaro, kad nesimatytų įvedamų simbolių (tik LINUX os)
// * Jei show == 0, tai įvedamų simbolių neatkartoja į ekraną
// * Jei show == 1, tai įvedamus simbolius rodo
// * Ant Windows nieko nedaro
void con_show_echo(int show);

// Miega nurodytą sekundžių skaičių.
// * seconds turi būti intervale [0.01; 100.0]
void con_sleep(float seconds);

#endif // CON_LIB_H
