#pragma once

enum
{
  HP,
  ATTACK,
  DEFENSE,
  SPATK,
  SPDEF,
  SPEED
};

struct pk_stats
{
  unsigned short current_stats[6];
  unsigned short iv[6];
  unsigned short ev[6];
};

struct pk_moves
{
  unsigned short move;
  char move_name[18];
  unsigned char pp;
};

struct pk_list
{
  unsigned short index_number;
  char species_name[18];

  char move_name[18];
};

struct pk_info
{
  unsigned char ability;
  unsigned short item;
  char nickname[18];


  unsigned char level;

  struct pk_list mon_name;
  struct pk_moves moves[4];
  struct pk_stats stats;
};
