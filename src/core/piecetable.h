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

// Sizes including deletes of buffer strings

class PieceTable
{
public:
    PieceTable();
    PieceTable(const string &originalText);
    void Insert(unsigned int index, const string &text);
    void Delete(unsigned int index, unsigned int length);
    string getContent() const;
    char getCharAt(unsigned int index) const;
    vector<unsigned int> getLineStarts(sourceType src, unsigned int start, unsigned int end) const;

private:
    string originalBuffer;
    string addBuffer;
    vector<Piece> pieces;
    Piece *currentPiece; // Utilized to prevent spamming new pieces when middle modification
    unsigned int documentLength;
    int lastDeletedIndex = -1; // Prevents inclusion of deleted text
};

#endif