#pragma once
#ifndef PIECETABLE_H
#define PIECETABLE_H

#include <vector>
#include <string>
using namespace std;

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
    vector<unsigned int> lineStart;
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
    PieceTable(const string &originalText);
    void Insert(unsigned int rawIndex, const string &text);
    void Delete(unsigned int rawIndex, unsigned int length);
    string GetContent() const;
    char GetCharAt(unsigned int rawIndex) const;
    vector<unsigned int> GetLineStarts(sourceType src, unsigned int start, unsigned int end) const;

    // Active Piece functions expect bounded index
    bool ValidActivePiece(unsigned int index);
    Piece *GetActivePiece(unsigned int index);

    // Visual Debugging Function
    void Visualize();

    unsigned int documentLength = 0;

private:
    string originalBuffer;
    string addBuffer;
    vector<Piece> pieces;
    CachedPiece activePiece = {0, 0}; // Utilized to prevent spamming new pieces when middle modification
    int lastDeletedIndex = -1;            // Prevents inclusion of deleted text
};

#endif