//
//  Cell.cpp
//  jEngine
//
//  Created by Khanh Hoang Bui on 8/13/14.
//  Copyright (c) 2014 Khanh Bui. All rights reserved.
//

#include "Cell.h"

using namespace jQuiz;

const char * Cell::ROMANJI[] = {
    "r_a", "r_i", "r_u", "r_e", "r_o",
    "r_ka", "r_ki", "r_ke", "r_ku", "r_ko",
    "r_ga", "r_gi", "r_gu", "r_ge", "r_go",
    "r_sa", "r_shi", "r_su", "r_se", "r_so",
    "r_za", "r_zi", "r_zu", "r_ze", "r_zo",
    "r_ta", "r_chi", "r_tsu", "r_te", "r_to",
    "r_da", "r_ji", "r_du", "r_de", "r_do",
    "r_na", "r_ni", "r_nu", "r_ne", "r_no",
    "r_ha", "r_hi", "r_fu", "r_he", "r_ho",
    "r_ba", "r_bi", "r_bu", "r_be", "r_bo",
    "r_pa", "r_pi", "r_pu", "r_pe", "r_po",
    "r_ma", "r_mi", "r_mu", "r_me", "r_mo",
    "r_ya", "r_yu", "r_yo",
    "r_ra", "r_ri", "r_re", "r_ru", "r_ro",
    "r_wa", "r_wo", "r_n"
};

const char * Cell::HIRAGANA[] = {
    "a", "i", "u", "e", "o",
    "ka", "ki", "ke", "ku", "ko",
    "ga", "gi", "gu", "ge", "go",
    "sa", "shi", "su", "se", "so",
    "za", "zi", "zu", "ze", "zo",
    "ta", "chi", "tsu", "te", "to",
    "da", "ji", "du", "de", "do",
    "na", "ni", "nu", "ne", "no",
    "ha", "hi", "fu", "he", "ho",
    "ba", "bi", "bu", "be", "bo",
    "pa", "pi", "pu", "pe", "po",
    "ma", "mi", "mu", "me", "mo",
    "ya", "yu", "yo",
    "ra", "ri", "re", "ru", "ro",
    "wa", "wo", "n"
};

Cell::Cell(int GridX, int GridY) : Sprite(),
gridX(GridX),
gridY(GridY)
{
    setPosition(55 + gridX * (SIZE + 10), 180 + gridY * (SIZE + 10));
    load("empty", SIZE, SIZE, 0, 0);
}

Cell::~Cell()
{
}

void Cell::setChar(int Index, int Type)
{
    index = Index;
    switch (Type) {
        case 0:
            load(HIRAGANA[index], SIZE, SIZE, 0, 0);
            break;
        
        case 1:
            load(ROMANJI[index], SIZE, SIZE, 0, 0);
            break;

        default:
            break;
    }
}
