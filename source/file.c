#include <stdio.h>
#include <string.h>

#include "data.h"

void load_pokemon_file(struct pk_list *output)
{
  FILE *f = fopen("data/pokemon_list.txt", "rb");

  if (!f) // NULL
  {
    printf("Error opening pokemon_list.txt; are you sure you have the file?");
    return;
  }

  fseek(f, 0, SEEK_END);
  long length = ftell(f);
  rewind(f);

  char buffer[length]; // temp
  fread(buffer, sizeof(char), length, f);

  int dex = 1;

  char *token = strtok(buffer, "\n");

  int i;
  for (i = 0; i < length - 1; i++)
  {
    while (buffer[i] != '\n')
      i++;
    output[dex].index_number = dex;

    token = strtok(NULL, "\n");
    strncpy(output[dex].species_name, token, strlen(token));
    dex++;

  }

  fclose(f);

  f = fopen("data/move_list.txt", "rb");

  if (!f) // NULL
  {
    printf("Error opening move_list.txt; are you sure you have the file?");
    return;
  }

  fseek(f, 0, SEEK_END);
  length = ftell(f);
  rewind(f);
  char buffer_m[length];
  fread(buffer_m, sizeof(char), length, f);
  int move = 1;
  strcpy(output[0].move_name, "NO MOVE");
  token = strtok(buffer_m, "\n");
  for (i = 0; i < length - 1; i++)
  {
    while (buffer_m[i] != '\n')
      i++;
    token = strtok(NULL, "\n");
    strncpy(output[move].move_name, token, strlen(token));
    move++;
  }

  fclose(f);
}
