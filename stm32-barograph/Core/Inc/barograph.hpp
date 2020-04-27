#ifndef BAROGRAPH_HPP
#define BAROGRAPH_HPP

bool displayInit();

constexpr static inline int right(const std::tuple<int, int> &coords) { return std::get<0>(coords); }

constexpr static inline int bottom(const std::tuple<int, int> &coords) { return std::get<1>(coords); }

/**
 * Draws a line of data
 *
 * @param x left position
 * @param y top position
 * @param font to be used
 * @param format 'printf()' style format
 * @param ... 'printf()' style arguments
 * @return [right, bottom] coordinates pair
 * @see printf()
 */
std::tuple<int, int> drawString(int x, int y, const sFONT &font, const char *format, ...);

int getAltitude();

#define WHITE 1
#define BLACK 0

extern const char months[][12];

const int chartPoints = 28;
constexpr int ALT_BKP_REGISTER = 15;
constexpr int LAST_TIMESTAMP_REGISTER = 14;

#endif