
// convert this all to struct?
static int cardsAmmt = 2;

static int cardsInfo[102][3] = {
   {2, 1, 2},  // card width / special point location / ammount of ink
   {1, 0, 1}
};

static int cardsInk[102][18] = {
   {1, 2}, // inked squares, see the original card for reference
   {0}
};

char cardsStyle[102][2][30] = {
   {"Tentatek", "Tentatek.png"}, // name, img location
   {"Burst Bomb", "Burst Bomb.png"}
}; 