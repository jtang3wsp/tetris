#pragma once
#include <vector>

const int ROWS = 20;
const int COLUMNS = 10;
const int CELL_SIZE = 40;
const int WINDOW_ROWS = ROWS + 2;
const int WINDOW_COLUMNS = COLUMNS + 14;
const int BOARD_ROW_START = 2;
const int BOARD_ROW_END = BOARD_ROW_START + ROWS;
const int BOARD_COLUMN_START = 6;
const int BOARD_COLUMN_END = BOARD_COLUMN_START + COLUMNS;

const std::vector<std::vector<std::vector<int>>> JLSTZ_CLOCKWISE_WALLKICK_DATA{
	{{0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1}},
	{{0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1}},
	{{0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1}},
	{{0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1}}
};
const std::vector<std::vector<std::vector<int>>> JLSTZ_COUNTERCLOCKWISE_WALLKICK_DATA{
	{{0, 0}, {0, 1}, {1, 1}, {-2, 0}, {-2, 1}},
	{{0, 0}, {0, -1}, {-1, -1}, {2, 0}, {2, -1}},
	{{0, 0}, {0, -1}, {1, -1}, {-2, 0}, {-2, -1}},
	{{0, 0}, {0, 1}, {-1, 1}, {2, 0}, {2, 1}}
};
const std::vector<std::vector<std::vector<int>>> I_CLOCKWISE_WALLKICK_DATA{
	{{0, 0}, {0, -2}, {0, 1}, {1, -2}, {-2, 1}},
	{{0, 0}, {0, -1}, {0, 2}, {-2, -1}, {1, 2}},
	{{0, 0}, {0, 2}, {0, -1}, {-1, 2}, {2, -1}},
	{{0, 0}, {0, 1}, {0, -2}, {2, 1}, {-1, -2}}
};
const std::vector<std::vector<std::vector<int>>> I_COUNTERCLOCKWISE_WALLKICK_DATA{
	{{0, 0}, {0, 2}, {0, -1}, {-1, 2}, {2, -1}},
	{{0, 0}, {0, 1}, {0, -2}, {2, 1}, {-1, -2}},
	{{0, 0}, {0, -2}, {0, 1}, {1, -2}, {-2, 1}},
	{{0, 0}, {0, -1}, {0, 2}, {-2, -1}, {1, 2}}	
};