#include "game.h"

/** Returns a random int
 * between the given <em>low</em> and <em>high</em> bounds, inclusive.
 */
static size_t rand_bound(size_t low, size_t high)
{
	size_t r;
	do {
		r = round(genrand64_real1());
	} while (r < low || r > high);
	return r;
}

/* Modern Fisher-Yates per Wikipedia.
 * /wiki/Fisher%E2%80%93Yates_shuffle#The_modern_algorithm
*/
static void shuffle_tiles(struct tile *a, size_t top)
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	init_genrand64(tp.tv_nsec); /* Seed the PRNG for the shuffling. */

	for (size_t i = top - 1; i > 1; --i) {
		size_t j = rand_bound(0, i);
		struct tile swap = a[i];
		a[i] = a[j];
		a[j] = swap;
	}
}

/** Creates all tiles, unshuffled.
 * @remarks //TODO// Implement distinct prey animals
 * 	Need update to distinct prey animals.
 * 	Reminder comments present for which animal is present.
 * 	Remember that animals are at lake edges.
 */
static void init_deck(struct tile deck[TILE_COUNT])
{
	/* Tileset: http://cise.ufl.edu/~dts/cen3031/TigerZone%20-%20Tournament%20Rules%20v2.2.pdf */
	/* Format: {(top), (right), (bottom), (left), (centre)} */
	size_t ind = 0;

		/* start tile, must be first. */
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, GAMETRAIL, JUNGLE, GAMETRAIL}, NONE);

		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, JUNGLE, JUNGLE, JUNGLE, JUNGLE}, NONE);

	for (int i = 0; i < 4; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, JUNGLE, JUNGLE, JUNGLE, JUNGLE}, DEN);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, JUNGLE, GAMETRAIL, JUNGLE, JUNGLE}, DEN);

		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, GAMETRAIL, GAMETRAIL, GAMETRAIL, GAMETRAIL}, NONE);

	for (int i = 0; i < 8; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, JUNGLE, GAMETRAIL, JUNGLE, GAMETRAIL}, NONE);

	for (int i = 0; i < 9; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, JUNGLE, JUNGLE, GAMETRAIL, GAMETRAIL}, NONE);

	for (int i = 0; i < 4; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, JUNGLE, GAMETRAIL, GAMETRAIL, GAMETRAIL}, NONE);

		deck[ind++] = make_tile((enum edge[5])
			{LAKE, LAKE, LAKE, LAKE, LAKE}, NONE);

	for (int i = 0; i < 4; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, LAKE, LAKE, LAKE, LAKE}, NONE);

	for (int i = 0; i < 5; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{LAKE, LAKE, JUNGLE, JUNGLE, LAKE}, NONE);

	for (int i = 0; i < 3; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, LAKE, JUNGLE, LAKE, LAKE}, NONE);

	for (int i = 0; i < 3; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{LAKE, JUNGLE, LAKE, JUNGLE, JUNGLE}, NONE);

	for (int i = 0; i < 5; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{LAKE, JUNGLE, JUNGLE, JUNGLE, JUNGLE}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, LAKE, LAKE, JUNGLE, JUNGLE}, NONE);

		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, JUNGLE, GAMETRAIL, GAMETRAIL}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, JUNGLE, GAMETRAIL, GAMETRAIL}, PREY); // Boar

		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, LAKE, GAMETRAIL, GAMETRAIL, GAMETRAIL}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{JUNGLE, LAKE, GAMETRAIL, GAMETRAIL, GAMETRAIL}, PREY); // Buffalo

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, GAMETRAIL, JUNGLE, GAMETRAIL}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, GAMETRAIL, JUNGLE, GAMETRAIL}, PREY); // Deer

		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, LAKE, LAKE, LAKE}, NONE);

		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, GAMETRAIL, GAMETRAIL, GAMETRAIL}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, GAMETRAIL, GAMETRAIL, GAMETRAIL}, PREY); // Boar

	for (int i = 0; i < 3; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, LAKE, GAMETRAIL, LAKE}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, LAKE, GAMETRAIL, LAKE}, PREY); // Buffalo

		deck[ind++] = make_tile((enum edge[5])
			{LAKE, JUNGLE, GAMETRAIL, JUNGLE, GAMETRAIL}, NONE);

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{LAKE, JUNGLE, GAMETRAIL, JUNGLE, GAMETRAIL}, PREY); // Deer

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{GAMETRAIL, LAKE, LAKE, LAKE, LAKE}, PREY); // Crocodile

	for (int i = 0; i < 2; ++i)
		deck[ind++] = make_tile((enum edge[5])
			{LAKE, GAMETRAIL, LAKE, GAMETRAIL, GAMETRAIL}, NONE);

	assert(ind == TILE_COUNT);
	return;
}

void calculate_scores(struct game *g)
{
	struct feature **scratch = malloc(sizeof(*scratch) * g->features_used);
	size_t len = TILE_COUNT * TILE_COUNT * 4 * 3;
	update_scores(g->scores, scratch, g->features, &len);
	free(scratch);
}

void free_game(struct game *g)
{
	struct feature **scratch = malloc(sizeof(*scratch) * g->features_used);
	size_t len = TILE_COUNT * TILE_COUNT * 4 * 3;
	update_scores(g->scores, scratch, g->features, &len);
	for (size_t i = 0; i < len; ++i) {
		free(scratch[i]);
	}
	free(scratch);
}

/** Initialises the given game. */
void make_game(struct game *g)
{
	g->features_used = g->tiles_used = 0;
	for (size_t i = 0; i < PLAYER_COUNT; ++i) {
		g->scores[i] = 0;
		g->meeples[i] = MEEPLE_COUNT;
	}
	init_deck(g->tile_deck);
	/* Leave index 0 untouched. That index will be the starting tile. */
	shuffle_tiles(&g->tile_deck[1], TILE_COUNT - 1);
	g->board = make_board();
	memset(g->features, 0,
			sizeof(g->features[1]) * TILE_COUNT * TILE_COUNT * 4*3);
	return;
}

/** Writes the given deck into the given game's tile_deck by deep copy. */
void set_game_deck(struct game *g, struct tile *deck)
{
	memcpy(g->tile_deck, deck, sizeof(*deck) * TILE_COUNT);
}

int is_move_valid(struct game *g, struct move m, int player)
{
	int rc;
	struct slot neighbors[4];
	struct slot *adjs[4];
	for (size_t i = 0; i < 4; ++i) {
		adjs[i] = &neighbors[i];
	}
	if ((rc = test_move_board(&g->board, m))) {
		return rc;
	}
	if ((rc = test_meeple(m, player, g->features))) {
		return rc;
	}
	return 0;
}

/** Tries to play the given move by the player on the given game. */
int play_move(struct game *g, struct move m, int player)
{
	int rc;
	struct slot adj[4];
	struct slot *adjs[4] = {&adj[0], &adj[1], &adj[2], &adj[3]};
	struct tile tadj[4];
	struct tile *tadjs[4] = {&tadj[0], &tadj[1], &tadj[2], &tadj[3]};

	list_adjacent_slots(m.slot, adjs);
	list_adjacent_tiles(&g->board, adjs, tadjs);

	for (int i = 0; i < 4; ++i) {
		if (tadjs[i] == NULL) {
			adjs[i] = NULL; /* Only want placed slots. */
		}
	}
	if ((rc = is_move_valid(g, m, player))) {
		return rc;
	}
	play_move_board(&g->board, m);
	play_move_feature(m, adjs, g->features, &g->features_used);
	play_meeple(m, player, g->features);
	if (m.tcorner > 0) {
		m.tcorner--;
	}
	if (m.ccorner > 0) {
		m.ccorner--;
	}
	return 0;
}

/** Returns whether the number of tiles dealt for the given game exceeds
 * <em>TILE_COUNT</em>.
 *
 * @see game.h: TILE_COUNT
 */
bool is_tile_deck_empty(struct game *g)
{
	return (TILE_COUNT - (g->tiles_used + 1)) <= 0;
}

/** Returns the next tile from the given game's tile_deck */
struct tile deal_tile(struct game *g)
{
	return g->tile_deck[g->tiles_used++];
}

void generate_available_moves(struct game *g, int player,
		struct tile t, struct move *pmoves, size_t *pmoves_len)
{
	struct move m;
	size_t num_moves = 0;
	size_t max_possible_moves = *pmoves_len;

	m.tile = t;
	for (size_t i = 0; i < g->board.empty_slot_count; i++){
		m.slot = g->board.slot_spots[i];
		for (int j = 0; j < 4; ++j) {
			m.rotation = j;
			//m.tcorner = m.ccorner = -1;
			/* TODO: Get working  */
			for (int k = -1; k < 13; ++k) {
				m.tcorner = k;
				for (int l = -1; l < 13; ++l) {
					m.ccorner = l;
					if (is_move_valid(g, m, player)) {
						continue;
					}
					pmoves[num_moves++] = m;
					if (num_moves == max_possible_moves) {
						*pmoves_len = num_moves;
						return;
					}
				}
			}
			if (is_move_valid(g, m, player)) {
				continue;
			}
			pmoves[num_moves++] = m;
			if (num_moves == max_possible_moves) {
				*pmoves_len = num_moves;
				return;
			}
		}
	}
	*pmoves_len = num_moves;
	return;
}

#ifdef TEST
int main(void)
{
	struct game g;
	make_game(&g);
	char buf[TILE_LEN];
	for (int i = 0; i < TILE_COUNT; ++i) {
		printf("%s\n", print_tile(deal_tile(&g), buf));
	}
	return 0;
}
#endif
