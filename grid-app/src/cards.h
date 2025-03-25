typedef struct {
   int width;
   int specialPointLocation;
   int inkAmount;
   int inkedSquares[18];
   char name[30];
   char imgLocation[30];
} Card;

static int cardsAmmt = 2;

static Card cards[102] = {
   {
      .width = 2,
      .specialPointLocation = 1,
      .inkAmount = 2,
      .inkedSquares = {1, 2},
      .name = "Tentatek",
      .imgLocation = "Tentatek.png"
   },
   {
      .width = 1,
      .specialPointLocation = 0,
      .inkAmount = 1,
      .inkedSquares = {0},
      .name = "Burst Bomb",
      .imgLocation = "Burst Bomb.png"
   }
};