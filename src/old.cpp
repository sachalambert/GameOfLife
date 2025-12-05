#include <cstdint>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

typedef uint64_t coord;

class Cells {
	public:
		coord rows, cols;
		bool* cells;
		bool* tempCells;

		Cells(coord rows, coord cols) : rows(rows), cols(cols) {
			cells = (bool*)calloc(rows*cols, sizeof(bool));
			tempCells = (bool*)malloc(rows*cols*sizeof(bool));
		};

		void randomize() {
			for (coord i = 0; i < rows*cols; i++) {
				cells[i] = rand() > (RAND_MAX >> 1);
			}
		}

		void randomizeSquare(coord x, coord y, coord l) {
			for (coord i = x; i < x+l; i++) {
				for (coord j = y; j < y+l; j++) {
					cells[i*cols + j] = rand() > (RAND_MAX >> 1);
				}
			}
		}

		bool read(coord x, coord y) {
			return cells[x*cols + y];
		}

		bool safeRead(coord x, coord y) {
			if (x < 0 || x >= rows || y < 0 || y >= cols) return false;
			return cells[x*cols + y];
		}

		void display() {
			/*const char* characters[] = {
				" ", // 0
				"▘", // 1
				"▝", // 2
				"▀", // 3
				"▖", // 4
				"▌", // 5
				"▞", // 6
				"▛", // 7
				"▗", // 8
				"▚", // 9
				"▐", // 10
				"▜", // 11
				"▄", // 12
				"▙", // 13
				"▟", // 14
				"█", // 15
			};

			for (unsigned short i = 0; i < height; i++) {
				for (unsigned short j = 0; j < width; j++) {
					unsigned char index = 0;

					if (cells[(2*i+0)*2*width + (2*j+0)]) index += 1;
					if (cells[(2*i+0)*2*width + (2*j+1)]) index += 2;
					if (cells[(2*i+1)*2*width + (2*j+0)]) index += 4;
					if (cells[(2*i+1)*2*width + (2*j+1)]) index += 8;

					printf("%s", characters[index]);
				}
				if (i < height - 1) printf("\n");
			}*/

			for (coord i = 0; i < rows; i++) {
				for (coord j = 0; j < cols; j++) {
					if (read(i, j)) {
						printf("█");
					} else {
						printf(" ");
					}
				}

				printf("\n");
			}
		}

		uint8_t neighbours(coord x, coord y) {
			unsigned char n = 0;

			n += safeRead(x-1, y-1);
			n += safeRead(x-1, y);
			n += safeRead(x-1, y+1);

			n += safeRead(x, y-1);
			n += safeRead(x, y+1);

			n += safeRead(x+1, y-1);
			n += safeRead(x+1, y);
			n += safeRead(x+1, y+1);

			return n;
		}

		bool updateSquare(coord i, coord j) {
				uint8_t n = neighbours(i, j);

				if (cells[i*cols + j]) {
					return n == 2 || n == 3;
				}
				return n == 3;
		}

		void update() {
			for (coord i = 0; i < rows; i++) {
				for (coord j = 0; j < cols; j++) {
					tempCells[i*cols + j] = updateSquare(i, j);
				}
			}

			bool* t = cells;
			cells = tempCells;
			tempCells = t;
		}
};

void init(coord* height, coord* width) {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	*height = w.ws_row;
	*width = w.ws_col;

	srand(time(NULL));
}

int main(void) {
	coord height, width;
	init(&height, &width);


	Cells cells = Cells(height, width);
	cells.randomize();

	for (coord i = 0; i < 1000; i++) {
	cells.display();
	cells.update();
	}

	return 0;
}
