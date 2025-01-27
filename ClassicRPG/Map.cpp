//======================================
//	}bv
//======================================
#include "Map.h"
#include <assert.h>
// ævg^Cv
static bool isInMap(int x, int y);

MapArray mapField = {
        "~~~~~~~~~~~~~~~~",
        "~~MMMMM~~MMMM.~~",
        "~M...M.##..M...~",
        "~M.M.M.~~M.M.M.~",
        "~M.M...~~M...M.~",
        "~M.MMMM~~MMMM..~",
        "~M..MM.~~~~~~#~~",
        "~~M.M.~~~~~~~#~~",
        "~~M.MM~~~~BMM..~",
        "~~...MM~~M.MMM.~",
        "~...~~M~~M...M.~",
        "~..~~~K~~MMM.M.~",
        "~..~~~.~~M...M.~",
        "~......~~M.MM..~",
        "~~....~~~~....~~",
        "~~~~~~~~~~~~~~~~"
};
MapArray mapKingCastle = {
        "HHH.......HHH",
        "H.H.......H.H",
        "HHHHHHHHHHHHH",
        ".H.........H.",
        ".H.HHH.HHH.H.",
        ".H.H0H.H1H.H.",
        ".H.........H.",
        ".HW.......WH.",
        ".HY.......YH.",
        "HHHHHH.HHHHHH",
        "H.H~~~#~~~H.H",
        "HHH~~~#~~~HHH",
        ".............."
};
MapArray mapBossCastle = {
       "HHH.......HHH",
        "H.H.......H.H",
        "HHHHHHHHHHHHH",
        ".H....H....H.",
        ".H..WHHHW..H.",
        ".H..YH2HY..H.",
        ".H.........H.",
        ".H..W...W..H.",
        ".H..Y...Y..H.",
        ".H.........H.",
        "HHHHHH.HHHHHH",
        "H.H~~~#~~~H.H",
        "HHH~~~#~~~HHH",
        "~~~~~~#~~~~~~",
        "~~~~~~#~~~~~~",
        "............."
};

MapSpec specField = {
    &mapField,                // mapArray
    '~',                      // outData
    { MAP_NONE,0,0 },         // nextMap_Out
    { MAP_KING_CASTLE,6,12},  // nextMap_K
    { MAP_BOSS_CASTLE,6,15},  // nextMap_B
    true,                     // isBattleEncount
};

MapSpec specKingCastle = {
    &mapKingCastle,     // mapArray
    '.',                // outData
    { MAP_FIELD,6,12},  // nextMap_Out
    { MAP_NONE,0,0},    // nextMap_K
    { MAP_NONE,0,0},    // nexrMap_B
    false,              // isBattleEncount
};

MapSpec specBossCastle = {
    &mapBossCastle,     // mapArray
    '.',                // outData
    { MAP_FIELD,10,9},  // nextMap_Out
    { MAP_NONE,0,0},    // nextMap_K
    { MAP_NONE,0,0},    // nexrMap_B
    false,              // isBattleEncount
};
// id©ç}bvXybNðæ¾
MapSpec* GetMapSpec(MapId id)
{
    static MapSpec* specs[] = {
        &specField,
        &specKingCastle,
        &specBossCastle,
    };
    const int specsSize = sizeof(specs) / sizeof(specs[0]);
    assert(0 <= id && id < specsSize);
    return specs[id];
}
// }bvf[^ðæ¾
char GetMapData(MapSpec* spec, int x, int y)
{
    if (IsInMap(x, y)) {
        char c = (*spec->array)[y][x];
        if (c != '/0') {
            return c;
        }
    }
    return outOfMap;
}
// }bvÌAA(AXL[A[g)ðæ¾
const char* GetMapAA(char c)
{
    switch (c) {
    case '~':   return "`";  // C
    case '.':   return "D";  // ½n
    case 'M':   return "l";  // R
    case '#':   return "";  // ´
    case 'K':   return "¤";  // ¤lÌé
    case 'B':   return "";  // ¤Ìé
    case 'H':   return " ";  // Ç
    case 'W':   return "";  // 
    case 'Y':   return "x";  // Cä
    case '0':   return "¤";  // ¤
    case '1':   return "P";  // P
    case '2':   return "";  // ¤
    }
    assert(false);
    return "";
}
bool IsBattleEncount(MapSpec* spec)
{
    return spec->isBattleEncount;
}

static bool isInMap(int x, int y)
{
    return 0 <= x && x < MAP_WIDTH
        && 0 <= y && y < MAP_HEIGHT;
}