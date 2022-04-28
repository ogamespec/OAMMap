// OAMMap.cpp: A small utility for creating a visual map of the PPU OAM.

#include "pch.h"

void EmitCellText(std::string &text, size_t bit_num, size_t row, size_t col, size_t pp[8])
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

const size_t originOffset = 10ULL;
const size_t cellSize = 10ULL;
const size_t GapBetweenBanks = 30ULL;
const size_t GapBetweenCells = 1ULL;

void GenRowLane(std::string & text, size_t& y, size_t bit_num, size_t row, bool ntsc_ppu)
{
    int cols_order_ntsc[] = { 31, 15, 23, 7, 27, 11, 19, 3, 30, 14, 22, 6, 26, 10, 18, 2, 29, 13, 21, 5, 25, 9, 17, 1, 28, 12, 20, 4, 24, 8, 16, 0 };
    int cols_order_pal[] = { 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    int* cols_order = ntsc_ppu ? cols_order_ntsc : cols_order_pal;

    size_t pp[8]{};

    for (size_t n = 0; n < 32; n++)
    {
        size_t col = cols_order[n];

        size_t x = (cellSize + GapBetweenCells) * n + originOffset;
        size_t cell_y = y;

        if (bit_num < 4)
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

        EmitCellText(text, bit_num, row, col, pp);
    }

    y += cellSize + GapBetweenCells;
}

void GenMap(bool ntsc_ppu)
{
    // Order of rows
    int rows_order_ntsc[] = { 1, 3, 5, 7, 0, 2, 4, 6, 8 };
    int rows_order_pal[] = { 6, 4, 2, 0, 7, 5, 3, 1, 8 };
    int rows_order_ntsc_24[] = { 1, 3, 7, 0, 4, 5, 8 };
    int rows_order_pal_24[] = { 5, 4, 0, 7, 3, 1, 8 };

    std::string text = "";

    text += headerText;

    size_t y = originOffset;

    for (int n = 7; n >= 0; n--)
    {
        bool bits_24 = (n == 2 || n == 3 || n == 4);

        if (bits_24)
        {
            if (ntsc_ppu)
            {
                for (size_t p = 0; p < _countof(rows_order_ntsc_24); p++)
                {
                    GenRowLane(text, y, n, rows_order_ntsc_24[p], ntsc_ppu);
                }
            }
            else
            {
                for (size_t p = 0; p < _countof(rows_order_pal_24); p++)
                {
                    GenRowLane(text, y, n, rows_order_pal_24[p], ntsc_ppu);
                }
            }
        }
        else
        {
            if (ntsc_ppu)
            {
                for (size_t p = 0; p < _countof(rows_order_ntsc); p++)
                {
                    GenRowLane(text, y, n, rows_order_ntsc[p], ntsc_ppu);
                }
            }
            else
            {
                for (size_t p = 0; p < _countof(rows_order_pal); p++)
                {
                    GenRowLane(text, y, n, rows_order_pal[p], ntsc_ppu);
                }
            }
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

/// <summary>
/// Output the logical NTSC OAM column numbers.
/// The output is from right to left relative to the physical addressing of the columns.
/// </summary>
void PrintNtscColumns()
{
    for (int n = 0; n < 4; n++)
    {
        printf("%d ", (0 << 2) | n);
        printf("%d ", (4 << 2) | n);
        printf("%d ", (2 << 2) | n);
        printf("%d ", (6 << 2) | n);
        printf("%d ", (1 << 2) | n);
        printf("%d ", (5 << 2) | n);
        printf("%d ", (3 << 2) | n);
        printf("%d\n", (7 << 2) | n);
    }
}

int main()
{
    PrintNtscColumns();
    GenMap(true);
    GenMap(false);
}
