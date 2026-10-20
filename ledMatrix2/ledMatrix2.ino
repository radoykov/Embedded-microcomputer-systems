#include <Adafruit_NeoPixel.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

#define ROWS        8
#define COLS        32
#define NUM_PIXELS  (ROWS * COLS)
#define BRIGHTNESS  64    
#define SHORT_DELAY 200

#define LED_PIN	12

typedef int (*effect_ptr)(uint8_t [ROWS][COLS][3]);

int display_rgb(uint8_t buff[ROWS][COLS][3]);
int simple_rainbow(uint8_t buff[ROWS][COLS][3]);
int red_blue_sine(uint8_t buff[ROWS][COLS][3]);
int green_sine(uint8_t buff[ROWS][COLS][3]);
int raindrops(uint8_t buff[ROWS][COLS][3]);
int wave(uint8_t buff[ROWS][COLS][3]);

const effect_ptr effects[] = {bounce};

Adafruit_NeoPixel led_matrix(NUM_PIXELS, LED_PIN, NEO_GRB);

void setup()
{
	// Initialize the matrix to all black and set the brightness
	led_matrix.begin();
	led_matrix.setBrightness(BRIGHTNESS);
	led_matrix.show();
}

void loop()
{
	uint8_t frame_buffer[ROWS][COLS][3] = {{{0}}};

	for (size_t i = 0; i < (sizeof(effects) / sizeof(effects[0])); i++)
	{
		effects[i](frame_buffer);
	}
}

int display_rgb(uint8_t buff[ROWS][COLS][3])
{
	if (buff == NULL)
		return -1;

	for (uint8_t col = 0; col < COLS; col += 2)
	{
		for (uint8_t row = 0; row < ROWS; row++)
		{
			led_matrix.setPixelColor(col * ROWS + row,
				led_matrix.Color(buff[row][col][0], buff[row][col][1], buff[row][col][2]));
		}
	}
	for (uint8_t col = 1; col < COLS; col += 2)
	{
		for (uint8_t row = 0; row < ROWS; row++)
		{
			led_matrix.setPixelColor((col + 1) * ROWS - row - 1,
				led_matrix.Color(buff[row][col][0], buff[row][col][1], buff[row][col][2]));
		}
	}
	led_matrix.show();

	return 0;
}

int simple_rainbow(uint8_t buff[ROWS][COLS][3])
{
	static const uint8_t rainbow_colors[7][3] = {
		{0xE5, 0x00, 0x00},
		{0xFF, 0x8D, 0x00},
		{0xFF, 0xEE, 0x00},
		{0x02, 0x81, 0x21},
		{0x00, 0x4C, 0xFF},
		{0x76, 0x00, 0x88},
		{0x00, 0x00, 0x00}
	};

	if (buff == NULL)
		return -1;

	for (size_t t = 0; t < COLS * 2; t++)
	{
		for (uint8_t col = 0; col < COLS; col++)
		{
			uint8_t color_index = col + t > COLS - 1 ? (col + t - COLS) / 5 : 6;
			if (color_index > 6)
			{
				color_index = 6;
			}
			for (uint8_t row = 0; row < ROWS; row++)
			{
				buff[row][col][0] = rainbow_colors[color_index][0];
				buff[row][col][1] = rainbow_colors[color_index][1];
				buff[row][col][2] = rainbow_colors[color_index][2];
			}
		}
		display_rgb(buff);
		delay(SHORT_DELAY);
	}

	return 0;
}

int red_blue_sine(uint8_t buff[ROWS][COLS][3])
{
	if (buff == NULL)
		return -1;

	for (size_t t = 0; t < COLS * 2; t++)
	{
		for (uint8_t col = 0; col < COLS; col++)
		{
			for (uint8_t row = 0; row < ROWS; row++)
			{
				buff[row][col][0] = (uint8_t)(255.0 * sinf(PI * (float)((row + t) % ROWS) / (float) ROWS));
				buff[row][col][1] = 0;
				buff[row][col][2] = (uint8_t)(255.0 * sinf(PI * (float)((col + t) % COLS) / (float) COLS));
			}
		}
		display_rgb(buff);
		delay(SHORT_DELAY);
	}

	return 0;
}

int green_sine(uint8_t buff[ROWS][COLS][3])
{
	if (buff == NULL)
		return -1;

	for (size_t t = 0; t < COLS * 2; t++)
	{
		for (uint8_t col = 0; col < COLS; col++)
		{
			for (uint8_t row = 0; row < ROWS; row++)
			{
				buff[row][col][0] = 0;
				buff[row][col][1] = (uint8_t)(255.0 * sinf(PI * (float)((col + t) % COLS) / (float) COLS));
				buff[row][col][2] = 0;
			}
		}
		display_rgb(buff);
		delay(SHORT_DELAY);
	}

	return 0;
}

int raindrops(uint8_t buff[ROWS][COLS][3])
{
	if (buff == NULL)
		return -1;

	for (size_t t = 0; t < 50; t++)
	{
		for (uint8_t col = 0; col < COLS; col++)
		{
			for (uint8_t row = 0; row < ROWS; row++)
			{
				if (rand() % 20 == 0)
				{
					buff[row][col][0] = 0x00;
					buff[row][col][1] = 0x05;
					buff[row][col][2] = 0x1D;
				}
				else
				{
					buff[row][col][0] = 0x00;
					buff[row][col][1] = 0x0D;
					buff[row][col][2] = 0x64;
				}
			}
		}
		display_rgb(buff);
		delay(2 * SHORT_DELAY);
	}

	return 0;
}
int bounce(uint8_t buff[ROWS][COLS][3])
{
	static const uint8_t colors[5][3] = {
		{0xFF, 0x00, 0x30},
		{0x00, 0xE0, 0xFF},
		{0xFF, 0xE0, 0x00},
		{0x30, 0xFF, 0x30},
		{0xC0, 0x20, 0xFF}
	};
	const int radius = 12;   
	const int period = 40;   

	if (buff == NULL)
		return -1;

	int x = COLS - 1, y = 0;   
	int dx = -1, dy = 1;

	for (int t = 0; t < 400; t++)
	{
		int i = (t / period) % 5;
		long f = (t % period) * 255L / period;
		uint8_t blob[3], bg[3];
		for (int c = 0; c < 3; c++)
		{
			blob[c] = colors[i][c] + ((long)colors[(i + 1) % 5][c] - colors[i][c]) * f / 255;
			bg[c]   = (colors[(i + 2) % 5][c] + ((long)colors[(i + 3) % 5][c] - colors[(i + 2) % 5][c]) * f / 255) / 2;
		}

		for (int col = 0; col < COLS; col++)
		{
			for (int row = 0; row < ROWS; row++)
			{
				int d = max(abs(row - y) * (COLS / ROWS), abs(col - x));
				long k = constrain((radius - d) * 510L / radius, 0, 255);

				for (int c = 0; c < 3; c++)
					buff[row][col][c] = bg[c] + ((long)blob[c] - bg[c]) * k / 255;
			}
		}
		display_rgb(buff);
		delay(SHORT_DELAY / 5);

		x += dx;
		y += dy;
		if (x <= 0)        dx = 1;
		if (x >= COLS - 1) dx = -1;
		if (y <= 0)        dy = 1;
		if (y >= ROWS - 1) dy = -1;
	}

	return 0;
}