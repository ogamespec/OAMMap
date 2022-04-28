// OAMMap.cpp: A small utility for creating a visual map of the PPU OAM.

#include "pch.h"

void EmitCellText(std::string &text, size_t bit_num, size_t row, size_t col, size_t pp[8], bool ntsc_ppu)
{
    char cell[0x400]{};
    const char* CellColor = "";
    char CellLabel[0x20]{};

    size_t oam_addr;

    if (row == 8)
    {
        oam_addr = col;    // Temp OAM
    }
    else
    {
        oam_addr = (col << 3) | row;
        if (ntsc_ppu)
        {
            oam_addr = ~oam_addr;
        }
    }

    sprintf_s(CellLabel, sizeof(CellLabel), "%02X-%zd\nrow: %zd\ncol: %zd", (uint8_t)oam_addr, bit_num, row, col);

    if (row == 8)
    {
        CellColor = "DarkViolet";       // Temp OAM
    }
    else
    {
        switch (oam_addr & 3)
        {
            case 0:
                CellColor = "LimeGreen";    // Y
                break;
            case 1:
                CellColor = "Ivory";    // Index
                break;
            case 2:
                CellColor = "Red";      // Attr
                break;
            case 3:
                CellColor = "Blue";     // X
                break;
        }
    }

    sprintf_s(cell, sizeof(cell), cellText, CellLabel, CellColor,
        pp[0], pp[1],
        pp[2], pp[3],
        pp[4], pp[5],
        pp[6], pp[7]);

    text += cell;
}

void GenMap(bool ntsc_ppu)
{
    std::string text = "";

    text += headerText;

    const size_t originOffset = 10ULL;
    const size_t cellSize = 10ULL;
    const size_t GapBetweenBanks = 30ULL;
    const size_t GapBetweenCells = 1ULL;

    size_t pp[8]{};

    size_t y = originOffset;

    for (int n = 7; n >= 0; n--)
    {
        for (size_t row = 0; row < 9; row++)
        {
            if ((row == 2 || row == 6) && (n == 2 || n == 3 || n == 4))
                continue;

            for (size_t col = 0; col < 32; col++)
            {
                size_t x = cellSize * (32 - col) + originOffset + (32 - col) * GapBetweenCells;
                size_t cell_y = y;

                if (n < 4)
                {
                    cell_y += GapBetweenBanks;
                }

                pp[0] = x;
                pp[1] = cell_y;
                pp[2] = x + cellSize;
                pp[3] = cell_y;
                pp[4] = x + cellSize;
                pp[5] = cell_y + cellSize;
                pp[6] = x;
                pp[7] = cell_y + cellSize;

                EmitCellText(text, n, row, col, pp, ntsc_ppu);
            }

            y += cellSize + GapBetweenCells;
        }

    }

    text += footerText;

    FILE* f;
    fopen_s(&f, ntsc_ppu ? "oammap_ntsc.xml" : "oammap_pal.xml", "wt");
    if (f)
    {
        fwrite(text.c_str(), 1, text.size(), f);
        fclose(f);
    }
}

int main()
{
    GenMap(true);
    GenMap(false);
}
