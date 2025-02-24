#pragma once
#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
#include <set>

enum sourceType
{
    original,
    add
};

struct Piece
{
    sourceType source;
    unsigned int start;
    unsigned int length;
};

struct CachedPiece
{
    unsigned int idx;  // Index of currentPiece
    unsigned int cLen; // Length at currentPiece start relative to total docstring
};

// Sizes including deletes of buffer strings

class PieceTable
{
public:
    PieceTable();
    PieceTable(const std::string &originalText);
    void Insert(unsigned int rawIndex, const std::string &text);
    void Delete(unsigned int rawIndex, unsigned int length);
    std::string GetContent();
    char GetCharAt(unsigned int docIndex) const;

    // Active Piece functions expect bounded index

    // Checks if given document index is in active piece
    bool ValidActivePiece(unsigned int docIndex);
    // Gets current piece and makes it the active piece
    Piece *GetActivePiece(unsigned int docIndex);
    // Gets the piece by index in document string
    Piece *GetPiece(unsigned int docIndex, bool activate = false);

    unsigned int MoveCursorX(unsigned int currOffset, int dir);
    unsigned int MoveCursorY(unsigned int currOffset, int dir);

    // Visual Debugging Function
    void Visualize();

    unsigned int documentLength = 0;

private:
    std::string originalBuffer;
    std::string addBuffer;
    std::vector<Piece> pieces;
    CachedPiece activePiece = {0, 0}; // Utilized to prevent spamming new pieces when middle modification
    int lastDeletedIndex = -1;        // Prevents inclusion of deleted text
    std::vector<unsigned int> lineStarts;
};

#endif