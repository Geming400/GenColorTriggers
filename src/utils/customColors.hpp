#pragma once

#include <vector>
#include <map>

enum CustomColors {
    BG = 1000,
    G1,
    LINE,
    // 3DL
    THREEDL,
    OBJ,
    P1,
    P2,
    // Light BG
    LBG,
    G2 = 1009,
    BLACK,
    WHITE,
    // A lighter version of the primary color in objects.
    LIGHTER,
    MG,
    MG2
};

const std::vector<int> allCustomColors = {
	(int) CustomColors::BG,
	(int) CustomColors::G1,
	(int) CustomColors::G2,
	(int) CustomColors::LINE,
	(int) CustomColors::THREEDL,
	(int) CustomColors::OBJ,
	(int) CustomColors::LBG,
	(int) CustomColors::MG,
	(int) CustomColors::MG2,

	(int) CustomColors::LIGHTER,
	(int) CustomColors::BLACK,
	(int) CustomColors::WHITE
};

const std::map<int, std::string> customColorNames = {
	{ (int) CustomColors::BG, "BG" },
	{ (int) CustomColors::G1, "G1" },
	{ (int) CustomColors::G2, "G2" },
	{ (int) CustomColors::LINE, "L" },
	{ (int) CustomColors::THREEDL, "3DL" },
	{ (int) CustomColors::OBJ, "Obj" },
	{ (int) CustomColors::LBG, "LBG" },
	{ (int) CustomColors::MG, "MG"},
	{ (int) CustomColors::MG2, "mg2" },
	{ (int) CustomColors::P1, "P1" },
	{ (int) CustomColors::P2, "P2" }
};