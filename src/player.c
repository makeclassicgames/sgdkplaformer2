#include "player.h"
#include "sprt.h"

void player_init(){
    player.position.x = 20;
    player.position.y = 110;
    player.sprite = SPR_addSprite(&zera, player.position.x, player.position.y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    player.direction = IDLE;
    player.isJumping = FALSE;
    player.flip = FALSE;
    player.camera.x = 0;
    player.camera.y = 0;
    player.spd.dx = 0;
    player.spd.dy = 0;
}

void player_draw(){
    
    SPR_setPosition(player.sprite, player.position.x , player.position.y);
    SPR_setAnim(player.sprite, player.direction);
    SPR_setHFlip(player.sprite, player.flip);
    
}