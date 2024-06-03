#include "Hamming.h"

#include <iostream>

bool IsPower2(int x) { return (x & (x - 1)) == 0; }

char DecodeHam74(const char* data) {
  char res = 0;
  char bits[16] = {0};
  uint8_t it = 0;

  for (int i = 0; i < 2; i++) {
    char current_char = data[i];
    for (int j = 0; j < 8; j++) {
      if ((current_char >> (7 - j)) & 1) {
        bits[i * 8 + j] = 1;
      }
    }
  }

  char parity_bits1[4];
  char parity_bits2[4];
  int error_bit1 = -1;
  int error_bit2 = -1;
  parity_bits1[0] = bits[2] ^ bits[4] ^ bits[6];
  parity_bits1[1] = bits[2] ^ bits[5] ^ bits[6];
  parity_bits1[2] = bits[4] ^ bits[5] ^ bits[6];
  parity_bits1[3] = parity_bits1[0] ^ parity_bits1[1] ^ parity_bits1[2];
  parity_bits2[0] = bits[10] ^ bits[12] ^ bits[14];
  parity_bits2[1] = bits[10] ^ bits[13] ^ bits[14];
  parity_bits2[2] = bits[12] ^ bits[13] ^ bits[14];
  parity_bits2[3] = parity_bits2[0] ^ parity_bits2[1] ^ parity_bits2[2];
  it = 0;
  for (int i = 1; i <= 8; i *= 2) {
    if (parity_bits1[it] != bits[i - 1]) {
      if (error_bit1 == -1) {
        error_bit1 = 0;
      }
      error_bit1 += i;
    }
    if (parity_bits2[it] != bits[i + 7]) {
      if (error_bit2 == -1) {
        error_bit2 = 0;
      }
      error_bit2 += i + 8;
    }
    ++it;
  }

  if (error_bit1 != -1) {
    if ((parity_bits1[3] != bits[7])) {
      std::cerr << "Unable to restore archive!";
      exit(EXIT_FAILURE);
    } else {
      if (bits[error_bit1 - 1]) {
        bits[error_bit1 - 1] = 0;
      } else {
        bits[error_bit1 - 1] = 1;
      }
    }
  }

  if (error_bit2 != -1) {
    if ((parity_bits2[3] != bits[15])) {
      std::cerr << "Unable to restore archive!";
      exit(EXIT_FAILURE);
    } else {
      if (bits[error_bit2 - 1]) {
        bits[error_bit2 - 1] = 0;
      } else {
        bits[error_bit2 - 1] = 1;
      }
    }
  }

  it = 0;
  int it2 = 0;
  for (int i = 0; i < 16; ++i) {
    if (!IsPower2(it2 + 1)) {
      if (bits[i]) {
        res |= (1 << (7 - it));
      }
      ++it;
    }
    ++it2;
    if (it2 == 8) {
      it2 = 0;
    }
  }
  return res;
}

char* EncodeHam74(const char& data) {
  char bits[8] = {0};
  for (int i = 7; i >= 0; --i) {
    if (data & (1 << i)) {
      bits[abs(i - 7)] = 1;
    }
  }

  char parity_bits1[4];

  parity_bits1[0] = bits[0] ^ bits[1] ^ bits[3];
  parity_bits1[1] = bits[0] ^ bits[2] ^ bits[3];
  parity_bits1[2] = bits[1] ^ bits[2] ^ bits[3];
  parity_bits1[3] = parity_bits1[0] ^ parity_bits1[1] ^ parity_bits1[2];

  char parity_bits2[4];

  parity_bits2[0] = bits[4] ^ bits[5] ^ bits[7];
  parity_bits2[1] = bits[4] ^ bits[6] ^ bits[7];
  parity_bits2[2] = bits[5] ^ bits[6] ^ bits[7];

  parity_bits2[3] = parity_bits2[0] ^ parity_bits2[1] ^ parity_bits2[2];

  char encoded1 = 0;
  char encoded2 = 0;
  int it = 0;
  int it2 = 0;
  for (int i = 0; i < 8; ++i) {
    if (IsPower2(i + 1)) {
      if (parity_bits1[it]) {
        encoded1 |= (1 << (7 - i));
      } else {
        encoded1 &= ~(1 << (7 - i));
      }
      ++it;
    } else {
      if (bits[it2]) {
        encoded1 |= (1 << (7 - i));
      }
      ++it2;
    }
  }
  if (parity_bits1[3]) {
    encoded1 |= 1;
  }
  it = 0;
  for (int i = 0; i < 8; ++i) {
    if (IsPower2(i + 1)) {
      if (parity_bits2[it]) {
        encoded2 |= (1 << (7 - i));
      } else {
        encoded2 &= ~(1 << (7 - i));
      }
      ++it;
    } else {
      if (bits[it2]) {
        encoded2 |= (1 << (7 - i));
      }
      ++it2;
    }
  }
  if (parity_bits2[3]) {
    encoded2 |= 1;
  }

  char* res = new char[2];
  res[0] = encoded1;
  res[1] = encoded2;

  return res;
}
