#pragma once

#define COURSE_WIDTH 512
#define COURSE_HEIGHT 16

#define GROUND_COURSE_WIDTH 17
#define GROUND_COURSE_HEIGHT 17

#define START_COURSE_WIDTH_1_2 17
#define START_COURSE_HEIGHT_1_2 17

#define GOAL_COURSE_WIDTH_1_2 48
#define GOAL_COURSE_HEIGHT_1_2 17


//コースの原本
//いじらないように


const char course[COURSE_HEIGHT][COURSE_WIDTH] = {
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                   ccc              cccc                           ccc                   ccc                       ccc              cccc                           ccc              cccc              g             ",
"                   ccc     ccccc    cccc                ccc        ccc                   ccc            ccc        ccc     ccccc    cccc                ccc        ccc     ccccc    cccc              g ccc         ",
"                           ccccc                        ccc                                             ccc                ccccc                        ccc                ccccc                      g ccc         ",
"                       q                                                        bbbbbbbb   bbbq              q           bbb    bqqb                                                        bb        g             ",
"                                                                                                                                                                                           bbb        g             ",
"                                                                                                                                                                                          bbbb        g             ",
"                                                                 q                                                                                                                       bbbbb        g     fff     ",
"                 q   bqbqb                    pp         pp                  bqb              b     bq    q  q  q     b          bb      b  b          bb  b            bbqb            bbbbbb        g     fff     ",
"  m                                   pp      pp  m      pp                                       m                                     bb  bb    m   bbb  bb                          bbbbbbb    m   g    fffff    ",
" mmm             m          pp        pp      pp mmm     pp      m                               mmm             m                     bbb  bbb  mmm bbbb  bbb   m pp              pp bbbbbbbb   mmm  g    fffff m  ",
"mmmmm      tttttmmm    ttt  pp        pp tttt ppmmmmm    pptttttmmm    ttt               tttt   mmmmm      tttttmmm    ttt            bbbbttbbbbmmmmbbbbb  bbbb mmmpp  ttt         ppbbbbbbbbb  mmmmm b    fffffmmm ",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};

const char sub_course[COURSE_HEIGHT][COURSE_WIDTH] = { //テクスチャの番号を割り振るためのコース
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                   012              0112                           012                   012                       012              0112                           012              0112              0             ",
"                   345 4   01112    3445                012        345               9ab 345  6         012  a     345     01112 bc 3445                012        345     01112    3445              1 012         ",
"                       C   34445                        345                          DDD      C         345  C             34445 CC                     345                34445                      1 345         ",
"                       0                                                        22222222   2220              0           222    2002                                                        11        1             ",
"                                                                                                                                                                                           111        1             ",
"                 1    2 3                     56         78      0            5                      f    7  8  9                                                         d               1111        1             ",
"                 C      C             34      DD         DD                                               C  C  C                                                         C              11111        1     000     ",
"                 0   20202  12        DD      01         01                  202              3     20    0  0  0     2          22      1  1          11  1            2202            111111        1     123     ",
"  0                         DD        01      23  0      23                                       0                                     11  11    0   111  11                          1111111    0   1    04440    ",
" 123             0          01        23      23 123     23      0                               123             0                     111  111  123 1111  111   0 01              01 11111111   123  1    22522 0  ",
"12453      01112123    012  23        23 0112 2312453    2301112123    012               0112   12453      01112123    012            1111021111124511111  1111 12323  012         23111111111  12453 1    22622123 ",
"000000000000000000000000000000000000000000000000000000000000000000000  000000000000000   0000000000000000000000000000000000000000000000000000000000000000  000000000000000000000000000000000000000000000000000000000",
"000000000000000000000000000000000000000000000000000000000000000000000  000000000000000   0000000000000000000000000000000000000000000000000000000000000000  000000000000000000000000000000000000000000000000000000000",
};


//1upキノコが出る隠しクエスチョンボックスを後から追加している。（「e」のクエスチョンボックス）
//レンガブロックに関して、hが１（コインが連続してでるやつ）に、jがf（とりあえず、スターが出るクエスチョンボックスに変わっている））
const char sub2_course[COURSE_HEIGHT][COURSE_WIDTH] = { //クエスチョンボックスやブロックの番号を割り振るためのコース
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                                                                                                                                                                                                                    ",
"                   ccc              cccc                           ccc                   ccc                       ccc              cccc                           ccc              cccc              g             ",
"                   ccc     ccccc    cccc                ccc        ccc                   ccc            ccc        ccc     ccccc    cccc                ccc        ccc     ccccc    cccc              g ccc         ",
"                           ccccc                        ccc                                             ccc                ccccc                        ccc                ccccc                      g ccc         ",
"                       4                                                        6789abcd   efg6              a           jkl    mbcn                                                        bb        g             ",
"                                                                                                                                                                                           bbb        g             ",
"                                                                                                                                                                                          bbbb        g             ",
"                                                                 e                                                                                                                       bbbbb        g     fff     ",
"                 1   12233                    pp         ii                  455              1     hf    7  8  9     i          op      b  b          bb  bb           qrds            bbbbbb        g     fff     ",
"  m                                   pp      pp  m      ii                                       m                                     bb  bb    m   bbb  bbb                         bbbbbbb    m   g    fffff    ",
" mmm             m          pp        pp      pp mmm     ii      m                               mmm             m                     bbb  bbb  mmm bbbb  bbbb  m pp              pp bbbbbbbb   mmm  g    fffff m  ",
"mmmmm      tttttmmm    ttt  pp        pp tttt ppmmmmm    iitttttmmm    ttt               tttt   mmmmm      tttttmmm    ttt            bbbbttbbbbmmmmbbbbb  bbbbbmmmpp  ttt         ppbbbbbbbbb  mmmmm b    fffffmmm ",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};


const char ground_course[GROUND_COURSE_HEIGHT][GROUND_COURSE_WIDTH] = {
"                ",
"                ",
"                ",
"b   bbbbbbb    p",
"b              p",
"b              p",
"b              p",
"b              p",
"b              p",
"b              p",
"b              p",
"b   bbbbbbb    p",
"b   bbbbbbb  ppp",
"b   bbbbbbb  ppp",
"bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbb",
};


const char ground_sub_course[GROUND_COURSE_HEIGHT][GROUND_COURSE_WIDTH] = {
"                ",
"                ",
"                ",
"2   2222222    2",
"2              2",
"2              2",
"2    DDDDD     2",
"2              2",
"2   DDDDDDD    2",
"2              2",
"2   DDDDDDD    2",
"2   2222222    2",
"2   2222222  468",
"2   2222222  579",
"0000000000000000",
"0000000000000000",
};


const char ground_sub2_course[GROUND_COURSE_HEIGHT][GROUND_COURSE_WIDTH] = {
"                ",
"                ",
"                ",
"1   2345678    p",
"9              p",
"a              p",
"b    12345     p",
"c              p",
"d   6789abc    p",
"e              p",
"f   defghij    p",
"g   hijklmn    p",
"o   bbbbbbb  iii",
"b   bbbbbbb  iii",
"bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbb",
};









//--------------------------
//1-2のコース
//--------------------------



const char course_1_2[COURSE_HEIGHT][COURSE_WIDTH] = {
"b                                                                                                                                                                                ",
"b                                                                                                                                                                                ",
"b     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                       bbbbbbbppbbbbbb",
"b                                                     bb  bbbbbb  bbbb      bbbb                                                                                         ppbbbbbb",
"b                                                     bb  bbbbbb  bbbb      bbbb                                                                                         ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                      b bbbb b     bb        bb   b    bb                                                                                               ppbbbbbb",
"b                            b         b b  b b     bb        bb   b q  bb          bbbbbb                                                                             ppppbbbbbb",
"b         qqqqq        b b             bbb  bbb     bbbb  bbbbbb   bb   bb  bbbb    bbbbbb                    pp                         bb       bbbbbb               ppppbbbbbb",
"b                    b b b b   b                      bb                                                pp    pp           bb           bbb                      bbbbbbbbbbbbbbbb",
"b                  b b b b b   b b                    bb                                                pp    pp    pp     bb          bbbb                      bbbbbbbbbbbbbbbb",
"b                b b b b b b   b b                                                                      pp    pp    pp     bb         bbbbb                      bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bb  bbbbbbbbbbbb       bbbbbbbb       bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bb  bbbbbbbbbbbb       bbbbbbbb       bbbbbbbbbbbbbbbb",
};


const char sub_course_1_2[COURSE_HEIGHT][COURSE_WIDTH] = {
"2                                                                                                                                                                                ",
"2                                                                                                                                                                                ",
"2     2222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222                       222222223222222",
"2                                                     22  222222  2222      2222                                                                                         23222222",
"2                                                     22  222222  2222      2222                                                                                         23222222",
"2                                                   22        22   2    222                                                                                              23222222",
"2                                                   22        22   2    22                                                                                               23222222",
"2                                                   22        22   2    22                                                                                               23222222",
"2         12345                        2 2222 2     22        22   2    22                                                                                               23222222",
"2          CCCC              2         2 2  2 2     22        22   2 0  22          222222                                                                             4683222222",
"2         00000        1 1             222  222     2222  222222   222  22  2222    222222                    01                         11       222222               5793222222",
"2                    1 1 1 1   1                      22                                                01    23           22           111                      2222222222222222",
"2                  1 1 1 1 1   1 1                    22                                                23    23    01     22          1111                      2222222222222222",
"2                1 1 1 1 1 1   1 1                                                                      23    23    23     22         11111                      2222222222222222",
"00000000000000000000000000000000000000000000000000000000000000000000000000000000   00000000000000000000000000000000000000  00  000000000000       00000000       0000000000000000",
"00000000000000000000000000000000000000000000000000000000000000000000000000000000   00000000000000000000000000000000000000  00  000000000000       00000000       0000000000000000",
};


const char sub2_course_1_2[COURSE_HEIGHT][COURSE_WIDTH] = {
"b                                                                                                                                                                                ",
"b                                                                                                                                                                                ",
"b     bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb                       bbbbbbbppbbbbbb",
"b                                                     bb  bbbbbb  bbbb      bbbb                                                                                         ppbbbbbb",
"b                                                     bb  bbbbbb  bbbb      bbbb                                                                                         ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                                   bb        bb   b    bb                                                                                               ppbbbbbb",
"b                                      b bbbb b     bb        bb   b    bb                                                                                               ppbbbbbb",
"b                            b         b b  b b     bb        bb   b 6  bb          bbbbbb                                                                             iiiibbbbbb",
"b         12345        b b             bbb  bbb     bbbb  bbbbbb   bbb  bb  bbbb    bbbbbb                    pp                         bb       bbbbbb               iiiibbbbbb",
"b                    b b b b   b                      bb                                                pp    pp           bb           bbb                      bbbbbbbbbbbbbbbb",
"b                  b b b b b   b b                    bb                                                pp    pp    pp     bb          bbbb                      bbbbbbbbbbbbbbbb",
"b                b b b b b b   b b                                                                      pp    pp    pp     bb         bbbbb                      bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bb  bbbbbbbbbbbb       bbbbbbbb       bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb   bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb  bb  bbbbbbbbbbbb       bbbbbbbb       bbbbbbbbbbbbbbbb",
};




const char start_course_1_2[START_COURSE_HEIGHT_1_2][START_COURSE_WIDTH_1_2] = {
"                ",
"                ",
"                ",
"   cccc         ",
"   cccc         ",
"                ",
"                ",
"                ",
"        ccc     ",
" fff    ccc     ",
" fff        pp  ",
"fffff       pp  ",
"fffff     pppp  ",
"fffff     pppp  ",
"bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbb",
};


const char start_sub_course_1_2[START_COURSE_HEIGHT_1_2][START_COURSE_WIDTH_1_2] = {
"                ",
"                ",
"                ",
"   0112         ",
"   3445         ",
"                ",
"                ",
"                ",
"        012     ",
" 000    345     ",
" 123        01  ",
"04440       23  ",
"22522     4683  ",
"22622     5793  ",
"0000000000000000",
"0000000000000000",
};

//壊せるブロックやクエスチョンボックスがないため、実質start_sub2_course_1_2は土管のためだけ
const char start_sub2_course_1_2[START_COURSE_HEIGHT_1_2][START_COURSE_WIDTH_1_2] = {
"                ",
"                ",
"                ",
"   cccc         ",
"   cccc         ",
"                ",
"                ",
"                ",
"        ccc     ",
" fff    ccc     ",
" fff        pp  ",
"fffff       pp  ",
"fffff     iiii  ",
"fffff     iiii  ",
"bbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbb",
};



const char goal_course_1_2[GOAL_COURSE_HEIGHT_1_2][GOAL_COURSE_WIDTH_1_2] = {
"                                ",
"                                ",
"                                ",
"   cccc               g         ",
"   cccc               g   ccc   ",
"                      g   ccc   ",
"            bbbbbbbbb g         ",
"           bbb        g         ",
"          bbbb        g         ",
"         bbbbb        g    fff  ",
"        bbbbbb        g    fff  ",
"       bbbbbbb        g   fffff ",
"   pp bbbbbbbb        g   fffff ",
"   ppbbbbbbbbb        b   fffff ",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};


const char goal_sub_course_1_2[GOAL_COURSE_HEIGHT_1_2][GOAL_COURSE_WIDTH_1_2] = {
"                                ",
"                                ",
"                                ",
"   0112               0         ",
"   3445               1         ",
"                      1         ",
"            111111111 1         ",
"           111        1         ",
"          1111        1         ",
"         11111        1    000  ",
"        111111        1    123  ",
"       1111111        1   04440 ",
"   01 11111111        1   22522 ",
"   23111111111        1   22622 ",
"00000000000000000000000000000000",
"00000000000000000000000000000000",
};


const char goal_sub2_course_1_2[GOAL_COURSE_HEIGHT_1_2][GOAL_COURSE_WIDTH_1_2] = { //実質的には必要ない
"                                ",
"                                ",
"                                ",
"   cccc               g         ",
"   cccc               g   ccc   ",
"                      g   ccc   ",
"            bb        g         ",
"           bbb        g         ",
"          bbbb        g         ",
"         bbbbb        g    fff  ",
"        bbbbbb        g    fff  ",
"       bbbbbbb        g   fffff ",
"   pp bbbbbbbb        g   fffff ",
"   ppbbbbbbbbb        b   fffff ",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
};