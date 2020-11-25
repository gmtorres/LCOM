#include "tile.h"

Tile tile (int i, uint16_t x, uint16_t y) {
  struct Tile ti;
  ti.id = i%(sizeof(tiles)/sizeof(tiles[0]));
  ti.spr = tiles[ti.id];
  ti.x = x;
  ti.y = y;
  return ti;
}

void drawTile (struct Tile* t) {
    drawBitmapFast(t->spr->bmp,t->x - t->spr->bmp->bitmapInfoHeader.width/2,t->y - t->spr->bmp->bitmapInfoHeader.height/2);
}

void initiateTiles(){
  tiles[0] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt01.bmp");//!
  tiles[1] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt02.bmp");//"
  tiles[2] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt03.bmp");//#
  tiles[3] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt04.bmp");//$
  tiles[4] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt05.bmp");//%
  tiles[5] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt06.bmp");//&
  tiles[6] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt07.bmp");//'
  tiles[7] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt08.bmp");//(
  tiles[8] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt09.bmp");//)
  tiles[9] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt10.bmp");//*
  tiles[10] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt11.bmp");//+
  tiles[11] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt12.bmp");//,
  tiles[12] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt13.bmp");//-
  tiles[13] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt14.bmp");//.
  tiles[14] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt15.bmp");///
  tiles[15] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt16.bmp");//0
  tiles[16] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt17.bmp");//1
  tiles[17] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt18.bmp");//2
  tiles[18] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt19.bmp");//3
  tiles[19] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt20.bmp");//4
  tiles[20] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt21.bmp");//5
  tiles[21] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt22.bmp");//6
  tiles[22] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt23.bmp");//7
  tiles[23] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt24.bmp");//8
  tiles[24] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt25.bmp");//9
  tiles[25] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt26.bmp");//:
  tiles[26] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt27.bmp");//;
  tiles[27] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt28.bmp");//<
  tiles[28] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt29.bmp");//=
  tiles[29] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt30.bmp");//>
  tiles[30] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt31.bmp");//?
  tiles[31] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt32.bmp");//@
  tiles[32] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt33.bmp");//A
  tiles[33] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt34.bmp");//B
  tiles[34] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt35.bmp");//C
  tiles[35] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt36.bmp");//D
  tiles[36] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt37.bmp");//E
  tiles[37] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt38.bmp");//F
  tiles[38] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt39.bmp");//G
  tiles[39] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt40.bmp");//H
  tiles[40] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt41.bmp");//I
  tiles[41] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt42.bmp");//J
  tiles[42] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt43.bmp");//K
  tiles[43] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt44.bmp");//L
  tiles[44] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt45.bmp");//M
  tiles[45] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt46.bmp");//N
  tiles[46] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt47.bmp");//O
  tiles[47] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt48.bmp");//P
  tiles[48] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt49.bmp");//Q
  tiles[49] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt50.bmp");//R
  tiles[50] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt51.bmp");//S
  tiles[51] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt52.bmp");//T
  tiles[52] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt53.bmp");//U
  tiles[53] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt54.bmp");//V
  tiles[54] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt55.bmp");//W
  tiles[55] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt56.bmp");//X
  tiles[56] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt57.bmp");//Y
  tiles[57] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt58.bmp");//Z
  tiles[58] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt59.bmp");//[
  tiles[59] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt60.bmp");/* \ */
  tiles[60] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt61.bmp");//]
  tiles[61] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt62.bmp");//^
  tiles[62] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt63.bmp");//_
  tiles[63] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt64.bmp");//`
  tiles[64] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt65.bmp");//a
  tiles[65] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt66.bmp");//b
  tiles[66] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt67.bmp");//c
  tiles[67] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt68.bmp");//d
  tiles[68] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt69.bmp");//e
  tiles[69] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt70.bmp");//f
  tiles[70] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt71.bmp");//g
  tiles[71] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt72.bmp");//h
  tiles[72] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt73.bmp");//i
  tiles[73] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt74.bmp");//j
  tiles[74] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt75.bmp");//k
  tiles[75] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt76.bmp");//l
  tiles[76] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt77.bmp");//m
  tiles[77] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt78.bmp");//n
  tiles[78] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt79.bmp");//o
  tiles[79] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt80.bmp");//p
  tiles[80] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt81.bmp");//q
  tiles[81] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt82.bmp");//r
  tiles[82] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt83.bmp");//s
  tiles[83] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt84.bmp");//t
  tiles[84] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt85.bmp");//u
  tiles[85] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt86.bmp");//v
  tiles[86] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt87.bmp");//w
  tiles[87] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt88.bmp");//x
  tiles[88] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt89.bmp");//y
  tiles[89] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/Asphalt_road/road_asphalt90.bmp");//z
  tiles[90] = sBmpInit("/home/lcom/labs/proj/resources/images/tiles/grass/land_grass04.bmp");//{


}
void freeTiles(){
  for(unsigned int i = 0; i < sizeof(tiles)/sizeof(tiles[0]); ++i)
    if(tiles[i] != NULL){
    freeBmp(tiles[i]);
    tiles[i] = NULL;
  }
}


void drawTileCamera(struct Tile*t , struct Camera * cm){
  struct Vector desl = cameraDeslocamento(cm);
  drawBitmapFast(t->spr->bmp, t->x - desl.x, t->y - desl.y);
}
