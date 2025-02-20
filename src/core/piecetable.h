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
    std::set<unsigned int> lineStart;
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
    std::string GetContent() const;
    char GetCharAt(unsigned int rawIndex) const;
    std::set<unsigned int> GetLineStarts(sourceType src, unsigned int start, unsigned int end) const;

    // Active Piece functions expect bounded index
    bool ValidActivePiece(unsigned int index);
    Piece *GetActivePiece(unsigned int index);
    unsigned int GetPrevLine(unsigned int currOffset);
    unsigned int GetNextLine(unsigned int currOffset);

    // Visual Debugging Function
    void Visualize();

    unsigned int documentLength = 0;

private:
    std::string originalBuffer;
    std::string addBuffer;
    std::vector<Piece> pieces;
    CachedPiece activePiece = {0, 0}; // Utilized to prevent spamming new pieces when middle modification
    unsigned int CurrentLine[2];      // Range of current line based of new line chars only stores end \n
    int lastDeletedIndex = -1;        // Prevents inclusion of deleted text
};

#endif