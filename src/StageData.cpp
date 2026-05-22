//
// Created by link9 on 2026/5/10.
//

#include "StageData.hpp"

StageConfig StageData::GetStage(int stageNo) {
    static std::vector<StageConfig> stages = {
        // ===== Stage 1 =====
        {
            {
                ".............",
                ".B.B.B.B.B.B.",
                ".B.B.B.B.B.B.",
                ".B.B.BSB.B.B.",
                ".B.B.....B.B.",
                ".....B.B.....",
                "S.BB.....BB.S",
                ".....BDB.....",
                ".B.B.BTB.B.B.",
                ".B.B.B.B.B.B.",
                ".B.B.....B.B.",
                ".B.B.4D3.B.B.",
                ".....R.L....."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST
            }
        },

        // ===== Stage 2 =====
        {
            {
                "...S...S.....",
                ".B.S...B.B.B.",
                ".B....BB.BSB.",
                "...B.....S...",
                "G..B..S..BGBS",
                "GG...B..S.G..",
                ".BBBGGGS..G..",
                "...SGB.B.B.B.",
                "...S.B.B...B.",
                "SB.B.BBB.BSB.",
                ".B.B.BBB.....",
                ".B...4D3.B.B.",
                ".B.B.R.L.BBB."
            },
            {
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL
            }
        },

        // ===== Stage 3 =====
        {
            {
                "....B...B....",
                ".GGGB.....ddd",
                "BGGG.........",
                "GGGG...B.BBBL",
                "GGGGBBBT.B.R.",
                "GGGG..B.S..R.",
                ".G....SSS..G.",
                ".D..D....GGGG",
                "BLRBLRTTTGGGG",
                ".....B.DDGGGG",
                "B..l...TTGGG.",
                "BB.l.4D3.GGG.",
                "SBB..R.L.B..."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL, //
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 4 =====
        {
            {
                ".GG........G.",
                "GG..DBBDD...G",
                "G..RBBBBBBD.t",
                "t..BTTTBBBTL.",
                "..RT...TBB...",
                "W.R.l.r.BL...",
                "..B.DD..BL.WW",
                "..BBBBBBBB...",
                ".RBBBBBBBBL..",
                ".TTBBBBBBTT..",
                ".BBDTBBTDBB.G",
                "G.TT.4D3TT.GG",
                "SG...R.L..GGS"
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 5 =====
        {
            {
                "....BB.......",
                "d.D.B...ttS..",
                "S.B...B......",
                "B.BBB.BB.WW.W",
                "T...T....W...",
                "..D.WW.WWW.BB",
                "BB..WB.BB..G.",
                ".D..W.....rl.",
                "WWW.W.S.B.r..",
                "...DD.....rBB",
                "....BTTTBD...",
                "BBT..4D3.TB..",
                "T....R.L....."
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST
            }
        },
        // ===== Stage 6 =====
        {
            {
                ".....L.RGG...",
                ".Lr.L...RGLRG",
                ".Lr.L.B.RGLRG",
                ".B..B.S.BG.BG",
                "...Rt.B.Tl.GG",
                "BBL..GBG..RBB",
                "....RGGGL....",
                "SBB.TGGGTRBBS",
                "ttt.D.G.D.ttt",
                ".D..B...B....",
                ".BL..T.T..RBG",
                "..T..4D3..GGG",
                "..D..R.L..DGG"
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },

        // ===== Stage 7 =====
        {
            {
                ".......tt....",
                "..Sttt....S..",
                "..S...G.tSS..",
                ".S...GS...S..",
                "....GSS...tS.",
                ".S.GSSS.S....",
                ".l.SS...SS...",
                "l...S.SSS..l.",
                ".rS...SSG..S.",
                ".S....SG..SS.",
                ".ttS..G..S...",
                ".....4D3.t.lS",
                "dd...R.L....."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL
            }
        },

        // ===== Stage 8 =====
        {
            {
                "..B..B.D.B...",
                "GBBB.B.d.BL..",
                "GGG..T.B.T.RL",
                "GWWWWWWWWWW.W",
                ".B....DD.....",
                "..B..RBBTBTtt",
                "BB.B.RBBGBddB",
                "...S.d.GGGG..",
                "WW.WWWWW.WWWW",
                "GG.R..DD.....",
                "GGB.L..R.dDB.",
                "GdB.L4D3.T.B.",
                ".....R.L.D.T."
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL
            }
        },
        // ===== Stage 9 =====
        {
            {
                "...B.....dG..",
                "B.....dGrSl.B",
                "...dGrSl.tG..",
                "..rSl.tG.....",
                "...tG........",
                "...GdG.GdG...",
                "SB.rSl.rSl.BS",
                ".D.GtG.GtG...",
                "....d...d....",
                "B..rSl.rSl..B",
                "B..GtG.GtG..B",
                "..D..4D3..D..",
                "..BB.R.L.BB.."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 10 =====
        {
            {
                ".............",
                ".RTB......BTL",
                "RT..B.GG.B..R",
                "B...BGGGGB..R",
                "B..RBGSSGBL.B",
                "RDDBWWWWWWBBB",
                ".BBBSSBSSBBBL",
                "..BBS.B.SBBL.",
                "..BBBBBBBBBL.",
                "BGTTTSSTTTTGB",
                "BGGGGGGGGGGGB",
                "..GGG4D3GGGG.",
                "...L.R.L..L.."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 11 =====
        {
            {
                ".....S.B.BB..",
                ".RBBBB.B.....",
                "...L.B.BB.GGG",
                ".R.....S.GGGG",
                ".R.BBBSBBGGTS",
                ".TTTS..B.GG.R",
                "RBBB.SGGGGG..",
                "...S..GGGGGB.",
                "SBGGGGGSGGGB.",
                "RBGGGGG....BL",
                ".BGG....tBBB.",
                "..GG.4D3.B.R.",
                ".DGG.R.L....."
            },
            {
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY, //
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST
            }
        },

        // ===== Stage 12 =====
        {
            {
                ".......BBB...",
                ".BBBD.D..B...",
                "....B.T....BB",
                ".WWWWW.BL..Bt",
                "..dddW.B.SlB.",
                "B.BBBWWW.WBB.",
                "....SW...Wt..",
                "WWW.WWBB.W...",
                ".....Btt.WWW.",
                "BBB..........",
                "..B.tt...BB.R",
                "B....4D3.B..B",
                ".....R.L....."
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER, //
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY, //
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },

        // ===== Stage 13 =====
        {
            {
                "....D...D....",
                ".BBBB...BBBB.",
                ".B....B....S.",
                ".S.BT...TB.BB",
                ".B.LGdSdGR.SB",
                ".T..GGGGG..tB",
                "Bd..GGGGG..DB",
                "BS.LG...GR.B.",
                "BB.BD...DB.S.",
                "BS....B....B.",
                "BBBBB...BBBSS",
                "BB..T4D3T..B.",
                "BB...R.L....."
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER, //
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 14 =====
        {
            {
                ".............",
                "GG..DBBBD..GG",
                "G..RBBBBBL..G",
                "...BBGBGBB...",
                "...BGGBGGB...",
                "G..BBBBBBB..G",
                "GG..BGBGB..GG",
                "WWW.BBBBB.WWW",
                "....RRRRR....",
                "....LLLLL....",
                "rrr.......lll",
                "LLL..4D3..RRR",
                "lllr.R.L.lrrr"
            },
            {
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER, //
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::POWER,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY, //
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY
            }
        },
        // ===== Stage 15 =====
        {
            {
                "....BB..B....",
                ".GGBB...B....",
                "GGGGGGGGBB...",
                "GtBGBBBGGGGBS",
                "GGBGGGtGGBrB.",
                ".GGBdGGGG..B.",
                ".BBBBBGGBBLGG",
                "rtBB...BT...G",
                ".B.BLdDTGGBLG",
                ".B..RBTGGB..G",
                ".BBLRTGGDGBGG",
                "..B.G4D3BGTG.",
                "..T..R.L.GGG."
            },
            {
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::NORMAL,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::FAST, //
                Enemy::EnemyType::FAST,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY,
                Enemy::EnemyType::HEAVY, //
                Enemy::EnemyType::HEAVY
            }
        }
    };

    int count = static_cast<int>(stages.size());

    if (count == 0) {
        return {};
    }

    if (stageNo <= 0) {
        stageNo = 1;
    }

    int index = (stageNo - 1) % count;

    return stages[index];
}

int StageData::GetStageCount() {
    return 15;
}
