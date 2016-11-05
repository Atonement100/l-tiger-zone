#ifndef GAME_H_
#define GAME_H_

#include <stddef.h>	/* size_t */
#include <math.h>	/* round() */
#include <time.h>	/* clock_gettime() */
#include <assert.h>	/* assert() */

#include "limits.h"
#include "tile.h"
#include "mt19937-64.h"

#define TILE_COUNT 72

struct game {
	int graphs[100];
	struct tile tile_deck[TILE_COUNT];
	size_t graphs_used;
	size_t tiles_used;
};

struct game make_game(void);

#endif
