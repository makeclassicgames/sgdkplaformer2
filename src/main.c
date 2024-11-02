/**
 * Hello World Example
 * Created With Genesis-Code extension for Visual Studio Code
 * Use "Genesis Code: Compile" command to compile this program.
 **/
#include <genesis.h>

#include "player.h"
#include "mapa2.h"

#include "gfx.h"
#include "sprt.h"

void inputHandle(u16, u16, u16);
void readInput(void);
void update_physics(void);

Map *map;
int main()
{
    u16 index = TILE_USER_INDEX;
    VDP_drawImageEx(BG_B, &fondo, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, index), 0, 0, TRUE, CPU);
    index += fondo.tileset->numTile;
    VDP_loadTileSet(&tilesetmapa2, index, DMA);
    PAL_setPalette(PAL1, palmapa2.data, DMA);
    map = MAP_create(&mapa2, BG_A, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, index));
    MAP_scrollTo(map, 0, 0);
    PAL_setPalette(PAL2, zera.palette->data, CPU);
    player_init();
    JOY_setEventHandler(inputHandle);
    while (1)
    {
        readInput();
        update_physics();
        player_draw();
        MAP_scrollTo(map, player.camera.x, player.camera.y);
        SPR_update();
        // For versions prior to SGDK 1.60 use VDP_waitVSync instead.
        SYS_doVBlankProcess();
    }
    return (0);
}

void readInput(void)
{
    u16 value = JOY_readJoypad(JOY_1);

    if (value & BUTTON_LEFT)
    {
        player.direction = LEFT;
        player.flip = TRUE;
        player.spd.dx = -1;
        if (player.position.x < 80 && player.camera.x > 0)
        {
            player.camera.x -= 1;
            player.spd.dx = 0;
        }
    }
    else if (value & BUTTON_RIGHT)
    {
        player.direction = RIGHT;
        player.flip = FALSE;
        player.spd.dx = 1;
        if (player.position.x > 160)
        {
            player.camera.x += 1;
            player.spd.dx = 0;
        }
    }
    else
    {
        player.direction = IDLE;
        player.spd.dx = 0;
    }
}

void inputHandle(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if ((changed & BUTTON_C & state) && !player.isJumping)
        {
            player.isJumping = TRUE;
            player.spd.dy = -8;
        }
    }
}

void update_physics(void)
{

    player.position.y += player.spd.dy;
    player.spd.dy += GRAVITY;
    u16 tile_x = (player.position.x + player.camera.x) >> 3;
    u16 tile_y = (player.position.y >> 3) + 4;
    if (collision_mapa2[tile_y * 128 + tile_x] != 1)
    {
        player.spd.dy = 0;
        player.isJumping = FALSE;
    }

    tile_x = (player.position.x + player.camera.x) >> 3;
    tile_y = (player.position.y >> 3)+3;

    if (player.direction == RIGHT)
    {
        tile_x += 3;
    }

    if (player.direction == LEFT)
    {
        tile_x -= 1;
    }

    if (collision_mapa2[tile_y * 128 + tile_x] != 1)
    {
        player.spd.dx = 0;
    }
    if (collision_mapa2[tile_y * 128 + tile_x] == 3)
    {
        if (player.direction == RIGHT)
        {
            player.spd.dx = 1;
            player.spd.dy = 3;
        }
        if (player.direction == LEFT)
        {
            player.spd.dx = -1;
            player.spd.dy = -3;
        }
    }

    if (collision_mapa2[tile_y * 128 + tile_x] == 4)
    {
        if (player.direction == RIGHT)
        {
            player.spd.dx = 1;
            player.spd.dy = -3;
        }
        if (player.direction == LEFT)
        {
            player.spd.dx = -1;
            player.spd.dy = 3;
        }
    }
    player.position.x += player.spd.dx;
}