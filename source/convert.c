#include <stdio.h>
#include <string.h> // memcpy

#include "convert.h"

const char characters_english[0xFF] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ():;[]abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ():;[]abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ():;[]abcdefghijklmnopqrstuvwxyz";

void convert_text(char *text, int l)
{
  int i;
  for (i = 0; i < l; i++)
    text[i] = characters_english[(int) text[i] + 128];
}
