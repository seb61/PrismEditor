#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "convert.h"

#define NORMAL  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN "\x1B[36m"
#define WHITE  "\x1B[37m"

typedef struct
{
  short entry_size;
  char capacity;

  int total_size;

  struct pk_info pokemon_info[7];

}Pokemon_list;

struct Savedata
{
  Pokemon_list *team_list;
  unsigned char *buffer;

  char filename[40];

  FILE *savefile;
  int filesize;

  unsigned short trainer_ID; // 0x2009 (2)

  char player_name[12]; // 0x200B (11)
  char rival_name[12]; // 0x2021 (11)
  char time_played[5]; // 0x2057 (4)

  int money; // 0x23dd (3)
};

typedef struct Savedata Savedata;

static void load_data(Savedata *s, struct pk_list *list)
{
  int i;

  // Player name
  for (i = 0; i < 11; i++)
  {
    s->player_name[i] = s->buffer[0x200B + i];
    if (s->player_name[i] == 0x50)
    {
      s->player_name[i] = '\0';
      break;
    }
  }
  convert_text(s->player_name, i);

  // Rival name
  for (i = 0; i < 11; i++)
  {
    s->rival_name[i] = s->buffer[0x2021 + i];

    if (s->rival_name[i] == 0x50)
    {
      s->rival_name[i] = '\0';
      break;
    }
  }
  convert_text(s->rival_name, i);

  // Trainer ID
  char TID[6];
  snprintf(TID, 5, "%0X%0X", s->buffer[0x2009], s->buffer[0x200A]);
  s->trainer_ID = strtol(TID, NULL, 16);

  // Time played
  for (i = 0; i < 4; i++)
    s->time_played[i] = s->buffer[0x2053 + i];

  // Money
  char temp[6];
  sprintf(temp, "%0X%0X%0X", s->buffer[0x23dc], s->buffer[0x23dd], s->buffer[0x23de]);

  s->money = strtol(temp, NULL, 16);

  // Pokemon
  i = 0;
  int j = 0;
  char n[100];
  for (i = 0; i < 6; i++, j++)
  {
    sprintf(n, "%X", list[s->buffer[0x2866 + i]].index_number);
    int f = strtol(n, NULL, 16);
    strcpy(s->team_list->pokemon_info[j].mon_name.species_name, list[f].species_name);
  }
  // moves
  int f = 0;
  for (i = 0; i < 0x30 * 0x6;)
  {
    for (j = 0; j < 4; j++)
      strcpy(s->team_list->pokemon_info[f].moves[j].move_name, list[s->buffer[0x286f + i + j]].move_name);

    i += 0x30;
    f++;
  }
  // 0x289f-0x286f
}

static void print_data(Savedata *s)
{
  printf(GREEN "Player name: "  "%s\n"NORMAL, s->player_name);
  printf(RED "Rival name: "  "%s\n"NORMAL, s->rival_name);
  printf(MAGENTA "Trainer ID: " "%05d\n"NORMAL , s->trainer_ID);
  printf(CYAN "Time played: %02d:%02d\n"NORMAL, s->time_played[0], s->time_played[1]);
  printf(YELLOW "Money: %03d\n\n"NORMAL, s->money);

  int i, j;
  for (i = 0; i < 6; i++)
  {
    printf(WHITE"%s\n\t"NORMAL, s->team_list->pokemon_info[i].mon_name.species_name);

    for (j = 0; j < 4; j++)
       printf(MAGENTA"%s "YELLOW"| "NORMAL, s->team_list->pokemon_info[i].moves[j].move_name);
    printf("\n");
  }
}

int main(int argc, const char **argv)
{
  Savedata *savedata = malloc(sizeof(Savedata));

  if (!savedata)
  {
    printf("Error allocating memory towards save data.\n");
    return 1;
  }

  savedata->team_list = malloc(sizeof(Pokemon_list));

  if (!savedata->team_list)
  {
    printf("Error allocating memory towards team list.\n");
    return 1;
  }

  savedata->team_list->entry_size = 48;
  savedata->team_list->capacity = 6;
  savedata->team_list->total_size = savedata->team_list->capacity *
                                  (savedata->team_list->entry_size + 23) + 2;

  memcpy(savedata->filename, argv[1], strlen(argv[1]));
  savedata->savefile = fopen(savedata->filename, "rb+");

  if (!savedata->savefile)
  {
    printf("Error opening file.\n");
    return 1;
  }

  fseek(savedata->savefile, 0, SEEK_END); // Probably not the best idea to do this

  savedata->filesize = ftell(savedata->savefile);
  rewind(savedata->savefile);

  savedata->buffer = malloc(sizeof(unsigned char) * savedata->filesize);

  if (!savedata->buffer)
  {
    printf("Error allocating memory.\n");
    return 1;
  }

  fread(savedata->buffer, savedata->filesize, sizeof(unsigned char), savedata->savefile);
  rewind(savedata->savefile);

  struct pk_list *pokemon_list = malloc(sizeof(struct pk_list) * 2000);
  load_pokemon_file(pokemon_list);

  load_data(savedata, pokemon_list);
  print_data(savedata);
  printf(NORMAL);
  free(pokemon_list);

  free(savedata->buffer);
  fclose(savedata->savefile);
  free(savedata);

  return 0;
}
